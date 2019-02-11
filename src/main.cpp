#include "fem.hpp"

#include <iostream>
#include <random>

using namespace fem;

double frand(){
  return (double)rand() / RAND_MAX;
}

fem::Mat RandMat(unsigned long size, double spar) {
  Mat mat(size, 1.0);
  std::random_device rand_dev;
  std::mt19937 gen(rand_dev());
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  for (unsigned long r = 0; r < size; ++r) {
    for (unsigned long c = r; c < size; ++c) {
      if(dist(gen) <= spar){
        double v = dist(gen);
        mat.set(r,c,v);
        mat.set(c,r,v);
      }
    }
  }
  mat += (Mat(size, 1.0) * (double)(size));
  return mat;
}

int main(int argc, char* argv[]) {
  srand(time(NULL));
  // Mat A = RandMat(1000, 0.5);
  // Vec b(1000, frand);
  // Vec c = A * b;
  // Vec d = fem::LinAlgSolve(A, c);
  // SaveToFile("A.mat", A);
  // SaveToFile("b.vec", b);
  // SaveToFile("c.vec", c);


  // Mat A = LoadMatFromFile("A.mat");
  // Vec b = LoadVecFromFile("b.vec");
  // Vec c = LoadVecFromFile("c.vec");
  // Vec d = fem::LinAlgSolve(A, c);
  // std::cout << 100.0 * (Norm(d) - Norm(b))/Norm(b) << "\n";

  PSLG pslg = LoadPslgFromFile("pslg/circles.pslg");
  image::WriteTikz("test.tikz", pslg);
  image::WritePng("test.png", 500, 500, pslg);
  image::WriteSvg("test.svg", 500, 500, pslg);
  return 0;
}
