#include "basis.hpp"
#include "mesh/mesh.hpp"
#include "util/util.hpp"

double fem::GlobalBasis(const Mesh& mesh, const unsigned long& i, const Pt& pt,
                        const unsigned long& tri) {
  unsigned j = 0;
  if (mesh.tri[tri][0] == i) {
    j = 0;
  } else if (mesh.tri[tri][1] == i) {
    j = 1;
  } else if (mesh.tri[tri][2] == i) {
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
