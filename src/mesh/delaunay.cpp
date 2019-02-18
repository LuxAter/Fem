#include "mesh/delaunay.hpp"

#include <cmath>
#include <stack>

#include "mesh/mesh.hpp"
#include "mesh/pslg.hpp"
#include "util/util.hpp"

fem::Mesh fem::DelTri(PSLG pslg) {
  double x_min = INFINITY, x_max = -INFINITY, y_min = INFINITY,
         y_max = INFINITY;
  for (auto& pt : pslg.points) {
    x_min = std::min(x_min, pt.x);
    x_max = std::max(x_max, pt.x);
    y_min = std::min(y_min, pt.y);
    y_max = std::max(y_max, pt.y);
  }
  for (auto& pt : pslg.holes) {
    x_min = std::min(x_min, pt.x);
    x_max = std::max(x_max, pt.x);
    y_min = std::min(y_min, pt.y);
    y_max = std::max(y_max, pt.y);
  }
  double dmax = std::max(x_max - x_min, y_max - y_min);
  for (auto& pt : pslg.points) {
    pslg.points.push_back({(pt.x - x_min) / dmax, (pt.y - y_min) / dmax});
  }
  for (auto& pt : pslg.holes) {
    pslg.holes.push_back({(pt.x - x_min) / dmax, (pt.y - y_min) / dmax});
  }
  // pslg.points = BSort(pslg.points);
  // pslg = UnNormalize(pslg);
  Mesh mesh = Delaun(pslg);
  for (auto& pt : mesh.pts) {
    pt.x = pt.x * dmax + x_min;
    pt.y = pt.y * dmax + y_min;
  }
  return mesh;
}

std::vector<fem::Pt> fem::BSort(const std::vector<Pt>& points) {
  unsigned Ndiv = static_cast<unsigned>(std::pow(points.size(), 0.25));
  std::vector<std::vector<unsigned>> bins(Ndiv * Ndiv);
  double x_max = 0, y_max = 0;
  for (auto& it : points) {
    x_max = std::max(it.x, x_max);
    y_max = std::max(it.y, y_max);
  }
  for (unsigned i = 0; i < points.size(); ++i) {
    unsigned I = static_cast<unsigned>(points[i].x * Ndiv * 0.99 / x_max);
    unsigned J = static_cast<unsigned>(points[i].y * Ndiv * 0.99 / y_max);
    if (I % 2 == 0) {
      bins[I * Ndiv + J].push_back(i);
    } else {
      bins[(I + 1) * Ndiv - J - 1].push_back(i);
    }
  }
  return QSorti(bins, points);
}

std::vector<fem::Pt> fem::QSorti(const std::vector<std::vector<unsigned>>& bins,
                                 const std::vector<Pt>& points) {
  std::vector<Pt> new_points;
  for (auto& bin : bins) {
    for (auto& ind : bin) {
      new_points.push_back(points[ind]);
    }
  }
  return new_points;
}

fem::Mesh fem::Delaun(const PSLG& pslg) {
  Mesh mesh;
  mesh.pts = pslg.points;
  unsigned long num_points = mesh.pts.size();
  mesh.pts.push_back({-100, -100});
  mesh.pts.push_back({100, -100});
  mesh.pts.push_back({0, 100});
  mesh.tri.push_back({0, 0, 0});
  mesh.adj.push_back({0, 0, 0});
  mesh.tri.push_back(
      {mesh.pts.size() - 3, mesh.pts.size() - 2, mesh.pts.size() - 1});
  mesh.adj.push_back({0, 0, 0});
  for (unsigned long i = 0; i < num_points; ++i) {
    unsigned long tri = TriLoc(mesh.pts[i], mesh);
    unsigned long a = mesh.adj[tri][0];
    unsigned long b = mesh.adj[tri][1];
    unsigned long c = mesh.adj[tri][3];
    unsigned long v1 = mesh.tri[tri][0];
    unsigned long v2 = mesh.tri[tri][1];
    unsigned long v3 = mesh.tri[tri][2];
    mesh.tri[tri] = {i, v1, v2};
    mesh.adj[tri] = {mesh.tri.size() + 1, a, mesh.tri.size()};

    mesh.tri.push_back({i, v2, v3});
    mesh.adj.push_back({tri, b, mesh.tri.size()});
    mesh.tri.push_back({i, v3, v1});
    mesh.adj.push_back({mesh.tri.size() - 2, c, tri});

    std::stack<unsigned long> tri_stack;
    if (a != 0) {
      tri_stack.push(tri);
    }
    if (b != 0) {
      mesh.adj[b][Edg(b, tri, mesh)] = mesh.tri.size() - 2;
      tri_stack.push(mesh.tri.size() - 2);
    }
    if (c != 0) {
      mesh.adj[c][Edg(c, tri, mesh)] = mesh.tri.size() - 1;
      tri_stack.push(mesh.tri.size() - 1);
    }

    while (tri_stack.empty() == false) {
      unsigned long l = tri_stack.top();
      tri_stack.pop();
      unsigned long r = mesh.adj[l][1];

      unsigned long erl = Edg(r, l, mesh);
      unsigned long era = (erl + 1) % 3;
      unsigned long erb = (era + 1) % 3;
      v1 = mesh.tri[r][erl];
      v2 = mesh.tri[r][era];
      v3 = mesh.tri[r][erb];

      if (Swap(mesh.pts[v1], mesh.pts[v2], mesh.pts[v3], mesh.pts[i]) == true) {
        a = mesh.adj[r][era];
        b = mesh.adj[r][erb];
        c = mesh.adj[l][2];

        mesh.tri[l][2] = v3;
        mesh.adj[l][1] = a;
        mesh.adj[l][2] = r;

        mesh.tri[r][0] = i;
        mesh.tri[r][1] = v3;
        mesh.tri[r][2] = v1;
        mesh.adj[r][0] = l;
        mesh.adj[r][1] = b;
        mesh.adj[r][2] = c;

        if (a != 0) {
          mesh.adj[a][Edg(a, r, mesh)] = l;
          tri_stack.push(l);
        }
        if (b != 0) {
          tri_stack.push(r);
        }
        if (c != 0) {
          mesh.adj[c][Edg(c, l, mesh)] = r;
        }
      }
    }
  }
  return mesh;
}

unsigned long fem::Edg(const unsigned long& tri, const unsigned long& adj,
                       const Mesh& mesh) {
  for (unsigned long i = 0; i < 3; ++i) {
    if (mesh.adj[tri][i] == adj) {
      return i;
    }
  }
  return 0;
}

bool fem::Swap(const Pt& p1, const Pt& p2, const Pt& p3, const Pt& p) {
  Pt e13 = p1 - p3;
  Pt e23 = p2 - p3;
  Pt e1p = p1 - p;
  Pt e2p = p2 - p;
  double cos_a = e13.x * e23.x + e13.y * e23.y;
  double cos_b = e2p.x * e1p.x + e1p.y * e2p.y;
  if (cos_a > 0.0 && cos_b >= 0.0) {
    return false;
  } else if (cos_a < 0.0 && cos_b < 0.0) {
    return true;
  } else {
    double sin_a = e13.x * e23.y - e23.x * e13.y;
    double sin_b = e2p.x * e1p.y - e1p.x * e2p.y;
    if (sin_a * cos_b + sin_b * cos_a < 0.0) {
      return true;
    } else {
      return false;
    }
  }
}
