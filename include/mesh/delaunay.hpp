#ifndef FEM_MESH_DELAUNAY_HPP_
#define FEM_MESH_DELAUNAY_HPP_

#include "../util/util.hpp"
#include "mesh_class.hpp"
#include "pslg.hpp"

namespace fem {
Mesh DelTri(PSLG pslg);
std::vector<Pt> BSort(const std::vector<Pt>& points);
std::vector<Pt> QSorti(const std::vector<std::vector<unsigned>>& bins,
                       const std::vector<Pt>& points);
Mesh Delaun(const PSLG& pslg);
unsigned long TriLoc(const Pt& p, const Mesh& mesh);
unsigned long Edg(const unsigned long& tri, const unsigned long& adj,
                  const Mesh& mesh);
bool Swap(const Pt& p1, const Pt& p2, const Pt& p3, const Pt& p);
}  // namespace fem

#endif  // FEM_MESH_DELAUNAY_HPP_
