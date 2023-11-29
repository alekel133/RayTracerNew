/***************************************************************************************
Definitions for flex parsing
***************************************************************************************/

%option yylineno
%option noyywrap

%{
#include "lexer.h"
extern "C" {
  int yylex(void);
}
%}

%%
"//".*
"/*".*"*/"
[\n\t\r ]+
-?[[:digit:]]*[.][[:digit:]]*     { return DOUBLE; }
-?[[:digit:]]*                    { return INT; }
"NONE"                            { return NONE; }
"Width"                           { return WIDTH; }
"Height"                          { return HEIGHT; }
"FOV"                             { return FOV; }
"Ambient"                         { return AMBIENT; }
"Camera"                          { return CAMERA; }
"Sphere"                          { return SPHERE; }
"Plane"                           { return PLANE; }
"Mesh"                            { return MESH; }
"Point"                           { return POINT; }
"Direction"                       { return DIRECTION; }
"label"                           { return LABEL; }
"material"                        { return MATERIAL; }
"parent"                          { return PARENT; }
"diffuse"                         { return DIFFUSE; }
"specular"                        { return SPECULAR; }
"shine"                           { return SHINE; }
"color"                           { return COLOR; }
"intensity"                       { return INTENSITY; }
"transform"                       { return TRANSFORM; }
"vertices"                        { return VERTICES; }
"faces"                           { return FACES; }
"v"                               { return VERTEX; }
"f"                               { return FACE; }
"RX"                              { return ROTATEX; }
"RY"                              { return ROTATEY; }
"RZ"                              { return ROTATEZ; }
"TX"                              { return TRANSLATEX; }
"TY"                              { return TRANSLATEY; }
"TZ"                              { return TRANSLATEZ; }
"SC"                              { return SCALE; }
"="                               { return EQUAL; }
"("                               { return OPENPAREN; }
")"                               { return CLOSEPAREN; }
"{"                               { return OPENBRACK; }
"}"                               { return CLOSEBRACK; }
","                               { return COMMA; }
":"                               { return COLON; }
";"                               { return SEMI; }
[a-zA-Z]{1,64}                    { return IDENTIFIER;}
<<EOF>>                           { return ENDOFFILE; }
%%

