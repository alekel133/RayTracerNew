#include <iostream>
#include <omp.h>
#include <vector>
#include "eigen-3.4.0/Eigen/Dense"
#include "CImg-3.3.1/CImg.h"
#include "Parser.h"
#include "Primitive.h"
#include "Primitive.h"
#include "Light.h"
#include "Ray.h"

using std::cout, std::endl, std::vector;
using namespace cimg_library;

int main(int argc, char *argv[]) {
  if(argc <= 2) {
    cout << "Invalid number of arguments." << endl;
    cout << "Usage: RayTracer <inFile.sdf> <outFile.format>." << endl;
    exit(EXIT_FAILURE);
  }

  char *infile = argv[1];
  char *outfile = argv[2];
  ParseData parseData;

  parseFile(infile, parseData);

  Eigen::initParallel();
  omp_set_num_threads(omp_get_max_threads());
  Eigen::setNbThreads(omp_get_max_threads());

  // Get Scene Data From Parser
  int Height = parseData.height;
  int Width = parseData.width;
  int fov = parseData.fov;
  double ambient = parseData.ambient;
  Camera *camera = parseData.camera;
  vector<Primitive*> Primitives = parseData.primitives;
  vector<Light*> Lights = parseData.lights;
  vector<AABB> boxes;

  // OutPut Buffer
  CImg<unsigned char> outImage(Width, Height, 1, 3, 0);
  
  boxes.reserve(Primitives.size());
  vector<Primitive*> tmp;
  for(int i = 0; i < Primitives.size(); ++i) {
    tmp.push_back(Primitives[i]);
    boxes.push_back(AABB());
    boxes.back().genBox(tmp);
    tmp.clear();
  }

  //GenerateRays
  Ray*** rays = new Ray**[Width];
  for(int i = 0; i < Width; ++i) rays[i] = new Ray*[Height];
  #pragma omp parallel 
  {
    #pragma omp for
    for(int i = 0; i < Width; ++i) {
      for(int j = 0; j < Height; ++j) {
        Ray *ray = new Ray;
        ray = generateRay(*camera,Width, Height, i, j); rays[i][j] = ray;
      }
    }
  }


  Hit* hits = new Hit[Width*Height];
  #pragma omp parallel for
  {
    for(int i = 0; i < Width*Height; ++i) {
      hits[i] = Hit();
      hits[i].material.diffuse = new int[3];
      hits[i].material.diffuse[0] = 0;
      hits[i].material.diffuse[1] = 0;
      hits[i].material.diffuse[2] = 0;
      hits[i].material.specular = new int[3];
      hits[i].material.specular[0] = 0;
      hits[i].material.specular[1] = 0;
      hits[i].material.specular[2] = 0;
    }
  }

  double minT;
  Hit hit;
  #pragma omp parallel private(hit)
  {
    #pragma omp for
    for(int k = 0; k < Width*Height; ++k) {
      hits[k].i = k/Width;
      hits[k].j = k%Width;
      minT = DBL_MAX;
      for(int l = 0; l < boxes.size(); ++l) {
        if(boxes[l].isHit(rays[hits[k].i][hits[k].j], hit)) {
          if(hit.t < minT){
            hits[k].t = hit.t;
            hits[k].material.diffuse[0] = hit.material.diffuse[0];
            hits[k].material.diffuse[1] = hit.material.diffuse[1];
            hits[k].material.diffuse[2] = hit.material.diffuse[2];
            hits[k].material.specular[0] = hit.material.specular[0];
            hits[k].material.specular[1] = hit.material.specular[1];
            hits[k].material.specular[2] = hit.material.specular[2];
            hits[k].material.shine = hit.material.shine;
          }
        }
      }
    }
  }
  

  #pragma omp parallel for
  for(int m = 0; m < Width*Height; ++m) {
    if(hits[m].material.diffuse[0] != 0 || hits[m].material.diffuse[1] != 0 || hits[m].material.diffuse[2] != 0){
      outImage(hits[m].i,hits[m].j,0,0) = hits[m].material.diffuse[0];
      outImage(hits[m].i,hits[m].j,0,1) = hits[m].material.diffuse[1];
      outImage(hits[m].i,hits[m].j,0,2) = hits[m].material.diffuse[2];
    }
  }

  CImgDisplay display(outImage);
  while(!display.is_closed()){
    display.wait();
  }

  outImage.save_png(outfile);

  free(rays);

  #pragma omp parallel for
  {
    for(int i = 0; i < Width*Height; ++i) {
      free(hits[i].material.diffuse);
      free(hits[i].material.specular);
    }
  }
  free(hits);

  return 0;
}
