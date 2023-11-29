#include "Parser.h"
int token;

void putError(string message) {
  std::cout << yylineno << ": " << message << std::endl;
  exit(EXIT_FAILURE);
}

bool isDouble() {
  if(token != DOUBLE && token != INT) return false;
  return true;
}

void checkToken(int VAL, string message) {
  if(token != VAL) {
    putError(message);
  }
}

void parseLabelData(Primitive *prim, std::vector<Primitive *> &prims, int isObject) {
  checkToken(LABEL, "Expeceted Label Data.");

  token = yylex();
  checkToken(COLON, "Expeceted ':'.");

  token = yylex();
  checkToken(IDENTIFIER, "Expected Identifier.");
  prim->label = yytext;

  token = yylex();
  checkToken(SEMI, "No terminating semicolon.");

  if(!isObject) {
    return;
  }

  token = yylex();
  checkToken(PARENT, "Expected Parent Data.");

  token = yylex();
  checkToken(COLON, "Expected ':'.");

  token = yylex();
  if(token != NONE) {
    checkToken(IDENTIFIER, "Expeceted Ientifier.");
    prim->parent = nullptr;
    for(int i = 0; i < prims.size(); ++i) {
      if(yytext == prims[i]->label) {
        prim->parent = prims[i];
      }
    }

    if(prim->parent == nullptr) {
      std::cout << "Invalid Parent." << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  else {
    prim->parent = nullptr;
  }

  token = yylex();
  checkToken(SEMI, "Expected ';'.");
}

void parseMaterialData(Primitive *prim) {
  checkToken(MATERIAL, "Expected Material Data.");

  token = yylex();
  checkToken(COLON, "Expected ':'.");

  prim->material.diffuse = new int[3];
  prim->material.specular = new int[3];

  token = yylex();
  checkToken(DIFFUSE, "Expected Diffuse Data.");

  token = yylex();
  checkToken(COLON, "Expected ':'.");

  token = yylex();
  checkToken(OPENPAREN, "Invalid Color Data.");

  token = yylex();
  checkToken(INT, "Invalid Color Data.");

  prim->material.diffuse[0] = atoi(yytext);

  token = yylex();
  checkToken(COMMA, "Invalid Color Data.");

  token = yylex();
  checkToken(INT, "Invalid Color Data.");

  
  prim->material.diffuse[1] = atoi(yytext);

  token = yylex();
  checkToken(COMMA, "Invalid Color Data.");

  token = yylex();
  checkToken(INT, "Invalid Color Data.");

  
  prim->material.diffuse[2] = atoi(yytext);

  token = yylex();
  checkToken(CLOSEPAREN, "Invalid Color Data.");

  token = yylex();
  checkToken(SEMI, "No terminating semicolon");

  token = yylex();
  checkToken(SPECULAR, "Expected Specular Data.");

  token = yylex();
  checkToken(COLON, "Expected ':'.");

  token = yylex();
  checkToken(OPENPAREN, "Invalid Color Data.");

  token = yylex();
  checkToken(INT, "Invalid Color Data.");

  prim->material.specular[0] = atoi(yytext);

  token = yylex();
  checkToken(COMMA, "Invalid Color Data.");

  token = yylex();
  checkToken(INT, "Invalid Color Data.");

  
  prim->material.specular[1] = atoi(yytext);

  token = yylex();
  checkToken(COMMA, "Invalid Color Data.");

  token = yylex();
  checkToken(INT, "Invalid Color Data.");

  
  prim->material.specular[2] = atoi(yytext);

  token = yylex();
  checkToken(CLOSEPAREN, "Invalid Color Data.");

  token = yylex();
  checkToken(SEMI, "No terminating semicolon");

  token = yylex();
  checkToken(SHINE, "Expected Shine Exponent.");

  token = yylex();
  checkToken(COLON, "Expected ':'.");

  token = yylex();

  prim->material.shine = atof(yytext);

  token = yylex();
  checkToken(SEMI, "No terminating semicolon");
}

void parseTopologyData(Mesh* mesh) {
  checkToken(VERTICES, "Expected Vertex List.");

  token = yylex();
  checkToken(COLON, "Expected ':'");

  token = yylex();
  parseVertexData(mesh->vertices);

  checkToken(FACES, "Expected Face Data.");

  token = yylex();
  checkToken(COLON, "Expected ':'.");

  token = yylex();
  parseFaceData(mesh->faces);
}

void parseVertexData(std::vector<Vector4d> &vertices) {
  Vector4d vert;
  int i = 0;
  while(token == VERTEX) {
    vert = Vector4d();
    token = yylex();
    if(!isDouble()) putError("Invalid Vertex.");
    vert[0] = atof(yytext);

    token = yylex();
    if(!isDouble()) putError("Invalid Vertex.");
    vert[1] = atof(yytext);

    token = yylex();
    if(!isDouble()) putError("Invalid Vertex.");
    vert[2] = atof(yytext);
    vert[3] = 1;
    
    vertices.push_back(vert);
    ++i;
    token = yylex();
  }
}

void parseFaceData(std::vector<Face> &faces) {
  Face* face;
  int i = 0;
  while(token == FACE) {
    face = new Face;
    token = yylex();
    checkToken(INT, "Invalid Index");
    face->vertices[0] = atoi(yytext);

    token = yylex();
    checkToken(INT, "Invalid Index");
    face->vertices[1] = atoi(yytext);

    token = yylex();
    checkToken(INT, "Invalid Index");
    face->vertices[2] = atoi(yytext);
    
    faces.push_back(*face);
    ++i;
    token = yylex();
  } 
}

void parseTransformData(Matrix4d &matrix) {
  checkToken(TRANSFORM, "Expected Transform Data.");

  token = yylex();
  checkToken(COLON, "Expected ':'");

  
  bool stop = false;
  while(true) {
    if(stop) {
      break;
    }
    token = yylex();
    switch(token) {
      case ROTATEX: {
        token = yylex();
        checkToken(COLON, "Expected ':'.");

        token = yylex();
        if(!isDouble()) putError("Invalid Vertex.");
        
        rotateX(atof(yytext), matrix);

        token = yylex();
        if(token == SEMI) {
          stop = true;
          break;
        }

        checkToken(COMMA, "Expected ';' or ','.");
        break;
      }

      case ROTATEY: {
        token = yylex();
        checkToken(COLON, "Expected ':'.");

        token = yylex();
        if(!isDouble()) putError("Invalid Transform.");
        
        rotateY(atof(yytext), matrix);

        token = yylex();
        if(token == SEMI) {
          stop = true;
          break;
        }

        checkToken(COMMA, "Expected ';' or ','.");
        break;
      }

      case ROTATEZ: {
        token = yylex();
        checkToken(COLON, "Expected ':'.");

        token = yylex();
        if(!isDouble()) putError("Invalid Transform.");
        
        rotateZ(atof(yytext), matrix);

        token = yylex();
        if(token == SEMI) {
          stop = true;
          break;
        }

        checkToken(COMMA, "Expected ';' or ','.");
        break;
      }

      case TRANSLATEX: {
        token = yylex();
        checkToken(COLON, "Expected ':'.");

        token = yylex();
        if(!isDouble()) putError("Invalid Transform.");
        
        translateX(atof(yytext), matrix);

        token = yylex();
        if(token == SEMI) {
          stop = true;
          break;
        }

        checkToken(COMMA, "Expected ';' or ','.");
        break;
      }

      case TRANSLATEY: {
        token = yylex();
        checkToken(COLON, "Expected ':'.");

        token = yylex();
        if(!isDouble()) putError("Invalid Transform.");
        
        translateY(atof(yytext), matrix);

        token = yylex();
        if(token == SEMI) {
          stop = true;
          break;
        }

        checkToken(COMMA, "Expected ';' or ','.");
        break; 
      }
      case TRANSLATEZ: {
        token = yylex();
        checkToken(COLON, "Expected ':'.");

        token = yylex();
        if(!isDouble()) putError("Invalid Transform.");
        
        translateZ(atof(yytext), matrix);

        token = yylex();
        if(token == SEMI) {
          stop = true;
          break;
        }

        checkToken(COMMA, "Expected ';' or ','.");
        break;
      }
      case SCALE: {
        token = yylex();
        checkToken(COLON, "Expected ':'.");

        token = yylex();
        if(!isDouble()) putError("Invalid Transform.");
        
        scale(atof(yytext), matrix);

        token = yylex();
        if(token == SEMI) {
          stop = true;
          break;
        }

        checkToken(COMMA, "Expected ';' or ','.");
        break;
      }
      default: {
        std::cout << "Invalid Transform Literal." << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }
}


void parseSphereData(ParseData &data) {
  checkToken(SPHERE, "What? How the hell did you get here? (SPHERE)");

  Sphere *sphere = new Sphere;

  token = yylex();
  checkToken(OPENBRACK, "Invlaid Sphere Data.");
  
  token = yylex();
  parseLabelData(sphere, data.primitives, 1);

  token = yylex();
  parseMaterialData(sphere);

  token = yylex();
  parseTransformData(sphere->mat);

  token = yylex();
  checkToken(CLOSEBRACK, "Expected '}'");

  data.primitives.push_back(sphere);
}

void parsePlaneData(ParseData &data) {
  checkToken(PLANE, "What? How the hell did you get here?(PLANE)");

  Plane *plane = new Plane;

  token = yylex();
  checkToken(OPENBRACK, "Invalid Plane Data.");
  
  token = yylex();
  parseLabelData(plane, data.primitives, 1);

  token = yylex();
  parseMaterialData(plane);

  token = yylex();
  parseTransformData(plane->mat);

  token = yylex();
  checkToken(CLOSEBRACK, "Expected '}'.");
  data.primitives.push_back(plane);
}

void parseMeshData(ParseData &data) {
  checkToken(MESH,"What? How the hell did you get here? (Mesh)");
  Mesh *mesh = new Mesh;

  token = yylex();
  checkToken(OPENBRACK, "Invalid Mesh Data.");
  
  token = yylex();
  parseLabelData(mesh, data.primitives, 1);

  token = yylex();
  parseMaterialData(mesh);

  token = yylex();
  parseTopologyData(mesh);

  parseTransformData(mesh->mat);

  token = yylex();
  checkToken(CLOSEBRACK, "Expeceted '}'.");
  data.primitives.push_back(mesh);
}

void parseImageData(ParseData &data) {
  checkToken(HEIGHT, "Invalid or Missing Height Data");

  token = yylex();
  checkToken(COLON, "Expected ':'.");

  token = yylex();
  checkToken(INT, "Invalid Height Data");

  data.height = atoi(yytext);

  token = yylex();
  checkToken(SEMI, "No terminating semicolon.");

  token = yylex();
  checkToken(WIDTH, "Invalid or Missing Width Data");

  token = yylex();
  checkToken(COLON, "Expected ':'.");

  token = yylex();
  checkToken(INT, "Invalid Width Data");

  data.width = atoi(yytext);

  token = yylex();
  checkToken(SEMI, "No terminating semicolon.");

  token = yylex();
  checkToken(FOV, "Invalid or Missing FOV Data");

  token = yylex();
  checkToken(COLON, "Expected ':'.");

  token = yylex();
  checkToken(INT, "Invalid FOV Data");

  data.fov = atoi(yytext);

  token = yylex();
  checkToken(SEMI, "No terminating semicolon.");

  token = yylex();
  checkToken(AMBIENT, "Invalid or Missing Ambient Data");

  token = yylex();
  checkToken(COLON, "Expected ':'.");

  token = yylex();
  if(!isDouble()) putError("Invalid Ambient Data.");

  data.ambient = atof(yytext);

  token = yylex();
  checkToken(SEMI, "No terminating semicolon.");
}

void parseCameraData(ParseData &data) {
  checkToken(CAMERA, "Invalid or Missing Camera Data");
  Camera *camera = new Camera;

  token = yylex();
  if(token == SEMI) {
    data.camera = camera;
    return;
  }  

  checkToken(OPENBRACK, "Expected ';' or '{'.");

  token = yylex();
  parseTransformData(camera->mat);

  token = yylex();
  checkToken(CLOSEBRACK, "Expected '}'.");
  
  data.camera = camera;
}

void parseLightData(ParseData &data) {
  Light *light;
  switch(token){
    case POINT: {
      light = new PointLight;
      break;
    }
    case DIRECTION: {
      light= new DirectionalLight;
      break;
    }
    defaul: {
      perror("Invalid Ligt Type.");
      exit(EXIT_FAILURE);
    }
  }

  light->material.diffuse = new int[3];

  token = yylex();
  checkToken(OPENBRACK, "Expected '{'.");

  token = yylex();
  parseLabelData(light, data.primitives, 0);
    
  token = yylex();
  checkToken(COLOR, "Expected Color Data to follow Label Data.");

  token = yylex();
  checkToken(COLON, "Expected ':'.");

  token = yylex();
  checkToken(OPENPAREN, "Invalid Color Data.");

  token = yylex();
  checkToken(INT, "Invalid Color Data.");

  light->material.diffuse[0] = atoi(yytext);

  token = yylex();
  checkToken(COMMA, "Invalid Color Data.");

  token = yylex();
  checkToken(INT, "Invalid Color Data.");

  light->material.diffuse[1] = atoi(yytext);

  token = yylex();
  checkToken(COMMA, "Invalid Color Data.");

  token = yylex();
  checkToken(INT, "Invalid Color Data.");

  light->material.diffuse[2] = atoi(yytext);

  token = yylex();
  checkToken(CLOSEPAREN, "Invalid Color Data.");

  token = yylex();
  checkToken(SEMI, "No terminating semicolon");

  token = yylex();
  checkToken(INTENSITY, "Expeted Intensity.");
  
  token = yylex();
  checkToken(COLON, "Expected ':'.");

  token = yylex();
  if(!isDouble()) putError("Invalid Intensity.");

  light->material.shine = atof(yytext);

  token = yylex();
  checkToken(SEMI, "No terminating semicolon.");

  token = yylex();
  parseTransformData(light->mat);
  
  token = yylex();
  checkToken(CLOSEBRACK, "Expected '}'.");

  data.lights.push_back(light);
}

void parseObjectData(ParseData &data) {
  switch(token) {
    case SPHERE: {
      parseSphereData(data);
      break;
    }
    case PLANE: {
      parsePlaneData(data);
      break;
    }
    case MESH: {
      parseMeshData(data);
      break;
    }
    default: {
      perror("Invalid Object Type");
      exit(EXIT_FAILURE);
    }
  }
}


void parseFile(char *filename, ParseData &data) {
  yyin = fopen(filename, "r");
  yyout = stdout;
  token = yylex();
  parseImageData(data);

  token = yylex();
  parseCameraData(data);

  token = yylex();
  while(token == POINT || token == DIRECTION){
    parseLightData(data);
    token = yylex();
  }

  while(token == SPHERE || token == PLANE || token == MESH) {
    parseObjectData(data);
    token = yylex();
  }

  token = yylex();
  checkToken(ENDOFFILE, "Excess data in file");
}

