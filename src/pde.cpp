#include "pde.hpp"

#include <string>

#include "mesh.hpp"
#include "script.hpp"

arta::PDE::PDE() : script_source() {}
arta::PDE::PDE(std::string script_, std::string mesh)
    : script_source(script_), mesh_source(mesh) {
  script::load_script(script_source);
  if (mesh_source != "") {
    mesh::construct_mesh(
        mesh_source,
        "./" +
            script_source.substr(
                script_source.rfind('/') + 1,
                script_source.rfind('.') - script_source.rfind('/') - 1) +
            '/',
        0.1, 20);
  }
}
arta::PDE::~PDE() { script::close_script(); }
