#include "image/tikz.hpp"

#include <fstream>
#include <string>

#include "mesh/pslg.hpp"

bool fem::image::WriteTikz(const std::string& file_name, const PSLG& pslg) {
  std::ofstream out(file_name.c_str());
  if (out.is_open()) {
    out << "\\begin{tikzpicture}\n";
    for (auto& pt : pslg.points) {
      out << "\\draw[black,fill=black] (" << pt.x << ',' << pt.y
          << ") circle (0.005);\n";
    }
    for (auto& edg : pslg.edges) {
      out << "\\draw (" << pslg.points[edg.x].x << ',' << pslg.points[edg.x].y
          << ") -- (" << pslg.points[edg.y].x << ',' << pslg.points[edg.y].y
          << ");\n";
    }
    for (auto& pt : pslg.holes) {
      out << "\\draw[red,fill=red] (" << pt.x << ',' << pt.y
          << ") circle (0.005);\n";
    }
    out << "\\end{tikzpicture}\n";
    out.close();
  }
  return true;
}
