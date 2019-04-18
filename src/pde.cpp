#include "pde.hpp"

#include <string>

#include <sys/stat.h>
#include <unistd.h>

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
  linalg::Matrix G(mesh.pts.size()), A(mesh.pts.size());
  linalg::Vector F(mesh.pts.size(), 0.0);
  for (unsigned long ele = 0; ele < mesh.tri.size(); ++ele) {
    for (unsigned long i = 0; i < 3; ++i) {
      F[mesh.tri[ele][i]] += 0.0;  // INTEGRATION!!!!
      for (unsigned long j = 0; j < 3; ++j) {
        G.set(
            mesh.tri[ele][i], mesh.tri[ele][j],
            G.at(mesh.tri[ele][i], mesh.tri[ele][j]) + 0.0);  // INTEGRATION!!!
        A.set(
            mesh.tri[ele][i], mesh.tri[ele][j],
            A.at(mesh.tri[ele][i], mesh.tri[ele][j]) + 0.0);  // INTEGRATION!!!
      }
    }
  }
  if (timer) {
    log::status("Construct Matricies: %f", time::stop());
  }
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
