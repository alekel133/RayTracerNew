#pragma once
#ifndef PARSERUTILS_H
#define PARSERUTILS_H
#include <vector>
#include <iostream>
#include "eigen-3.4.0/Eigen/Dense"
#include "lexer.h"
#include "Primitive.h"
#include "Light.h"
#include "MathUtils.h"

extern "C" {
  extern char *yytext;
  extern FILE* yyin;
  extern FILE* yyout;
  extern int yylineno;
  extern int yylex();
}

typedef struct {
  int height;
  int width;
  int fov;
  double ambient;
  Camera *camera;
  std::vector<Primitive*> primitives;
  std::vector<Light*> lights;
} ParseData;

// Root Expression
void parseFile(char *filename, ParseData &data);

// Secondary Expressions
void parseImageData(ParseData &data);
void parseCameraData(ParseData &data);
void parseLightData(ParseData &data);
void parseObjectData(ParseData &data);

// Useful abstractions of object data
void parseSphereData(ParseData &data);
void parseMeshData(ParseData &data);
void parsePlaneData(ParseData &data);

// Tertiary Expressions
void parseLabelData(Primitive *primitive);
void parseMaterialData(Primitive *primitive);
void parseTopologyData(Mesh* mesh);
void parseVertexData(std::vector<Vector4d> &vertices);
void parseFaceData(std::vector<Face> &faces);
void parseTransformData(Matrix4d &matrix);
#endif
