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

void addColors(int *a, int *b) {
  a[0] += b[0];
  a[1] += b[1];
  a[2] += b[2];
}

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

  Hit* hits = new Hit[Width*Height];
  Ray*** rays = new Ray**[Width];
  #pragma omp parallel for
  for(int i = 0; i < Width; ++i) rays[i] = new Ray*[Height];
  int ***colors = new int**[Width];
  #pragma omp parallel for
  for(int i = 0; i < Width; ++i) {
    colors[i] = new int*[Height];
    for(int j = 0; j < Height; ++j) {
      colors[i][j] = new int[3];
      colors[i][j][0] = 0;
      colors[i][j][1] = 0;
      colors[i][j][2] = 0;
    }
  }
  double minT;
  Hit hit;
  #pragma omp parallel private(hit)
  {
    #pragma omp for
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
      hits[i].vis = 0;
    }
 

  //GenerateRays
    #pragma omp for
    for(int i = 0; i < Width; ++i) {
      for(int j = 0; j < Height; ++j) {
        Ray *ray = new Ray;
        ray = generateRay(*camera,Width, Height, i, j); rays[i][j] = ray;
      }
    }

  //Intersect Objects, and get normal and location data
    #pragma omp for
    for(int i = 0; i < Width*Height; ++i) {
      hits[i].i = i%Width;
      hits[i].j = i/Width;
      minT = DBL_MAX;
      for(int j = 0; j < boxes.size(); ++j) {
        if(boxes[j].isHit(rays[hits[i].i][hits[i].j], hit)) {
          if(hit.t < minT){
            hits[i].t = hit.t;
            hits[i].normal = hit.normal;
            hits[i].material.diffuse[0] = hit.material.diffuse[0];
            hits[i].material.diffuse[1] = hit.material.diffuse[1];
            hits[i].material.diffuse[2] = hit.material.diffuse[2];
            hits[i].material.specular[0] = hit.material.specular[0];
            hits[i].material.specular[1] = hit.material.specular[1];
            hits[i].material.specular[2] = hit.material.specular[2];
            hits[i].material.shine = hit.material.shine;
            hits[i].vis = 1;
          }
        }
      }
    }

    #pragma omp for
    for(int i = 0; i < Width*Height; ++i) {
      for(int j = 0; j < Lights.size(); ++j){ 
        Ray* shadowRay = new Ray;
        shadowRay->origin = rays[hits[i].i][hits[i].j]->eval(hits[i].t) + 0.0006 * hits[i].normal;
        shadowRay->direction = Lights[j]->getDirection(shadowRay->origin);
        shadowRay->tNear = (Lights[j]->getPoint() - shadowRay->origin).norm();
        for(int k = 0; k < Primitives.size(); ++k) {
          if(Primitives[k]->isHit(transformRay(shadowRay, Primitives[k]->getLocal()), hit)) {
            hits[i].vis = 0;
            break;
          }
        }
      }
    }

    #pragma omp for
    for(int i = 0; i < Width*Height; ++i) {
      if(hits[i].vis){
        for(int j = 0; j < Lights.size(); ++j)  {
          addColors(colors[hits[i].i][hits[i].j],Lights[j]->illuminate(transformRay(rays[hits[i].i][hits[i].j], Lights[j]->getLocal()), hits[i]));
        }
        for(int j = 0; j < 3; ++j) {
          colors[hits[i].i][hits[i].j][j] += ambient * hits[i].material.diffuse[j];
          if(colors[hits[i].i][hits[i].j][j] > 255) colors[hits[i].i][hits[i].j][j] = 255;
          if(colors[hits[i].i][hits[i].j][j] < 0) colors[hits[i].i][hits[i].j][j] = 0;
        } 
      }
    }  

    #pragma omp for
    for(int i = 0; i < Width*Height; ++i) {
      if(hits[i].vis){
        outImage(hits[i].i,hits[i].j,0,0) = colors[hits[i].i][hits[i].j][0];
        outImage(hits[i].i,hits[i].j,0,1) = colors[hits[i].i][hits[i].j][1];
        outImage(hits[i].i,hits[i].j,0,2) = colors[hits[i].i][hits[i].j][2];
      }
    }
  }

  CImgDisplay display(outImage);
  while(!display.is_closed()){
    display.wait();
  }

  outImage.save_png(outfile);

  #pragma omp parallel for
  for(int i = 0; i < Width; ++i) {
    free(rays[i]);
  }
  free(rays);

  #pragma omp parallel for
  {
    for(int i = 0; i < Width*Height; ++i) {
      free(hits[i].material.diffuse);
      free(hits[i].material.specular);
    }
  }

  free(hits);

  #pragma omp parallel for 
  {
    for(int i = 0; i < Width; ++i) {
      free(colors[i]);
    }
  }

  free (colors);

  return 0;
}
