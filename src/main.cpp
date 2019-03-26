#include "fem.hpp"

#include <iostream>

// const char* argparse_str =
//     "s|script: m|mesh: r|res|[500]: "
//     "c|cmap|[parula]: p|plot: pt|plot-tri: load-matrix: save-matrix: "
//     "load-forcing: save-forcing: load-coef: save-coef:"
//     "h|?|help ";
const char* argparse_str =
    "s|script: "
    "m|mesh: "
    "load-system "
    "load-forcing "
    "load-approx "
    "no-save "
    "no-load "
    "p|plot: "
    "pt|plot-tri: "
    "pb|plot-both: "
    "pf|plot-func: "
    "tp|tikz-plot: "
    "tt|tikz-tri: "
    "tb|tikz-both: "
    "c|cmap|[parula]: "
    "r|res|[500]: "
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
  fem::log::status("PARSE ARGUMENTS");
  auto args = fem::argparse::parse_args(argparse_str, argc, argv);
  if (args.flags["help"]) {
    fem::argparse::print_help(argparse_help, 11, "[OPTIONS]", "fem");
    return 0;
  }
  fem::log::success("PARSE ARGUMENTS");

  /////////////////////////////////////////////////////////////////////////////
  // SCRIPT INITIALIZATION                                                   //
  /////////////////////////////////////////////////////////////////////////////
  fem::log::status("INITALIZE LUA SCRIPT");
  if (args.options["script"] != "") {
    fem::script::open_script(args.options["script"]);
  } else {
    fem::log::warning(
        "All processes with the exception of plotting requires a lua script!");
    if (!args.flags["load-approx"]) {
      fem::log::error("User defined forcing function requires a LUA script.");
      return -1;
    }
  }
  fem::log::success("INITALIZE LUA SCRIPT");

  /////////////////////////////////////////////////////////////////////////////
  // MESH INITIALIZATION                                                     //
  /////////////////////////////////////////////////////////////////////////////
  fem::log::status("INITALIZE MESH");
  fem::mesh::Mesh mesh;
  if (args.options["mesh"] != "") {
    mesh = fem::mesh::Mesh(args.options["mesh"]);
  } else {
    fem::log::error("Must define a mesh file for all functionalities.");
    return -2;
  }
  fem::log::success("INITALIZE MESH");

  /////////////////////////////////////////////////////////////////////////////
  // SAVED CHECK                                                             //
  /////////////////////////////////////////////////////////////////////////////
  if (!args.flags["no-load"] &&
      fem::file_exists(args.options["mesh"] + "/A.mat") &&
      fem::file_exists(args.options["mesh"] + "/F.vec")) {
    args.flags["load-system"] = true;
  }

  /////////////////////////////////////////////////////////////////////////////
  // CONSTRUCT SYSTEM                                                        //
  /////////////////////////////////////////////////////////////////////////////
  fem::log::status("CONSTRUCT SYSTEM");
  fem::math::Matrix A;
  fem::math::Vector F;
  if (!args.flags["load-approx"] && !args.flags["load-forcing"]) {
    A = fem::math::Matrix(mesh.pts.size());
    F = fem::math::Vector(mesh.pts.size(), 0.0);
    for (unsigned long e = 0; e < mesh.tri.size(); ++e) {
      for (unsigned long i = 0; i < 3; ++i) {
        F[mesh.tri[e][i]] +=
            fem::math::integrate(fem::sys_rhs(mesh, e, i, "force"), e, mesh);
        if (!args.flags["load-system"]) {
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
  if (args.flags["load-system"]) {
    A = fem::math::load_mat_from_file(args.options["mesh"] + "/A.mat");
  }
  if (args.flags["load-forcing"]) {
    F = fem::math::load_vec_from_file(args.options["mesh"] + "/F.vec");
  }
  if (!args.flags["no-save"] && !args.flags["load-forcing"] &&
      !args.flags["load-approx"]) {
    fem::math::save_vec_to_file(args.options["mesh"] + "/F.vec", F);
  }
  fem::log::success("CONSTRUCT SYSTEM");

  /////////////////////////////////////////////////////////////////////////////
  // APPLY BOUNDARY CONDITIONS                                               //
  /////////////////////////////////////////////////////////////////////////////
  fem::log::status("APPLY BOUNDARY CONDITIONS");
  if (!args.flags["load-approx"]) {
    for (unsigned i = 0; i < mesh.pts.size(); ++i) {
      if (mesh.is_boundary(i)) {
        F[i] = mesh.boundary(i);
        if (!args.flags["load-system"]) {
          for (unsigned j = 0; j < mesh.pts.size(); ++j) {
            if (i == j) {
              A.set(i, j, 1.0);
            } else {
              A.set(i, j, 0.0);
            }
          }
        }
      }
    }
  }

  if (!args.flags["no-save"] && !args.flags["load-system"] &&
      !args.flags["load-approx"]) {
    fem::math::save_mat_to_file(args.options["mesh"] + "/A.mat", A);
  }
  fem::log::success("APPLY BOUNDARY CONDITIONS");

  /////////////////////////////////////////////////////////////////////////////
  // SOLVING THE SYSTEM                                                      //
  /////////////////////////////////////////////////////////////////////////////
  fem::log::status("SOLVE SYSTEM");
  fem::math::Vector U;
  if (!args.flags["load-approx"]) {
    U = fem::math::solve(A, F, F.size() * 2);
  } else {
    U = fem::math::load_vec_from_file(args.options["mesh"] + "/coef.vec");
  }

  if (!args.flags["no-save"] && !args.flags["load-approx"]) {
    fem::math::save_vec_to_file(args.options["mesh"] + "/coef.vec", U);
  }
  fem::log::success("SOLVE SYSTEM");

  /////////////////////////////////////////////////////////////////////////////
  // PLOTTING                                                                //
  /////////////////////////////////////////////////////////////////////////////
  if (args.options["plot"] != "" || args.options["plot-both"] != "") {
    fem::log::status("PLOT APPROXIMATION");
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
    if (args.options["plot"] != "") {
      fem::plot::imsave(args.options["plot"], vals, args.options["cmap"]);
    }
    if (args.options["plot-both"] != "") {
      fem::plot::imsave(args.options["plot-both"], vals, args.options["cmap"],
                        std::numeric_limits<double>::infinity(),
                        -std::numeric_limits<double>::infinity(), &mesh);
    }
    fem::log::success("PLOT APPROXIMATION");
  }
  if (args.options["plot-tri"] != "") {
    fem::log::status("PLOT TRIANGLES");
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
    fem::log::success("PLOT TRIANGLES");
  }

  if (args.options["plot-func"] != "") {
    fem::log::status("PLOT FUNCTION");
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
        vals.back().push_back(
            fem::script::call(x, y, args.options["plot-func"]));
      }
    }
    fem::plot::imsave(args.options["plot-func"] + ".bmp", vals,
                      args.options["cmap"]);
    fem::log::success("PLOT FUNCTION");
  }

  /////////////////////////////////////////////////////////////////////////////
  // PLOTTING TIKZ                                                           //
  /////////////////////////////////////////////////////////////////////////////
  if (args.options["tikz-tri"] != "") {
    fem::log::status("TIKZ TRIANGLES");
    mesh.save_tikz(args.options["tikz-tri"]);
    fem::log::success("TIKZ TRIANGLES");
  }

  /////////////////////////////////////////////////////////////////////////////
  // TERMINATION                                                             //
  /////////////////////////////////////////////////////////////////////////////
  fem::script::close_script();
  return 0;
}
