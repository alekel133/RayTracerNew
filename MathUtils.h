#pragma once
#ifndef MATHUTILS_H
#define MATHUTILS_H
#include <math.h>
#include "eigen-3.4.0/Eigen/Dense"
void rotateX(double theta, Eigen::Matrix4d &mat);
void rotateY(double theat, Eigen::Matrix4d &mat);
void rotateZ(double theta, Eigen::Matrix4d &mat);
void translateX(double d, Eigen::Matrix4d &mat);
void translateY(double d, Eigen::Matrix4d &mat);
void translateZ(double d, Eigen::Matrix4d &mat);
void scale(double factor, Eigen::Matrix4d &mat);

#endif
