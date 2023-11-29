#pragma once
#ifndef LIGHTS_H
#define LIGHTS_H
#include "Primitive.h"

class Light: public Primitive {
  public:
    Light() {}
    ~Light() {}

    virtual int* illuminate(Ray *ray, Hit &hit, bool vis) = 0;
    virtual Vector4d getDirection(Vector4d point) = 0;
    virtual Vector4d getPoint() = 0;
};

class PointLight:public Light {
  public:
    Vector4d point;

    PointLight();
    PointLight(Vector4d &point, int* color);
    bool isHit(Ray *ray, Hit &hit) override;
    int* illuminate(Ray* ray, Hit &hit, bool vis) override;
    Vector4d getDirection(Vector4d point) override;
    Vector4d getPoint() override;
    Vector4d getMin() override;
    Vector4d getMax() override;
    void print(std::ostream &os) override;
};

class DirectionalLight: public Light {
  public:
    Vector4d direction;

    DirectionalLight();
    DirectionalLight(Vector4d &direction, int* color);
    bool isHit(Ray *ray, Hit &hit) override;
    int* illuminate(Ray* ray, Hit &hit, bool vis) override;
    Vector4d getDirection(Vector4d point) override;
    Vector4d getPoint() override;
    Vector4d getMin() override;
    Vector4d getMax() override;
    void print(std::ostream &os) override;
};



#endif
