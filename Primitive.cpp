#include "Primitive.h"
#include <iostream>

//~~~~~~~~~~~~~~~~~~~~~~~ Primitive ~~~~~~~~~~~~~~~~~~~~~~~~~//
Primitive::Primitive() {
  this->label = "None";
	this->parent = (Primitive *) malloc(sizeof(Primitive *));
	this->children = nullptr;
  this->material = Material();
  this->mat = mat.Identity();
}

Primitive::~Primitive(){
	parent = nullptr;
  free(parent);
}

bool Primitive::isHit(Ray *ray, Hit &hit) {
	hit.t = 0;
	return false;
}

Matrix4d Primitive::getLocal() {
	if(parent == nullptr) {
    return this->mat;
	}
	return this->mat * this->parent->getLocal();
}

Matrix4d Primitive::getWorld() {
	if(parent == nullptr) {
		return this->mat.inverse();
	}

	return this->parent->getWorld() * this->mat.inverse();
}

std::ostream &operator <<(std::ostream &os, Primitive &prime) {
  prime.print(os);
  return os;
}
//~~~~~~~~~~~~~~~~~~~~~~~ Camera ~~~~~~~~~~~~~~~~~~~~~~~~~//
Camera::Camera() {
	this->parent = nullptr;
	this->children = nullptr;
  this->mat = mat.Identity();
}

bool Camera::isHit(Ray *ray, Hit &hit) {
  return false;
}

Vector4d Camera::getMin() {
  return mat.inverse() * Vector4d(0,0,0,1);
}

Vector4d Camera::getMax() {
  return mat.inverse() * Vector4d(0,0,0,1);
}

void Camera::print(std::ostream &os) {
	os << "Camera ( Eye:[" << (getWorld() * Vector4d(0,0,0,1)).transpose() << "], Direction:[" << (getWorld()*Vector4d(0,0,1,0)).transpose() <<
		"], Up:[" << (getWorld()*Vector4d(0,1,0,0)).transpose() << "], Side:[" << (getWorld()*Vector4d(1,0,0,0)).transpose() << "])";
}

//~~~~~~~~~~~~~~~~~~~~~~~ Sphere ~~~~~~~~~~~~~~~~~~~~~~~~~//
Sphere::Sphere() {
  this->material = Material();
}

Sphere::Sphere(Material &material) {
  this->material = material;
}

bool Sphere::isHit(Ray *ray, Hit &hit) {
  hit.material = material;
  Vector4d m = ray->origin;
  double b = ray->direction.dot(m);
  double c = m.dot(m) - 2;
  
  double disc = (b*b)-c;
  if(disc < 0){
    return false;
  }
  
  double t0 = -b - sqrt(disc);
  double t1 = -b + sqrt(disc);


  if(t1 < 0) {
    return false;
  }

  if(t0 < 0){
    if(t1 > ray->tNear) return false;
    hit.t = t1;
    Vector4d point = ray->eval(hit.t);
    Vector4d normal((getLocal() * point)-Vector4d(0,0,0,1));
    hit.normal = (getWorld() * normal).normalized();
    return true;
  }

  if(t1 != t0) {
    hit.t = std::min(t0, t1);
    if(hit.t > ray->tNear) return false;
    Vector4d point = ray->eval(hit.t);
    Vector4d normal((getLocal() * point)-origin);
    hit.normal = (getWorld() * normal).normalized();
    return true;
  }
  
  if(t0 > ray->tNear) return false;
  hit.t = t0;
  Vector4d point = ray->eval(hit.t);
  Vector4d normal((getLocal() * point)-origin);
  hit.normal = (getWorld() * normal).normalized();
  return true;
}

Vector4d Sphere::getMin() {
  double x = getWorld()(0,3) - sqrt(getWorld()(0,0)*getWorld()(0,0) + getWorld()(0,1)*getWorld()(0,1) + getWorld()(0,2)*getWorld()(0,2));
  double y = getWorld()(1,3) - sqrt(getWorld()(1,0)*getWorld()(1,0) + getWorld()(1,1)*getWorld()(1,1) + getWorld()(1,2)*getWorld()(1,2));
  double z = getWorld()(2,3) - sqrt(getWorld()(2,0)*getWorld()(2,0) + getWorld()(2,1)*getWorld()(2,1) + getWorld()(2,2)*getWorld()(2,2));
  return Vector4d(x,y,z,1);
}

Vector4d Sphere::getMax() {
  double x = getWorld()(0,3) + sqrt(getWorld()(0,0)*getWorld()(0,0) + getWorld()(0,1)*getWorld()(0,1) + getWorld()(0,2)*getWorld()(0,2));
  double y = getWorld()(1,3) + sqrt(getWorld()(1,0)*getWorld()(1,0) + getWorld()(1,1)*getWorld()(1,1) + getWorld()(1,2)*getWorld()(1,2));
  double z = getWorld()(2,3) + sqrt(getWorld()(2,0)*getWorld()(2,0) + getWorld()(2,1)*getWorld()(2,1) + getWorld()(2,2)*getWorld()(2,2));
  return Vector4d(x,y,z,1);
}

void Sphere::print(std::ostream &os) {
	os << "Sphere ( Name: " << label << ", Origin:[" << (getWorld()*Vector4d(0,0,0,1)).transpose() << "], Radius: " 
		<< (getWorld()*Vector4d(1,0,0,0)).norm();
}

//~~~~~~~~~~~~~~~~~~~~~~~ Plane ~~~~~~~~~~~~~~~~~~~~~~~~~//
bool Plane::isHit(Ray *ray, Hit &hit) {
  hit.material = material;
  Vector4d eye = ray->origin;
  Vector4d direction = ray->direction;
  Vector4d normal = Vector4d(0,1, 0, 0);
  double bottom = normal.dot(direction);
  if(bottom == 0) return false;
  double invD = 1/bottom;
  double t = (-eye).dot(normal) * invD;

  if(t >= 0){
    if(t > ray->tNear) return false;
    hit.t = t;
    hit.normal = (getWorld()* Vector4d(0,-1,0,0)).normalized();
    return true;
  }

  return false;
}

Vector4d Plane::getMin() {
  return Vector4d(-INFINITY, -INFINITY, -INFINITY, 1);
}

Vector4d Plane::getMax() {
  return Vector4d(INFINITY, INFINITY, INFINITY, 1);
}

void Plane::print(std::ostream &os) {
	os << "Plane ( Name: " << label << ", Origin:[" << (getWorld()*Vector4d(0,0,0,1)).transpose() << "], Normal:["
		<< (getWorld()*Vector4d(0,1,0,0)).transpose() << "]";
}


//----------------------- Mesh --------------------------------//
Mesh::Mesh() {
  (void) 0;
}

bool Mesh::isHit(Ray *ray, Hit &hit) {
  double t;
  hit.t = DBL_MAX;
  for(int i = 0; i < faces.size(); ++i){
    hit.material = material;
    Vector3d direction, eye, p, AB, AC, pvec,tvec,qvec;
    direction << ray->direction[0], ray->direction[1], ray->direction[2];
    direction.normalize();
    eye << ray->origin[0], ray->origin[1], ray->origin[2];
    p << vertices[faces[i].vertices[0]][0], vertices[faces[i].vertices[0]][1], vertices[faces[i].vertices[0]][2];
    AB << vertices[faces[i].vertices[1]][0] - p[0], vertices[faces[i].vertices[1]][1]-p[1], vertices[faces[i].vertices[1]][2]-p[2];
    AC << vertices[faces[i].vertices[2]][0] - p[0], vertices[faces[i].vertices[2]][1]-p[1], vertices[faces[i].vertices[2]][2]-p[2];

    pvec << direction.cross(AC);
    double det = AB.dot(pvec);

    if(det < 0 ) continue;

    double invDet = 1.0/det;

    tvec = eye - p;

    double beta = tvec.dot(pvec) * invDet;
    if(beta < 0 || beta > 1) continue;
    
    qvec << tvec.cross(AB);
    double gamma = direction.dot(qvec) * invDet;
    double alpha = 1 - (beta + gamma);

    if(gamma < 0 || alpha < 0) continue;

  
    t = AC.dot(qvec) * invDet;
    if(t > ray->tNear) return false;
    if(t < hit.t) {
      hit.t = t;
      hit.material = material;
      hit.normal << AB.cross(AC).normalized(), 0;
      hit.normal = getWorld() * hit.normal;
    }
  }
  if(hit.t == DBL_MAX) {
    return false;
  }

  return true; 
}

Vector4d Mesh::getMin() {
  double x = INFINITY, y = INFINITY, z = INFINITY;
  Vector4d vert;
  for(int i = 0; i < vertices.size(); ++i)   {
    vert = getWorld() * vertices[i];
    if(vert[0] < x) x = vert[0];
    if(vert[1] < y) y = vert[1];
    if(vert[2] < z) z = vert[2];
  }

  return Vector4d(x,y,z,1);
}

Vector4d Mesh::getMax() {
  double x = -INFINITY, y = -INFINITY, z = -INFINITY;
  Vector4d vert;
  for(int i = 0; i < vertices.size(); ++i)   {
    vert = getWorld() * vertices[i];
    if(vert[0] > x) x = vert[0];
    if(vert[1] > y) y = vert[1];
    if(vert[2] > z) z = vert[2];
  }

  return Vector4d(x,y,z,1);
}

void Mesh::print(std::ostream &os){
  os << "Mesh: (Label: " << label << ", Parent: " << parent << ")";
}

//--------------------- AABB -----------------------
//TODO: Get Bounds for box boundaries
void AABB::genBox(std::vector<Primitive*> prims) {
  Vector4d pMin, pMax;
  minPoint = Vector4d(INFINITY, INFINITY, INFINITY, 1);
  maxPoint = Vector4d(-INFINITY, -INFINITY, -INFINITY, 1);
  if(prims.size() == 1) {
    for(int i = 0; i < prims.size(); ++i) {
      pMin = prims[i]->getMin();
      pMax = prims[i]->getMax();
      if(minPoint[0] > pMin[0]) minPoint[0] = pMin[0];
      if(minPoint[1] > pMin[1]) minPoint[1] = pMin[1];
      if(minPoint[2] > pMin[2]) minPoint[2] = pMin[2];
      if(maxPoint[0] < pMax[0]) maxPoint[0] = pMax[0];
      if(maxPoint[1] < pMax[1]) maxPoint[1] = pMax[1];
      if(maxPoint[2] < pMax[2]) maxPoint[2] = pMax[2];
    }
  }
  children = prims;
}

bool AABB::isHit(Ray* ray, Hit& hit) {
  // The code surrounded in //---// was taken from: https://tavianator.com/2022/ray_box_boundary.html
  // An article by Tavian Barnes on AABB intersection calculation

  //---------------------------------------------------------------//
  float tmin = 0.0, tmax = INFINITY;

  for (int d = 0; d < 3; ++d) {
      Vector4d dir_inv(1/ray->direction[0], 1/ray->direction[1], 1/ray->direction[2], 1);
      float t1 = (minPoint[d] - ray->origin[d]) * dir_inv[d];
      float t2 = (maxPoint[d] - ray->origin[d]) * dir_inv[d];

      tmin = fmax(tmin, fmin(t1, t2));
      tmax = fmin(tmax, fmax(t1, t2));
  }
  
  //---------------------------------------------------------------//
  if(tmin < tmax) {
    if(children.size() == 1) {
      return children[0]->isHit(transformRay(ray, children[0]->getLocal()), hit);
    }
  }
  return false;
}

Vector4d AABB::getMin() {
  return minPoint;
}

Vector4d AABB::getMax() {
  return maxPoint;
}

void AABB::print(std::ostream &os) {
  os << "AABB" << std::endl;
}

//--------------------- BVH ------------------------
//TODO: Generate Heirarchy
//TODO: Recursive isHit()
//
// Ray generation function
Ray *generateRay(Camera &camera, int &iWidth, int &iHeight, int &i, int &j) {
  Vector4d dir = {0,0,1,0};
  Vector4d eye(0,0,0,1);

  dir[0] = (((((i + 0.5)/iWidth)-0.5)));
  dir[1] = (((((j + 0.5)/iHeight)-0.5)));

  Ray *ray = new Ray(eye, dir);
  ray->origin = camera.mat.inverse() * ray->origin;
  ray->direction = camera.mat.inverse() * ray->direction;
  return ray;
}
