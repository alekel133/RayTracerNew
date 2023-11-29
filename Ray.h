#pragma once
#ifndef RAY_H
#define RAY_H
#include <ostream>
#include <math.h>
#include  <float.h>
#include "eigen-3.4.0//Eigen/Dense"

using namespace Eigen;

class Ray {
	public:	
		Vector4d origin;
		Vector4d direction;	
    double tNear;

		Ray();
		Ray(Vector4d &origin, Vector4d &direction);
		Vector4d eval(double &t);
		friend std::ostream &operator << (std::ostream &os, Ray &ray);
};

Ray* transformRay(Ray* ray, Matrix4d trans);

#endif
