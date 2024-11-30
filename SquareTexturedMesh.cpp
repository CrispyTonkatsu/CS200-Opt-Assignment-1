// SquareTexturedMesh.cpp
// cs200 10/20

#include "SquareTexturedMesh.h"


const glm::vec4 cs200::SquareTexturedMesh::vertices[4]
  = { point(-1,-1),  // 0
      point(1,-1),   // 1
      point(1,1),    // 2
      point(-1,1)    // 3
    };


const glm::vec2 cs200::SquareTexturedMesh::texcoords[4]
  = { glm::vec2(0,0), // 0
      glm::vec2(1,0), // 1
      glm::vec2(1,1), // 2
      glm::vec2(0,1)  // 3
    };


const cs200::Mesh::Face cs200::SquareTexturedMesh::faces[2]
  = { Face(0,1,2),  Face(0,2,3) };


const cs200::Mesh::Edge cs200::SquareTexturedMesh::edges[4]
  = { Edge(0,1),  Edge(1,2),  Edge(2,3),  Edge(3,0) };


const char *cs200::SquareTexturedMesh::textureFileName(void) const {
  const char *fname = "texture.bmp";
  return fname;
}


