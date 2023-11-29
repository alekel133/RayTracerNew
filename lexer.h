#pragma once
#ifndef LEXER_H
#define LEXER_H

// Image Data
#define HEIGHT      1000
#define WIDTH       1001
#define FOV         1002
#define AMBIENT     1003

// Primitive Definition
#define CAMERA      2000
#define SPHERE      2001
#define PLANE       2002
#define MESH        2003

// Light Definition
#define POINT       3000
#define DIRECTION   3001

// MISC DATA
#define LABEL       4000
#define PARENT      4001
#define MATERIAL    4002
#define TRANSFORM   4003
#define VERTICES    4004
#define FACES       4005
#define DIFFUSE     4006
#define SPECULAR    4007
#define SHINE       4008
#define COLOR       4009
#define VERTEX      4010 
#define FACE        4011 
#define INTENSITY   4012


// LITERALS
#define INT         5000
#define DOUBLE      5001
#define IDENTIFIER  5002
#define ROTATEX     5003
#define ROTATEY     5004
#define ROTATEZ     5005
#define TRANSLATEX  5006
#define TRANSLATEY  5007
#define TRANSLATEZ  5008
#define SCALE       5009
#define NONE        5010

// PUNCUATION
#define EQUAL       6000
#define COLON       6001
#define SEMI        6002
#define COMMA       6003
#define OPENPAREN   6004
#define CLOSEPAREN  6005
#define OPENBRACK   6006
#define CLOSEBRACK  6007

// OTHER
#define ENDOFFILE         7000

#endif
