// SquareTexturedMesh.h
// -- textured standard square
// cs200 10/20

#ifndef CS200_SQUARETEXTUREDMESH_H
#define CS200_SQUARETEXTUREDMESH_H

#include "TexturedMesh.h"
#include "Affine.h"


namespace cs200 {

  class SquareTexturedMesh : public TexturedMesh {
    public:
      int vertexCount(void) const;
      const glm::vec4* vertexArray(void) const;
      glm::vec4 dimensions(void) const;
      glm::vec4 center(void) const;
      int faceCount(void) const;
      const Face* faceArray(void) const;
      int edgeCount(void) const;
      const Edge* edgeArray(void) const;
      const glm::vec2* texcoordArray(void) const;
      const char *textureFileName(void) const;
    private:
      static const glm::vec4 vertices[4];
      static const glm::vec2 texcoords[4];
      static const Face faces[2];
      static const Edge edges[4];
  };

}


inline
int cs200::SquareTexturedMesh::vertexCount(void) const {
  return 4;
}


inline
const glm::vec4* cs200::SquareTexturedMesh::vertexArray(void) const {
  return vertices;
}


inline
glm::vec4 cs200::SquareTexturedMesh::dimensions(void) const {
  return cs200::vector(2,2);
}


inline
glm::vec4 cs200::SquareTexturedMesh::center(void) const {
  return cs200::point(0,0);
}


inline
int cs200::SquareTexturedMesh::faceCount(void) const {
  return 2;
}


inline
const cs200::Mesh::Face* cs200::SquareTexturedMesh::faceArray(void) const {
  return faces;
}


inline
int cs200::SquareTexturedMesh::edgeCount(void) const {
  return 4;
}


inline
const cs200::Mesh::Edge* cs200::SquareTexturedMesh::edgeArray(void) const {
  return edges;
}


inline
const glm::vec2* cs200::SquareTexturedMesh::texcoordArray(void) const {
  return texcoords;
}


#endif

