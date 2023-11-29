#include "MathUtils.h"

void rotateX(double theta, Eigen::Matrix4d &mat) {
  Eigen::Matrix4d rot;
  theta = theta * (M_PI/180);
  double sine = sin(theta);
  double cosine = cos(theta);
  rot << 1, 0, 0, 0,
         0, cosine, -sine, 0,
         0, sine, cosine, 0,
         0, 0, 0, 1;

  mat = rot * mat;
}

void rotateY(double theta, Eigen::Matrix4d &mat) {
  Eigen::Matrix4d rot;
  theta = theta * (M_PI/180);
  double sine = sin(theta);
  double cosine = cos(theta);
  rot << cosine, 0, sine, 0,
         0, 1, 0, 0,
         -sine, 0, cosine, 0,
         0, 0, 0, 1;

  mat = rot * mat;
}

void rotateZ(double theta, Eigen::Matrix4d &mat) {
  Eigen::Matrix4d rot;
  theta = theta * (M_PI/180);
  double sine = sin(theta);
  double cosine = cos(theta);
  rot << cosine, -sine, 0, 0,
         sine, cosine, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1;

  mat = rot * mat;
}

void translateX(double d, Eigen::Matrix4d &mat) {
  Eigen::Matrix4d rot;
  rot << 1, 0, 0, d,
         0, 1, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1;

  mat = rot * mat;
}

void translateY(double d, Eigen::Matrix4d &mat) {
  Eigen::Matrix4d rot;
  rot << 1, 0, 0, 0,
         0, 1, 0, d,
         0, 0, 1, 0,
         0, 0, 0, 1;

  mat = rot * mat;
}

void translateZ(double d, Eigen::Matrix4d &mat) {
  Eigen::Matrix4d rot;
  rot << 1, 0, 0, 0,
         0, 1, 0, 0,
         0, 0, 1, d,
         0, 0, 0, 1;

  mat = rot * mat;
}

void scale(double factor, Eigen::Matrix4d &mat) {
  Eigen::Matrix4d rot;
  rot << factor, 0, 0, 0,
         0, factor, 0, 0,
         0, 0, factor, 0,
         0, 0, 0, 1;

  mat = rot * mat;
}
