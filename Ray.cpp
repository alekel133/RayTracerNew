#include "Ray.h"

// Default constructor, defines ray from origin with no direction
Ray::Ray() {
	this->origin = Vector4d(0,0,0,0);
	this->direction = Vector4d(0,0,0,0);
  this->tNear = DBL_MAX;
}

// Defines a ray at origin, moving in direction
Ray::Ray(Vector4d &origin, Vector4d &direction) {
	this->origin = origin;
	this->origin(3) = 1;
	this->direction = direction;
  this->direction.normalize();
  this->tNear = DBL_MAX;
}

// Returns Evaluated Ray Euation as a Ponit
Vector4d Ray::eval(double &t) {
	return (this->direction * t) + this->origin;
}

std::ostream &operator << (std::ostream &os, Ray &ray) {
	os << "Ray: [" <<ray.origin.transpose() << "]" << " + " << "[" <<ray.direction.transpose() 
		<< "]" << "t";
	return os;
}


Ray* transformRay(Ray* ray, Matrix4d trans) {
  Ray* r = new Ray;
  r->origin = trans * ray->origin;
  r->direction = trans * ray->direction;
  r->direction.normalize();

  return r;
}

