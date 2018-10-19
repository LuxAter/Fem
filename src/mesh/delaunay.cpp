/* Copyright (C)
 * 2018 - Arden
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * XXX: Don't try to be smart, follow the given algorithm, and hope you do it
 * right. Others are much much smarter than you!
 *
 */

#include "mesh/delaunay.hpp"
#include "async.hpp"
#include "shape.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <unordered_set>
#include <vector>

#include "mesh/mesh.hpp"

#include "image/image.hpp"
#include "image/svg.hpp"

#include <iostream>

bool fem::mesh::Intersect(const std::array<double, 2>& a,
                          const std::array<double, 2>& b,
                          const std::array<double, 2>& c,
                          const std::array<double, 2>& d) {
  double t = ((a[0] - c[0]) * (c[1] - d[1]) - (a[1] - c[1]) * (c[0] - d[0])) /
             ((a[0] - b[0]) * (c[1] - d[1]) - (a[1] - b[1]) * (c[0] - d[0]));
  double u = -((a[0] - b[0]) * (a[1] - c[1]) - (a[1] - b[1]) * (a[0] - c[0])) /
             ((a[0] - b[0]) * (c[1] - d[1]) - (a[1] - b[1]) * (c[0] - d[0]));
  return t - 1e-4 > 0.0 && t + 1e-4 < 1.0 && u - 1e-4 > 0.0 &&
         u + 1e-4 < 1.0;  // XXX Dislike +- epsilon
}

bool fem::mesh::SharedEdge(const std::size_t& a, const std::size_t& b,
                           const std::array<std::size_t, 3>& tri) {
  return (a == tri[0] && b == tri[1]) || (a == tri[1] && b == tri[2]) ||
         (a == tri[2] && b == tri[0]) || (b == tri[0] && a == tri[1]) ||
         (b == tri[1] && a == tri[2]) || (b == tri[2] && a == tri[0]);
}
bool fem::mesh::RotationalLess(
    const std::size_t& center, const std::size_t& a, const std::size_t& b,
    const std::vector<std::array<double, 2>>& points) {
  std::array<double, 2> pa = points[a];
  std::array<double, 2> pb = points[b];
  std::array<double, 2> pc = points[center];
  if (pa[0] - pc[0] >= 0 && pb[0] - pc[0] < 0) return true;
  if (pa[0] - pc[0] < 0 && pb[0] - pc[0] >= 0) return false;
  if (pa[0] - pc[0] == 0 && pb[0] - pc[0] == 0) {
    if (pa[1] - pc[1] >= 0 || pb[1] - pc[1] >= 0) return pa[1] > pb[1];
    return pb[1] > pa[1];
  }
  int det =
      (pa[0] - pc[0]) * (pb[1] - pc[1]) - (pb[0] - pc[0]) * (pa[1] - pc[1]);
  if (det < 0) return true;
  if (det > 0) return false;
  int d1 =
      (pa[0] - pc[0]) * (pa[0] - pc[0]) + (pa[1] - pc[1]) * (pa[1] - pc[1]);
  int d2 =
      (pb[0] - pc[0]) * (pb[0] - pc[0]) + (pb[1] - pc[1]) * (pb[1] - pc[1]);
  return d1 > d2;
}
bool fem::mesh::InCircumCircle(
    const std::size_t& pt, const std::array<std::size_t, 3>& tri,
    const std::vector<std::array<double, 2>>& points) {
  double x13 = points[tri[0]][0] - points[tri[2]][0];
  double y13 = points[tri[0]][1] - points[tri[2]][1];
  double x23 = points[tri[1]][0] - points[tri[2]][0];
  double y23 = points[tri[1]][1] - points[tri[2]][1];
  double x1p = points[tri[0]][0] - points[pt][0];
  double y1p = points[tri[0]][1] - points[pt][1];
  double x2p = points[tri[1]][0] - points[pt][0];
  double y2p = points[tri[1]][1] - points[pt][1];
  return (x13 * x23 + y13 * y23) * (x2p * y1p - x1p * y2p) <
         (x23 * y13 - x13 * y23) * (x2p * x1p + y1p * y2p);
}
bool fem::mesh::BadInCircumCircle(
    const std::size_t& pt, const std::array<std::size_t, 3>& tri,
    const std::vector<std::array<double, 2>>& points) {
  return false;
}
// std::vector<std::array<std::size_t, 3>> fem::mesh::Delaunay(
//     std::vector<std::array<double, 2>>& points) {
//   double max_x = -INFINITY, max_y = -INFINITY, cen_x = 0, cen_y = 0;
//   for (std::vector<std::array<double, 2>>::const_iterator it =
//   points.begin();
//        it != points.end(); ++it) {
//     max_x = std::max(max_x, (*it)[0]);
//     max_y = std::max(max_y, (*it)[1]);
//     cen_x += (*it)[0];
//     cen_y += (*it)[1];
//   }
//   cen_x /= points.size();
//   cen_y /= points.size();
//   double dx = (max_x - cen_x);
//   double dy = (max_x - cen_x);
//   std::vector<std::array<std::size_t, 3>> triangles;
//   points.push_back({{cen_x - (10 * dx), cen_y - (10 * dy)}});
//   points.push_back({{cen_x + (10 * dx), cen_y - (10 * dy)}});
//   points.push_back({{cen_x, cen_y + (10 * dy)}});
//   triangles.push_back(
//       {{points.size() - 3, points.size() - 2, points.size() - 1}});
//   for (std::size_t i = 0; i < points.size() - 3; ++i) {
//     std::vector<std::size_t> bad_tri;
//     std::vector<std::array<std::size_t, 2>> polygon;
//     for (std::size_t j = 0; j < triangles.size(); ++j) {
//       if (InCircumCircle(i, triangles[j], points)) {
//         bad_tri.push_back(j);
//       }
//     }
//     for (std::size_t j = 0; j < bad_tri.size(); j++) {
//       bool a = false, b = false, c = false;
//       for (std::size_t k = 0; k < bad_tri.size(); k++) {
//         if (j == k) continue;
//         a = a || SharedEdge(triangles[bad_tri[j]][0],
//         triangles[bad_tri[j]][1],
//                             triangles[bad_tri[k]]);
//         b = b || SharedEdge(triangles[bad_tri[j]][1],
//         triangles[bad_tri[j]][2],
//                             triangles[bad_tri[k]]);
// c = c || SharedEdge(triangles[bad_tri[j]][2],
//         triangles[bad_tri[j]][0],
//                             triangles[bad_tri[k]]);
//       }
//       if (!a)
//         polygon.push_back(
//             {{triangles[bad_tri[j]][0], triangles[bad_tri[j]][1]}});
//       if (!b)
//         polygon.push_back(
//             {{triangles[bad_tri[j]][1], triangles[bad_tri[j]][2]}});
//       if (!c)
//         polygon.push_back(
//             {{triangles[bad_tri[j]][2], triangles[bad_tri[j]][0]}});
//     }
//     for (std::vector<std::size_t>::reverse_iterator it = bad_tri.rbegin();
//          it != bad_tri.rend(); ++it) {
//       triangles.erase(triangles.begin() + (*it));
//     }
//     for (std::size_t j = 0; j < polygon.size(); ++j) {
//       triangles.push_back({{i, polygon[j][0], polygon[j][1]}});
//     }
//   }
//   std::vector<std::size_t> outer_tris;
//   for (std::size_t i = 0; i < triangles.size(); ++i) {
//     if (triangles[i][0] >= points.size() - 3 ||
//         triangles[i][1] >= points.size() - 3 ||
//         triangles[i][2] >= points.size() - 3) {
//       outer_tris.push_back(i);
//     }
//   }
//   for (std::vector<std::size_t>::reverse_iterator it = outer_tris.rbegin();
//        it != outer_tris.rend(); ++it) {
//     triangles.erase(triangles.begin() + (*it));
//   }
//   return triangles;
// }

void fem::mesh::InsertPoint(Mesh& mesh, const std::array<double, 2>& vertex) {
  std::vector<std::size_t> bad_tri;
  std::vector<std::array<std::size_t, 2>> polygon;
  for (std::size_t i = 0; i < mesh.triangles.size(); ++i) {
    if (mesh.CircumCircle(i, vertex)) bad_tri.push_back(i);
  }
  for (std::size_t i = 0; i < bad_tri.size(); ++i) {
    bool a = false, b = false, c = false;
    for (std::size_t j = 0; j < bad_tri.size(); ++j) {
      if (j == i) continue;
      a = a ||
          SharedEdge(mesh.triangles[bad_tri[i]][0],
                     mesh.triangles[bad_tri[i]][1], mesh.triangles[bad_tri[j]]);
      b = b ||
          SharedEdge(mesh.triangles[bad_tri[i]][1],
                     mesh.triangles[bad_tri[i]][2], mesh.triangles[bad_tri[j]]);
      c = c ||
          SharedEdge(mesh.triangles[bad_tri[i]][2],
                     mesh.triangles[bad_tri[i]][0], mesh.triangles[bad_tri[j]]);
    }
    if (!a)
      polygon.push_back(
          {{mesh.triangles[bad_tri[i]][0], mesh.triangles[bad_tri[i]][1]}});
    if (!b)
      polygon.push_back(
          {{mesh.triangles[bad_tri[i]][1], mesh.triangles[bad_tri[i]][2]}});
    if (!c)
      polygon.push_back(
          {{mesh.triangles[bad_tri[i]][2], mesh.triangles[bad_tri[i]][0]}});
  }
  for (std::vector<std::size_t>::reverse_iterator it = bad_tri.rbegin();
       it != bad_tri.rend(); ++it) {
    mesh.triangles.erase(mesh.triangles.begin() + (*it));
  }
  for (std::size_t i = 0; i < polygon.size(); ++i) {
    mesh.triangles.push_back(
        {{mesh.points.size(), polygon[i][0], polygon[i][1]}});
  }
  mesh.points.push_back(vertex);
}

bool fem::mesh::EdgeCheck(
    const std::vector<std::array<std::size_t, 2>>& bad_edges,
    const std::vector<std::array<std::size_t, 2>>& good_edges,
    const std::array<std::size_t, 2>& edge) {
  bool edge_checked = false;
  for (std::size_t j = 0; j < bad_edges.size() || j < good_edges.size(); j++) {
    if (j < bad_edges.size() &&
        ((edge[0] == bad_edges[j][0] && edge[1] == bad_edges[j][1]) ||
         (edge[0] == bad_edges[j][1] && edge[1] == bad_edges[j][0]))) {
      edge_checked = true;
      break;
    }
    if (j < good_edges.size() &&
        ((edge[0] == good_edges[j][0] && edge[1] == good_edges[j][1]) ||
         (edge[0] == good_edges[j][1] && edge[1] == good_edges[j][0]))) {
      edge_checked = true;
      break;
    }
  }
  return edge_checked;
}

void fem::mesh::ApplyEdge(Mesh& mesh, const std::array<std::size_t, 2>& edge) {
  std::vector<std::array<std::size_t, 2>> bad_edges;
  std::vector<std::array<std::size_t, 2>> good_edges;
  for (std::size_t i = 0; i < mesh.triangles.size(); ++i) {
    if (!EdgeCheck(bad_edges, good_edges,
                   {{mesh.triangles[i][0], mesh.triangles[i][1]}}) &&
        Intersect(mesh.points[edge[0] + 3], mesh.points[edge[1] + 3],
                  mesh.points[mesh.triangles[i][0]],
                  mesh.points[mesh.triangles[i][1]])) {
      bad_edges.push_back({{mesh.triangles[i][0], mesh.triangles[i][1]}});
    } else {
      good_edges.push_back({{mesh.triangles[i][0], mesh.triangles[i][1]}});
    }
    if (!EdgeCheck(bad_edges, good_edges,
                   {{mesh.triangles[i][1], mesh.triangles[i][2]}}) &&
        Intersect(mesh.points[edge[0] + 3], mesh.points[edge[1] + 3],
                  mesh.points[mesh.triangles[i][1]],
                  mesh.points[mesh.triangles[i][2]])) {
      bad_edges.push_back({{mesh.triangles[i][1], mesh.triangles[i][2]}});
    } else {
      good_edges.push_back({{mesh.triangles[i][1], mesh.triangles[i][2]}});
    }
    if (!EdgeCheck(bad_edges, good_edges,
                   {{mesh.triangles[i][2], mesh.triangles[i][0]}}) &&
        Intersect(mesh.points[edge[0] + 3], mesh.points[edge[1] + 3],
                  mesh.points[mesh.triangles[i][2]],
                  mesh.points[mesh.triangles[i][0]])) {
      bad_edges.push_back({{mesh.triangles[i][2], mesh.triangles[i][0]}});
    } else {
      good_edges.push_back({{mesh.triangles[i][2], mesh.triangles[i][0]}});
    }
  }
  good_edges.clear();
  for (std::size_t i = 0; i < bad_edges.size(); ++i) {
    std::size_t tri_a = 0, tri_b = 0;
    for (std::size_t j = 0; j < mesh.triangles.size(); ++j) {
      if (Contains(mesh.triangles[j], bad_edges[i])) {
        if (tri_a == 0)
          tri_a = j;
        else if (tri_b == 0)
          tri_b = j;
      }
    }
    std::array<std::size_t, 2> good_vertex = {{0, 0}};
    if (!Contains(bad_edges[i], mesh.triangles[tri_a][0]))
      good_vertex[0] = mesh.triangles[tri_a][0];
    else if (!Contains(bad_edges[i], mesh.triangles[tri_a][1]))
      good_vertex[0] = mesh.triangles[tri_a][1];
    else if (!Contains(bad_edges[i], mesh.triangles[tri_a][2]))
      good_vertex[0] = mesh.triangles[tri_a][2];
    if (!Contains(bad_edges[i], mesh.triangles[tri_b][0]))
      good_vertex[1] = mesh.triangles[tri_b][0];
    else if (!Contains(bad_edges[i], mesh.triangles[tri_b][1]))
      good_vertex[1] = mesh.triangles[tri_b][1];
    else if (!Contains(bad_edges[i], mesh.triangles[tri_b][2]))
      good_vertex[1] = mesh.triangles[tri_b][2];
    mesh.triangles[tri_a] = {{good_vertex[0], bad_edges[i][0], good_vertex[1]}};
    mesh.triangles[tri_b] = {{good_vertex[1], bad_edges[i][1], good_vertex[0]}};
    if(Intersect(mesh.points[edge[0] + 3], mesh.points[edge[1] + 3], mesh.points[good_vertex[0]], mesh.points[good_vertex[1]])){
      bad_edges.push_back(good_vertex);
    }
    // TODO If new edge is bad, then add it to end of list.
  }
}

fem::mesh::Mesh fem::mesh::Delaunay(const Pslg& pslg) {
  Mesh mesh;
  std::array<double, 2> maximum = {{-INFINITY, -INFINITY}};
  std::array<double, 2> minimum = {{INFINITY, INFINITY}};
  std::vector<std::array<double, 2>> points = pslg.points;
  for (auto& it : points) {
    maximum[0] = std::max(maximum[0], it[0]);
    maximum[1] = std::max(maximum[1], it[1]);
    minimum[0] = std::min(minimum[0], it[0]);
    minimum[1] = std::min(minimum[1], it[1]);
  }
  std::array<double, 2> center = {
      {(maximum[0] + minimum[0]) / 2.0, (maximum[1] + minimum[1]) / 2.0}};
  double dx = (maximum[0] - center[0]);
  double dy = (maximum[1] - center[1]);
  mesh.points.push_back({{center[0] - (3 * dx), center[1] - (3 * dy)}});
  mesh.points.push_back({{center[0] + (3 * dx), center[1] - (3 * dy)}});
  mesh.points.push_back({{center[0], center[1] + (3 * dy)}});
  mesh.triangles.push_back({{0, 1, 2}});

  image::Svg svg(5000, 5000);
  svg.Fill("white");
  svg.Pslg(pslg);
  std::size_t i = 0;
  svg.WriteSvgWait(image::GenFileName("ex", "svg", i));
  i++;

  for (auto& pt : points) {
    InsertPoint(mesh, pt);
  }
  svg.Fill("white");
  svg.Pslg(pslg);
  // svg.Mesh(mesh);
  svg.Mesh(mesh, "blue", "blue", 1, 2);
  svg.WriteSvgWait(image::GenFileName("ex", "svg", i));
  i++;
  for (auto& eg : pslg.edges) {
    std::cout << std::string('=', 50) << "\n";
    ApplyEdge(mesh, eg);
    // TODO RESTORE DELAUNAY!
    svg.Fill("white");
    svg.Pslg(pslg);
    svg.Mesh(mesh, "blue", "blue", 3, 2);
    svg.Line(pslg.points[eg[0]][0], pslg.points[eg[0]][1],
             pslg.points[eg[1]][0], pslg.points[eg[1]][1], "red", 4);
    svg.WriteSvgWait(image::GenFileName("ex", "svg", i));
    i++;
    int a;
    // std::cin >> a;
    // break;
  }
  // mesh.triangles.erase(
  //     std::remove_if(mesh.triangles.begin(), mesh.triangles.end(),
  //                    [mesh = mesh](const std::array<std::size_t, 3>& tri) {
  //                      return Contains(tri, 0) || Contains(tri, 1) ||
  //                             Contains(tri, 2);
  //                    }),
  //     mesh.triangles.end());
  return mesh;
}
