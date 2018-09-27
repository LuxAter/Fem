#include <string>
#include <iostream>

#include "image/image.hpp"


int main(int argc, char *argv[])
{
  fem::image::Image img(500, 500);
  img.Fill(img.GetColor(0.1, 1.0, 0.8));
  img.WritePng("test.png");
  return 0;
}
