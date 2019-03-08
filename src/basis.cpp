#include "basis.hpp"

#include "geometry.hpp"
#include "mesh.hpp"

double fem::basis::local_basis(const mesh::Mesh& mesh, const Pair<double>& pt,
                               const unsigned& tri, const unsigned& vert) {
  double x1 = mesh.pts[mesh.tri[tri][vert]].x;
  double y1 = mesh.pts[mesh.tri[tri][vert]].y;
  double x2 = mesh.pts[mesh.tri[tri][(vert + 1) % 3]].x;
  double y2 = mesh.pts[mesh.tri[tri][(vert + 1) % 3]].y;
  double x3 = mesh.pts[mesh.tri[tri][(vert + 2) % 3]].x;
  double y3 = mesh.pts[mesh.tri[tri][(vert + 2) % 3]].y;
  return ((y2 - y3) * (pt.x - x3) + (x3 - x2) * (pt.y - y3)) /
         ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
}

double fem::basis::local_basis_deriv(const mesh::Mesh& mesh,
                                     const Pair<double>& pt,
                                     const unsigned& tri,
                                     const unsigned& vert) {
  double x1 = mesh.pts[mesh.tri[tri][vert]].x;
  double y1 = mesh.pts[mesh.tri[tri][vert]].y;
  double x2 = mesh.pts[mesh.tri[tri][(vert + 1) % 3]].x;
  double y2 = mesh.pts[mesh.tri[tri][(vert + 1) % 3]].y;
  double x3 = mesh.pts[mesh.tri[tri][(vert + 2) % 3]].x;
  double y3 = mesh.pts[mesh.tri[tri][(vert + 2) % 3]].y;
  return ((y2 - y3) * pt.x + (x3 - x2) * pt.y) /
         ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
}

double fem::basis::global_basis(const mesh::Mesh& mesh, const Pair<double>& pt,
                                const unsigned& tri, const unsigned& vert) {
  unsigned j = 0;
  if (mesh.tri[tri][0] == vert) {
    j = 0;
  } else if (mesh.tri[tri][1] == vert) {
    j = 1;
  } else if (mesh.tri[tri][2] == vert) {
    j = 2;
  } else {
    return 0.0;
  }
  double x1 = mesh.pts[mesh.tri[tri][j]].x;
  double y1 = mesh.pts[mesh.tri[tri][j]].y;
  double x2 = mesh.pts[mesh.tri[tri][(j + 1) % 3]].x;
  double y2 = mesh.pts[mesh.tri[tri][(j + 1) % 3]].y;
  double x3 = mesh.pts[mesh.tri[tri][(j + 2) % 3]].x;
  double y3 = mesh.pts[mesh.tri[tri][(j + 2) % 3]].y;
  double x = pt.x;
  double y = pt.y;
  return ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) /
         ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
}
