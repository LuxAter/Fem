#include "fem.hpp"

#include <iostream>

const char* argparse_str =
    "s|script: m|mesh: r|res|[500]: "
    "c|cmap|[parula]: p|plot: pt|plot-tri: load-matrix: save-matrix: "
    "load-forcing: save-forcing: load-coef: save-coef:"
    "h|?|help ";

const char* argparse_help[] = {
    "s script [script/funcs.lua]:Sets LUA script file to read user define "
    "functions from.",
    "m mesh [.mesh/trial]:Which Triangle mesh to use for approximation.",
    "r res [500]:Resolution of output plot if plotting.",
    "c cmap [parula]:Color map of output plot if plotting.",
    "p plot:Plots resulting approximation to file.",
    "pt plot-tri:Plots triangles of loaded mesh.",
    "load-matrix:`A` matrix to load instead of constructing from mesh.",
    "save-matrix:Saves `A` matrix to output file.",
    "load-forcing:`F` vector to load instead of construction from mesh and "
    "forcing function.",
    "save-forcing:Saves `F` vector to output file.",
    "load-coef:Load coefficients vector for plotting.",
    "save-coef:Save coefficients vector for later analysis."};

int main(int argc, char* argv[]) {
  /////////////////////////////////////////////////////////////////////////////
  // PARSE ARGUMENTS                                                         //
  /////////////////////////////////////////////////////////////////////////////
  auto args = fem::argparse::parse_args(argparse_str, argc, argv);
  if (args.flags["help"]) {
    fem::argparse::print_help(argparse_help, 11, "[OPTIONS]", "fem");
    return 0;
  }

  /////////////////////////////////////////////////////////////////////////////
  // SCRIPT INITIALIZATION                                                   //
  /////////////////////////////////////////////////////////////////////////////
  if (args.options["script"] != "") {
    fem::script::open_script(args.options["script"]);
  } else {
    fem::log::warning(
        "Any user defined function based boundary conditions requires a LUA "
        "script.");
    if (!args.flags["load-forcing"]) {
      fem::log::error("User defined forcing function requires a LUA script.");
      return -1;
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // MESH INITIALIZATION                                                     //
  /////////////////////////////////////////////////////////////////////////////
  fem::mesh::Mesh mesh;
  if (args.options["mesh"] != "") {
    mesh = fem::mesh::Mesh(args.options["mesh"]);
  } else {
    fem::log::error("Must define a mesh file for all functionalities.");
    return -2;
  }

  // TODO Differentiation test!
  // double val = fem::basis::local_basis_deriv_x(mesh, {0.5, 0.0}, 0, 2);
  // std::cout << "ANALYTICAL: " << val << "\n";
  // val = fem::basis::local_basis_deriv_y(mesh, {0.5, 0.0}, 0, 2);
  // std::cout << "ANALYTICAL: " << val << "\n";
  // return 0;

  /////////////////////////////////////////////////////////////////////////////
  // CONSTRUCT MATRIX                                                        //
  /////////////////////////////////////////////////////////////////////////////
  fem::math::Matrix A;
  fem::math::Vector F;
  if ((args.options["load-matrix"] == "" ||
       args.options["load-forcing"] == "") &&
      args.options["load-coef"] == "") {
    A = fem::math::Matrix(mesh.pts.size());
    F = fem::math::Vector(mesh.pts.size(), 0.0);
    for (unsigned long e = 0; e < mesh.tri.size(); ++e) {
      for (unsigned long i = 0; i < 3; ++i) {
        if (args.options["load-forcing"] == "") {
          F[mesh.tri[e][i]] += fem::math::integrate(
              fem::sys_rhs(mesh, e, i, "forcing"), e, mesh);
        }
        if (args.options["load-matrix"] == "") {
          for (unsigned long j = 0; j < 3; ++j) {
            A.set(
                mesh.tri[e][i], mesh.tri[e][j],
                A.at(mesh.tri[e][i], mesh.tri[e][j]) +
                    fem::math::integrate(fem::sys_lhs(mesh, e, i, j), e, mesh));
          }
        }
      }
    }
  }
  if (args.options["load-matrix"] != "") {
    A = fem::math::load_mat_from_file(args.options["load-matrix"]);
  }
  if (args.options["load-forcing"] != "") {
    F = fem::math::load_vec_from_file(args.options["load-vector"]);
  }

  if (args.options["save-matrix"] != "") {
    fem::math::save_mat_to_file(args.options["save-matrix"], A);
  }
  if (args.options["save-forcing"] != "") {
    fem::math::save_vec_to_file(args.options["save-forcing"], F);
  }

  /////////////////////////////////////////////////////////////////////////////
  // APPLY BOUNDARY CONDITIONS                                               //
  /////////////////////////////////////////////////////////////////////////////
  for (unsigned i = 0; i < mesh.pts.size(); ++i) {
    if (mesh.is_boundary(i)) {
      F[i] = mesh.boundary(i);
      for (unsigned j = 0; j < mesh.pts.size(); ++j) {
        if (i == j) {
          A.set(i, j, 1.0);
        } else {
          A.set(i, j, 0.0);
        }
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // SOLVING THE SYSTEM                                                      //
  /////////////////////////////////////////////////////////////////////////////
  fem::math::Vector U;
  if (args.options["load-coef"] == "") {
    U = fem::math::solve(A, F, F.size() * 2);
  } else {
    U = fem::math::load_vec_from_file(args.options["load-coef"]);
  }

  if (args.options["save-coef"] != "") {
    fem::math::save_vec_to_file(args.options["save-coef"], U);
  }

  fem::log::debug("ERROR: %s", fem::fmt_val(fem::math::norm(F - (A * U))).c_str());

  /////////////////////////////////////////////////////////////////////////////
  // PLOTTING                                                                //
  /////////////////////////////////////////////////////////////////////////////
  if (args.options["plot"] != "") {
    float res = args.geti("res");
    float step = (mesh.bounds[2] - mesh.bounds[0]) / res;
    std::vector<std::vector<double>> vals;
    for (double y = mesh.bounds[1]; y <= mesh.bounds[3]; y += step) {
      vals.push_back(std::vector<double>());
      for (double x = mesh.bounds[0]; x <= mesh.bounds[2]; x += step) {
        int t = mesh.locate({x, y});
        if (t < 0) {
          vals.back().push_back(std::numeric_limits<double>::quiet_NaN());
          continue;
        }
        vals.back().push_back(fem::approx(U, mesh, t, {x, y}));
      }
    }
    fem::plot::imsave(args.options["plot"], vals, args.options["cmap"]);
  }
  if (args.options["plot-tri"] != "") {
    float res = args.geti("res");
    float step = (mesh.bounds[2] - mesh.bounds[0]) / res;
    std::vector<std::vector<double>> vals;
    for (double y = mesh.bounds[1]; y <= mesh.bounds[3]; y += step) {
      vals.push_back(std::vector<double>());
      for (double x = mesh.bounds[0]; x <= mesh.bounds[2]; x += step) {
        int t = mesh.locate({x, y});
        if (t < 0) {
          vals.back().push_back(std::numeric_limits<double>::quiet_NaN());
          continue;
        }
        vals.back().push_back(t);
      }
    }
    fem::plot::imsave(args.options["plot-tri"], vals, args.options["cmap"]);
  }

  /////////////////////////////////////////////////////////////////////////////
  // TERMINATION                                                             //
  /////////////////////////////////////////////////////////////////////////////
  fem::script::close_script();
  return 0;
}
