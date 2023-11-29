#include "Light.h"
#include <iostream>

//------------------------- Point Light-------------------------//
PointLight::PointLight() {
  this->point = Vector4d(0,0,0,1);
  this->material.diffuse = nullptr;
}

PointLight::PointLight(Vector4d &point, int *color) {
  this->point = point;
  this->material.diffuse = color;
}

bool PointLight::isHit(Ray* ray, Hit &hit) {
  return false;
}

int *PointLight::illuminate(Ray* ray, Hit &hit, bool vis){
  int color[3];
  color[0] = this->material.shine*this->material.diffuse[0];
  color[1] = this->material.shine*this->material.diffuse[1];
  color[2] = this->material.shine*this->material.diffuse[2];
  this->material.specular[0] = 0;
  this->material.specular[1] = 0;
  this->material.specular[2] = 0;
  if(!vis) return this->material.specular;
  Vector4d l = point - getLocal() * ray->eval(hit.t);
  Vector4d v = (getLocal() * -ray->direction);
  Vector4d h = (l+v).normalized();
  double distance = l.norm();
  l.normalize();
  double invDist = 1/(distance * distance);
  double aofi = l.dot(hit.normal);

  for(int i = 0; i < 3; ++i) {
    this->material.specular[i] = int((pow(hit.normal.dot(h), hit.material.shine) * hit.material.specular[i])+(color[i] * aofi * hit.material.diffuse[i] * invDist * 100));
    if(this->material.specular[i] > 255)this->material.specular[i] = 255;
    if(this->material.specular[i] < 0) this->material.specular[i] = 0;
  }

  return material.specular;
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
  this->material.specular = nullptr;
}

DirectionalLight::DirectionalLight(Vector4d &direction, int *color) {
  this->direction = direction;
  this->material.specular = color;
}

bool DirectionalLight::isHit(Ray* ray , Hit &hit) {
  return false;
}

int *DirectionalLight::illuminate(Ray *ray, Hit &hit, bool vis) {
  int color[3];
  color[0] = this->material.shine*this->material.diffuse[0];
  color[1] = this->material.shine*this->material.diffuse[1];
  color[2] = this->material.shine*this->material.diffuse[2];

  material.specular[0] = 0;
  material.specular[1] = 0;
  material.specular[2] = 0;
  if(!vis) return material.specular;
  Vector4d dir = getLocal() * direction;
  dir.normalize();
  Vector4d v = (getLocal() * -ray->direction);
  Vector4d h = (dir+v).normalized();
  double aofi = dir.dot(hit.normal);

  for(int i = 0; i < 3; ++i) {
    material.specular[i] = int((pow(hit.normal.dot(h), hit.material.shine) * hit.material.specular[i])+(color[i] * aofi * hit.material.diffuse[i]));
    if(material.specular[i] > 255) material.specular[i] = 255;
    if(material.specular[i] < 0) material.specular[i] = 0;
  }

  return material.specular;
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
  os << "PointLight (Label: " << label << ")";
}
