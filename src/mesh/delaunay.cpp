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
#include "shape.hpp"

#include "image/image2d.hpp"

#include <array>
#include <cmath>
#include <vector>
#include <unordered_set>


#include <iostream>
// std::vector<fem::shape::Triangle> fem::mesh::Delaunay(
//     const std::vector<fem::shape::Point>& points) {

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
  // return false;
}

std::vector<std::array<std::size_t, 3>> fem::mesh::Delaunay(
    std::vector<std::array<double, 2>>& points) {
  double max_x = -INFINITY, max_y = -INFINITY, cen_x = 0, cen_y = 0;
  for (std::vector<std::array<double, 2>>::const_iterator it = points.begin();
       it != points.end(); ++it) {
    max_x = std::max(max_x, (*it)[0]);
    max_y = std::max(max_y, (*it)[1]);
    cen_x += (*it)[0];
    cen_y += (*it)[1];
  }
  cen_x /= points.size();
  cen_y /= points.size();
  double dx = (max_x - cen_x);
  double dy = (max_x - cen_x);
  std::vector<std::array<std::size_t, 3>> triangles;
  points.push_back({{cen_x - (10 * dx), cen_y - (10 * dy)}});
  points.push_back({{cen_x + (10 * dx), cen_y - (10 * dy)}});
  points.push_back({{cen_x, cen_y + (10 * dy)}});
  fem::image::Image2D img(1000, 1000);
  triangles.push_back(
      {{points.size() - 3, points.size() - 2, points.size() - 1}});
  for (auto& it : points) {
    img.Point(it);
  }
  img.Triangle(triangles[0], points, 0xff99ff);
  std::size_t image_id = 0;
  img.WritePng(fem::image::GenFileName("delaunay", image_id));
  image_id++;
  for (std::size_t i = 0; i < points.size() - 3; ++i) {
    img.Fill(0x000000);
    for (auto& it : points) {
      img.Point(it);
    }
    for (auto& it : triangles) {
      img.Triangle(it, points, 0x0000ff);
    }
    img.Circle(points[i], 5, 0x00ff00);
    img.WritePng(fem::image::GenFileName("delaunay", image_id));
    image_id++;
    std::vector<std::size_t> bad_tri;
    std::unordered_set<std::size_t> polygon_points;
    for (std::size_t j = 0; j < triangles.size(); ++j) {
      if (InCircumCircle(i, triangles[j], points)) {
        bad_tri.push_back(j);
        img.Triangle(triangles[j], points, 0xff0000);

        polygon_points.insert(triangles[j][0]);
        polygon_points.insert(triangles[j][1]);
        polygon_points.insert(triangles[j][2]);
      }
    }
    std::vector<std::size_t> polygon(polygon_points.begin(), polygon_points.end());
    for(auto& it: polygon){
      std::cout << it << ' ';
    }
    std::cout << '\n';
    img.WritePng(fem::image::GenFileName("delaunay", image_id));
    image_id++;
    for (std::vector<std::size_t>::reverse_iterator it = bad_tri.rbegin();
         it != bad_tri.rend(); ++it) {
      triangles.erase(triangles.begin() + (*it));
    }
    img.Fill(0x000000);
    for (auto& it : points) {
      img.Point(it);
    }
    for (auto& it : triangles) {
      img.Triangle(it, points, 0x0000ff);
    }
    img.Circle(points[i], 5, 0x00ff00);
    img.WritePng(fem::image::GenFileName("delaunay", image_id));
    image_id++;
    for (std::size_t j = 1; j < polygon.size(); ++j) {
    // std::unordered_set<std::size_t>::iterator prev_it = polygon.begin(), it = ++polygon.begin();
    // for(;it != polygon.end(); ++it, ++prev_it){
      img.Fill(0x000000);
      for (auto& it : points) {
        img.Point(it);
      }
      for (auto& it : triangles) {
        img.Triangle(it, points, 0x0000ff);
      }
      img.Circle(points[i], 5, 0x00ff00);
      triangles.push_back({{i, polygon[j - 1], polygon[j]}});
      // triangles.push_back({{i, *prev_it, *it}});
      img.Triangle(triangles[triangles.size() - 1], points, 0xffff00);
      img.WritePng(fem::image::GenFileName("delaunay", image_id));
      image_id++;
    }
    // Triangle.push_back({{i, polygon.back(), *polygon.begin()}})
    
    triangles.push_back({{i, polygon[polygon.size() - 1], polygon[0]}});
    img.Triangle(triangles[triangles.size() - 1], points, 0xffff00);
    img.WritePng(fem::image::GenFileName("delaunay", image_id));
    image_id++;
  }
  return triangles;
}
