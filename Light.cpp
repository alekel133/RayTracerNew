#include "Light.h"
#include <iostream>

//------------------------- Point Light-------------------------//
PointLight::PointLight() {
  this->point = Vector4d(0,0,0,1);
  this->parent = nullptr;
  this->material.diffuse = new int[3];
  this->material.specular = new int[3];
}

PointLight::PointLight(Vector4d &point, int *color) {
  this->point = point;
  this->material.diffuse = color;
  this->material.specular = new int[3];
}

bool PointLight::isHit(Ray* ray, Hit &hit) {
  return false;
}

int *PointLight::illuminate(Ray* ray, Hit &hit){
  int *color = new int[3];
  color[0] = 0;
  color[1] = 0;
  color[2] = 0;
  Vector4d l = point - ray->eval(hit.t);
  Vector4d v = -ray->direction;
  Vector4d h = (l+v).normalized();
  Vector4d surfaceNorm = getLocal() * hit.normal;
  double distance = l.norm();
  l.normalize();
  double invDist = 1/(distance * distance);
  double aofi = l.dot(surfaceNorm);

  for(int i = 0; i < 3; ++i){ 
    color[i] = int((pow(surfaceNorm.dot(h), hit.material.shine) * hit.material.specular[i]) + ((this->material.shine * this->material.diffuse[i]/255.0) * aofi * hit.material.diffuse[i] * invDist * 75));
    if(color[i] > 255) color[i] = 255;
    if(color[i] < 0) color[i] = 0;
  }

  return color;
}

Vector4d PointLight::getDirection(Vector4d p) {
  Vector4d ret = point - getLocal() *p;
  ret(3) = 0;
  ret.normalize();
  return getWorld() * ret;
}

Vector4d PointLight::getPoint() {
  return getWorld() * point;
};

Vector4d PointLight::getMin() {
  return getPoint();
}

Vector4d PointLight::getMax() {
  return getPoint();
}

void PointLight::print(std::ostream &os) {
  os << "PointLight (Label: " << label << ")";
}
//------------------------ Directional Light ------------------//
DirectionalLight::DirectionalLight() {
  this->direction = Vector4d(0,0,1,0);
  this->parent = nullptr;
  this->material.specular = nullptr;
}

DirectionalLight::DirectionalLight(Vector4d &direction, int *color) {
  this->direction = direction;
  this->material.specular = color;
}

bool DirectionalLight::isHit(Ray* ray , Hit &hit) {
  return false;
}

int *DirectionalLight::illuminate(Ray *ray, Hit &hit) {
  int *color = new int[3];
  color[0] = 0;
  color[1] = 0;
  color[2] = 0;
  Vector4d dir = direction;
  dir.normalize();
  Vector4d v = -ray->direction;
  Vector4d h = (dir+v).normalized();
  double aofi = dir.dot(getLocal() * hit.normal);

  for(int i = 0; i < 3; ++i) {
    color[i] = int((pow((getLocal() * hit.normal).dot(h), hit.material.shine) * hit.material.specular[i])+((material.shine * this->material.diffuse[i]/255.0) * aofi * hit.material.diffuse[i]));
    if(color[i] > 255) color[i] = 255;
    if(color[i] < 0) color[i] = 0;
  }

  return color;
}

Vector4d DirectionalLight::getDirection(Vector4d point){
  return (getWorld()*(direction)).normalized();
}


Vector4d DirectionalLight::getPoint() {
  return Vector4d(DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX);
}

Vector4d DirectionalLight::getMin() {
  return getPoint();
}

Vector4d DirectionalLight::getMax() {
  return getPoint();
}

void DirectionalLight::print(std::ostream &os) {
  os << "DirectionalLight (Label: " << label << ")";
}

