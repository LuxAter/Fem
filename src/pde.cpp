#include "pde.hpp"

#include <string>

#include <sys/stat.h>
#include <unistd.h>

#include "basis.hpp"
#include "linalg.hpp"
#include "logger.hpp"
#include "mesh.hpp"
#include "script.hpp"
#include "timer.hpp"

arta::PDE::PDE() : script_source() {}
arta::PDE::PDE(argparse::Arguments args)
    : script_source(args.options["script"]),
      mesh_source(args.options["mesh"]),
      mesh_constraints({{args.getf("mesh-area"), args.getf("mesh-angle")}}),
      timer(args.flags["time"]) {
  load_script();
  load_mesh();
  construct_matrices();
  construct_forcing(0.0);
}
arta::PDE::PDE(std::string script_, std::string mesh_src,
               std::array<double, 2> mesh_const, bool time)
    : script_source(script_),
      mesh_source(mesh_src),
      mesh_constraints(mesh_const),
      timer(time) {
  load_script();
  load_mesh();
}
arta::PDE::~PDE() { script::close_script(); }

void arta::PDE::construct_matrices() {
  if (timer) {
    time::start();
  }

  if (access((dest_dir + "G.mat").c_str(), F_OK) != -1 &&
      access((dest_dir + "M.mat").c_str(), F_OK) != -1) {
    G_ = linalg::load_mat_from_file(dest_dir + "G.mat");
    M_ = linalg::load_mat_from_file(dest_dir + "M.mat");
  } else {
    G_ = linalg::Matrix(mesh.pts.size());
    M_ = linalg::Matrix(mesh.pts.size());
    for (unsigned long ele = 0; ele < mesh.tri.size(); ++ele) {
      for (unsigned long i = 0; i < 3; ++i) {
        for (unsigned long j = 0; j < 3; ++j) {
          G_.set(mesh.tri[ele][i], mesh.tri[ele][j],
                 G_.at(mesh.tri[ele][i], mesh.tri[ele][j]) +
                     calc::integrate(G(ele, i, j), ele, &mesh));
          M_.set(mesh.tri[ele][i], mesh.tri[ele][j],
                 M_.at(mesh.tri[ele][i], mesh.tri[ele][j]) +
                     calc::integrate(A(ele, i, j), ele, &mesh) +
                     calc::integrate(B(ele, i, j), ele, &mesh) +
                     calc::integrate(C(ele, i, j), ele, &mesh));
        }
      }
    }
    for (unsigned i = 0; i < mesh.pts.size(); ++i) {
      if (mesh.is_boundary(i)) {
        for (unsigned j = 0; j < mesh.pts.size(); ++j) {
          if (i == j) {
            M_.set(i, j, 1.0);
          } else {
            M_.set(i, j, 0.0);
          }
        }
      }
    }
    linalg::save_mat_to_file(dest_dir + "G.mat", G_);
    linalg::save_mat_to_file(dest_dir + "M.mat", M_);
  }
  if (timer) {
    log::status("Construct Matricies: %f", time::stop());
  }
}

void arta::PDE::construct_forcing(const double& t) {
  time_ = t;
  if (timer) {
    time::start();
  }
  if (access((dest_dir + "F.vec").c_str(), F_OK) != -1) {
    F_ = linalg::load_vec_from_file(dest_dir + "F.vec");
  } else {
    F_ = linalg::Vector(mesh.pts.size(), 0.0);
    for (unsigned long ele = 0; ele < mesh.tri.size(); ++ele) {
      for (unsigned long i = 0; i < 3; ++i) {
        F_[mesh.tri[ele][i]] += calc::integrate(F(ele, i, time_), ele, &mesh);
      }
    }
    for (unsigned i = 0; i < mesh.pts.size(); ++i) {
      if (mesh.is_boundary(i)) {
        F_[i] = script::boundary(mesh.bdry_index[i], mesh.pts[i].x,
                                 mesh.pts[i].y, time_);
      }
    }
    linalg::save_vec_to_file(dest_dir + "F.vec", F_);
  }
  if (timer) {
    log::status("Construct Forcing: %f", time::stop());
  }
}

arta::linalg::Vector arta::PDE::solve_time_indep() {
  if (timer) {
    time::start();
  }
  U_ = linalg::solve(M_, F_, F_.size());
  if (timer) {
    log::status("Solving Time Indep: %f", time::stop());
  }
  return U_;
}

double arta::PDE::approx(const double& x, const double& y,
                         const unsigned& e) const {
  double val = 0.0;
  for (unsigned i = 0; i < U_.size(); ++i) {
    val += (U_[i] * basis::global(&mesh, x, y, e, i));
  }
  return val;
}

std::function<double(double, double)> arta::PDE::G(const unsigned& e,
                                                   const unsigned& i,
                                                   const unsigned& j) {
  mesh::Mesh* mesh_ptr = &mesh;
  return [mesh_ptr, e, i, j](double x, double y) {
    return basis::local(mesh_ptr, x, y, e, i) *
           basis::local(mesh_ptr, x, y, e, j);
  };
}
std::function<double(double, double)> arta::PDE::A(const unsigned& e,
                                                   const unsigned& i,
                                                   const unsigned& j) {
  mesh::Mesh* mesh_ptr = &mesh;
  return [mesh_ptr, e, i, j](double x, double y) {
    return -(calc::derivative(script::A, x, y, mesh_ptr->grain_size(e), 0.0, 1,
                              1) *
                 basis::local(mesh_ptr, x, y, e, i) +
             script::A(x, y, 1, 1) * basis::dlocal_dx(mesh_ptr, x, y, e, i)) *
               basis::dlocal_dx(mesh_ptr, x, y, e, j) -
           (calc::derivative(script::A, x, y, mesh_ptr->grain_size(e), 0.0, 1,
                             2) *
                basis::local(mesh_ptr, x, y, e, i) +
            script::A(x, y, 1, 2) * basis::dlocal_dx(mesh_ptr, x, y, e, i)) *
               basis::dlocal_dy(mesh_ptr, x, y, e, j) -
           (calc::derivative(script::A, x, y, 0.0, mesh_ptr->grain_size(e), 2,
                             1) *
                basis::local(mesh_ptr, x, y, e, i) +
            script::A(x, y, 2, 1) * basis::dlocal_dy(mesh_ptr, x, y, e, i)) *
               basis::dlocal_dx(mesh_ptr, x, y, e, j) -
           (calc::derivative(script::A, x, y, 0.0, mesh_ptr->grain_size(e), 2,
                             2) *
                basis::local(mesh_ptr, x, y, e, i) +
            script::A(x, y, 2, 2) * basis::dlocal_dy(mesh_ptr, x, y, e, i)) *
               basis::dlocal_dy(mesh_ptr, x, y, e, j);
  };
}
std::function<double(double, double)> arta::PDE::B(const unsigned& e,
                                                   const unsigned& i,
                                                   const unsigned& j) {
  mesh::Mesh* mesh_ptr = &mesh;
  return [mesh_ptr, e, i, j](double x, double y) {
    return script::B(x, y, 1) * basis::local(mesh_ptr, x, y, e, i) *
               basis::dlocal_dx(mesh_ptr, x, y, e, j) +
           script::B(x, y, 2) * basis::local(mesh_ptr, x, y, e, i) *
               basis::dlocal_dy(mesh_ptr, x, y, e, j);
  };
}
std::function<double(double, double)> arta::PDE::C(const unsigned& e,
                                                   const unsigned& i,
                                                   const unsigned& j) {
  mesh::Mesh* mesh_ptr = &mesh;
  return [mesh_ptr, e, i, j](double x, double y) {
    return script::C(x, y) * basis::local(mesh_ptr, x, y, e, i) *
           basis::local(mesh_ptr, x, y, e, j);
  };
}
std::function<double(double, double)> arta::PDE::F(const unsigned& e,
                                                   const unsigned& i,
                                                   const double& t) {
  mesh::Mesh* mesh_ptr = &mesh;
  return [mesh_ptr, e, i, t](double x, double y) {
    return basis::local(mesh_ptr, x, y, e, i) * script::forcing(x, y, t);
  };
}

void arta::PDE::load_script() {
  if (timer) {
    time::start();
  }
  script::load_script(script_source);
  if (timer) {
    log::status("Script Load: %f", time::stop());
  }
  dest_dir = "./" +
             script_source.substr(
                 script_source.rfind('/') + 1,
                 script_source.rfind('.') - script_source.rfind('/') - 1) +
             '/';
}

void arta::PDE::load_mesh() {
  if (script::has("mesh") && mesh_source == "") {
    mesh_source = script::gets("mesh");
  }
  if (script::has("mesh_area") && mesh_constraints[0] == -1) {
    mesh_constraints[0] = script::getd("mesh_area");
  }
  if (script::has("mesh_angle") && mesh_constraints[1] == -1) {
    mesh_constraints[1] = script::getd("mesh_angle");
  }
  if (mesh_constraints[0] == -1) mesh_constraints[0] = 1.0;
  if (mesh_constraints[1] == -1) mesh_constraints[1] = 10.0;
  if (mesh_source != "") {
    if (timer) {
      time::start();
    }
    std::string mesh_base_name(mesh_source);
    mesh_base_name.erase(0, mesh_base_name.find_last_of("\\/") + 1);
    mesh_base_name.erase(mesh_base_name.rfind('.'));
    std::string mesh_dest = dest_dir + mesh_base_name;
    if (access((mesh_dest + ".ele").c_str(), F_OK) != -1 &&
        access((mesh_dest + ".neigh").c_str(), F_OK) != -1 &&
        access((mesh_dest + ".node").c_str(), F_OK) != -1 &&
        access((mesh_dest + ".poly").c_str(), F_OK) != -1) {
    } else {
      mesh::construct_mesh(mesh_source, dest_dir, mesh_constraints[0],
                           mesh_constraints[1]);
    }
    mesh = mesh::Mesh(mesh_dest);
    log::info("Verts: %ld Tris: %ld", mesh.pts.size(), mesh.tri.size());
    if (timer) {
      log::status("Mesh Gen/Load: %f", time::stop());
    }
  } else {
    log::error("Must define a mesh file either by command line or by script.");
  }
}
