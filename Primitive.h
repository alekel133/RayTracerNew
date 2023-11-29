#pragma once
#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include <ostream>
#include <string>
#include <float.h>
#include "eigen-3.4.0/Eigen/Dense"
#include "Ray.h"

using std::string;
using namespace Eigen;


typedef struct {
  int *diffuse;
  int *specular;
  int shine;
} Material;

typedef struct{
	double t;
  int i;
  int j;
  Vector4d normal;
  Material material;
} Hit;

typedef struct{
  int vertices[3];
}Face;

class Primitive {
	public:
		// Data for recursive transform application
		string label;
		Primitive *parent;
		Primitive **children;
    Material material;
    Matrix4d mat;
    

		// Virtual class functions
		Primitive();
		~Primitive();
		// Virtual Functions for Ray Tracing and Ray Acceleration
		virtual bool isHit(Ray *ray, Hit &hit) = 0;
    virtual void print(std::ostream &os) = 0;
    virtual Vector4d getMin() = 0;
    virtual Vector4d getMax() = 0;
	  Matrix4d getLocal();
	  Matrix4d getWorld();
		friend std::ostream &operator << (std::ostream &os, Primitive &prim);
};

class Camera: public Primitive {
	public:
		// Camera, like all Prmitives, is initialized at the origin, and moved by a series of transformations
		Camera(void);
		
		// Function for outputing the data stored in the Camera object.
    bool isHit(Ray *ray, Hit &hit) override;
    Vector4d getMin() override;
    Vector4d getMax() override;
    void print(std::ostream &os) override;
};

class Sphere: public Primitive {
	public:
		float radius;
		Vector4d origin;
		
		Sphere();
		Sphere(Material &material);
		bool isHit(Ray *ray, Hit &hit) override;
    Vector4d getMin() override;
    Vector4d getMax() override;
    void print(std::ostream &os) override;
};

class Plane: public Primitive {
	public:
		bool isHit(Ray *ray, Hit &hit) override;
    Vector4d getMin() override;
    Vector4d getMax() override;
    void print(std::ostream &os) override;
};

class Mesh: public Primitive {
  public:
    std::vector<Vector4d> vertices;
    std::vector<Face> faces;

    Mesh();
    ~Mesh();
    bool isHit(Ray *ray, Hit &hit) override;
    Vector4d getMin() override;
    Vector4d getMax() override;
    void print(std::ostream &os) override;
};

class AABB: public Primitive {
  public:
    Vector4d maxPoint;
    Vector4d minPoint;
    std::vector<Primitive*> children;

    void genBox(std::vector <Primitive*> prims);
    bool isHit(Ray *ray, Hit &hit) override;
    Vector4d getMin() override;
    Vector4d getMax() override;
    void print(std::ostream &os) override;
};

class BVH: public Primitive {
  public:
    AABB *root;

    bool isHit(Ray *ray, Hit &hit) override;
    Vector4d getMin() override;
    Vector4d getMax() override;
    void print(std::ostream &os) override;
};

Ray *generateRay(Camera &camera, int &iWidth, int &iHeight, int &i, int &j);

#endif
