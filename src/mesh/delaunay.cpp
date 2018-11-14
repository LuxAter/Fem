#include "mesh/delaunay.hpp"

#include <array>
#include <cmath>
#include <numeric>
#include <stack>
#include <vector>

#include "mesh/mesh.hpp"

#include "image/figure.hpp"
#include "image/svg.hpp"

fem::mesh::Mesh fem::del::DelTri(std::vector<std::array<double, 2>> points) {
  std::array<double, 4> bounds = {{INFINITY, INFINITY, -INFINITY, -INFINITY}};
  for (auto& pt : points) {
    bounds[0] = std::min(bounds[0], pt[0]);
    bounds[1] = std::min(bounds[1], pt[1]);
    bounds[2] = std::max(bounds[2], pt[0]);
    bounds[3] = std::max(bounds[3], pt[1]);
  }
  double dmax = std::max(bounds[2] - bounds[0], bounds[3] - bounds[1]);
  for (auto& pt : points) {
    pt[0] = (pt[0] - bounds[0]) / dmax;
    pt[1] = (pt[1] - bounds[1]) / dmax;
  }
  points = BSort(points);
  std::pair<std::vector<std::array<uint64_t, 3>>,
            std::vector<std::array<uint64_t, 3>>>
      delaunay = Delaun(points);
  for (auto& pt : points) {
    pt[0] = pt[0] * dmax + bounds[0];
    pt[1] = pt[1] * dmax + bounds[1];
  }
  fem::mesh::Mesh delaunay_mesh(points, delaunay.first);
  return delaunay_mesh;
}

std::vector<std::array<double, 2>> fem::del::BSort(
    const std::vector<std::array<double, 2>>& points) {
  uint32_t Ndiv = static_cast<uint32_t>(std::pow(points.size(), 0.25));
  std::vector<std::vector<uint32_t>> bins(Ndiv * Ndiv);
  double x_max = 0, y_max = 0;
  for (auto& it : points) {
    x_max = std::max(it[0], x_max);
    y_max = std::max(it[1], y_max);
  }
  for (uint32_t i = 0; i < points.size(); ++i) {
    uint32_t I = static_cast<uint32_t>(points[i][0] * Ndiv * 0.99 / x_max);
    uint32_t J = static_cast<uint32_t>(points[i][1] * Ndiv * 0.99 / y_max);
    if (I % 2 == 0) {
      bins[I * Ndiv + J].push_back(i);
    } else {
      bins[(I + 1) * Ndiv - J - 1].push_back(i);
    }
  }
  return QSorti(bins, points);
}

std::vector<std::array<double, 2>> fem::del::QSorti(
    const std::vector<std::vector<uint32_t>>& bins,
    const std::vector<std::array<double, 2>>& points) {
  std::vector<std::array<double, 2>> new_points;
  for (auto& bin : bins) {
    for (auto& ind : bin) {
      new_points.push_back(points[ind]);
    }
  }
  return new_points;
}

std::pair<std::vector<std::array<uint64_t, 3>>,
          std::vector<std::array<uint64_t, 3>>>
fem::del::Delaun(std::vector<std::array<double, 2>> points) {
  uint64_t num_points = points.size();
  // TODO change back to large numbers
  points.push_back({{-3, -3}});
  points.push_back({{3, -3}});
  points.push_back({{0, 3}});
  std::vector<std::array<uint64_t, 3>> vrt, adj;
  vrt.push_back({{0, 0, 0}});
  adj.push_back({{0, 0, 0}});
  vrt.push_back({{points.size() - 3, points.size() - 2, points.size() - 1}});
  adj.push_back({{0, 0, 0}});
  fem::image::Svg img(500, 500);
  for (uint64_t i = 0; i < points.size() - 3; ++i) {
    uint64_t tri = TriLoc(points[i], points, vrt, adj);

    uint64_t a = adj[tri][0];
    uint64_t b = adj[tri][1];
    uint64_t c = adj[tri][2];
    uint64_t v1 = vrt[tri][0];
    uint64_t v2 = vrt[tri][1];
    uint64_t v3 = vrt[tri][2];

    vrt[tri] = {{i, v1, v2}};
    adj[tri] = {{vrt.size() + 1, a, vrt.size()}};

    vrt.push_back({{i, v2, v3}});
    adj.push_back({{tri, b, vrt.size()}});
    vrt.push_back({{i, v3, v1}});
    adj.push_back({{vrt.size() - 2, c, tri}});

    std::stack<uint64_t> tri_stack;

    if (a != 0) {
      tri_stack.push(tri);
    }
    if (b != 0) {
      adj[b][Edg(b, tri, adj)] = vrt.size() - 2;
      tri_stack.push(vrt.size() - 2);
    }
    if (c != 0) {
      adj[c][Edg(c, tri, adj)] = vrt.size() - 1;
      tri_stack.push(vrt.size() - 1);
    }

    while (tri_stack.empty() == false) {
      uint64_t l = tri_stack.top();
      tri_stack.pop();
      uint64_t r = adj[l][1];

      uint64_t erl = Edg(r, l, adj);
      uint64_t era = (erl+1) % 3;
      uint64_t erb = (era+1) % 3;
      v1 = vrt[r][erl];
      v2 = vrt[r][era];
      v3 = vrt[r][erb];

      if (Swap(points[v1], points[v2], points[v3], points[i]) == true) {
        a = adj[r][era];
        b = adj[r][erb];
        c = adj[l][2];

        vrt[l][2] = v3;
        adj[l][1] = a;
        adj[l][2] = r;

        vrt[r][0] = i;
        vrt[r][1] = v3;
        vrt[r][2] = v1;
        adj[r][0] = l;
        adj[r][1] = b;
        adj[r][2] = c;

        if (a != 0) {
          adj[a][Edg(a, r, adj)] = l;
          tri_stack.push(l);
        }
        if (b != 0) {
          tri_stack.push(r);
        }
        if (c != 0) {
          adj[c][Edg(c, l, adj)] = r;
        }
      }
    }
  }
  uint64_t tri;
  for (tri = 0; tri < vrt.size(); ++tri) {
    if (vrt[tri][0] >= num_points || vrt[tri][1] >= num_points ||
        vrt[tri][2] >= num_points) {
      for (uint64_t i = 0; i < 3; ++i) {
        uint64_t a = adj[tri][i];
        if (a != 0) {
          adj[a][Edg(a, tri, adj)] = 0;
        }
      }
      break;
    }
  }

  uint64_t tri_start = tri + 1;
  uint64_t tri_stop = vrt.size();
  uint64_t num_tri = tri - 1;

  for (tri = tri_start; tri < tri_stop; ++tri) {
    if (vrt[tri][0] >= num_points || vrt[tri][1] >= num_points ||
        vrt[tri][2] >= num_points) {
      for (uint64_t i = 0; i < 3; ++i) {
        uint64_t a = adj[tri][i];
        if (a != 0) {
          adj[a][Edg(a, tri, adj)] = 0;
        }
      }
    } else {
      num_tri++;
      for (uint64_t i = 0; i < 3; ++i) {
        uint64_t a = adj[tri][i];
        adj[num_tri][i] = a;
        vrt[num_tri][i] = vrt[tri][i];
        if (a != 0) {
          adj[a][Edg(a, tri, adj)] = num_tri;
        }
      }
    }
  }
  vrt.resize(num_tri+1);
  vrt.resize(num_tri+1);
  return std::make_pair(vrt, adj);
}

uint64_t fem::del::Edg(const uint64_t& tri, const uint64_t& adj,
                       const std::vector<std::array<uint64_t, 3>>& edg) {
  for (uint64_t i = 0; i < 3; ++i) {
    if (edg[tri][i] == adj) {
      return i;
    }
  }
  return 0;
}

uint64_t fem::del::TriLoc(
    const std::array<double, 2>& p,
    const std::vector<std::array<double, 2>>& points,
    const std::vector<std::array<uint64_t, 3>>& vertices,
    const std::vector<std::array<uint64_t, 3>>& adjacency) {
  uint64_t t = vertices.size() - 1;
  bool searching = true;
  while (searching == true) {
    bool within = true;
    for (uint64_t i = 0; i < 3; ++i) {
      uint64_t v1 = vertices[t][i];
      uint64_t v2 = vertices[t][(i + 1) % 3];
      if ((points[v1][1] - p[1]) * (points[v2][0] - p[0]) >
          (points[v1][0] - p[0]) * (points[v2][1] - p[1])) {
        t = adjacency[t][i];
        within = false;
      }
    }
    if (within == true) {
      searching = false;
    }
  }
  return t;
}

bool fem::del::Swap(const std::array<double, 2>& p1,
                    const std::array<double, 2>& p2,
                    const std::array<double, 2>& p3,
                    const std::array<double, 2>& p) {
  std::array<double, 2> e13 = p1 - p3;
  std::array<double, 2> e23 = p2 - p3;
  std::array<double, 2> e1p = p1 - p;
  std::array<double, 2> e2p = p2 - p;
  double cos_a = e13[0] * e23[0] + e13[1] * e23[1];
  double cos_b = e2p[0] * e1p[0] + e1p[1] * e2p[1];
  if (cos_a >= 0.0 && cos_b >= 0.0) {
    return false;
  } else if (cos_a < 0.0 && cos_b < 0.0) {
    return true;
  } else {
    double sin_a = e13[0] * e23[1] - e23[0] * e13[1];
    double sin_b = e2p[0] * e1p[1] - e1p[0] * e2p[1];
    if (sin_a * cos_b + sin_b * cos_a < 0.0) {
      return true;
    } else {
      return false;
    }
  }
}
