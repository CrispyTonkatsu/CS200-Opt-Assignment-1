/**
 * @file: MyTexturedMesh
 * @author: Edgar Jose Donoso Mansilla
 * @course: CS200
 * @assignment: Opt-Assignment-1
 */

#include <string>
#include <vector>
#include "Affine.h"
#include "TexturedMesh.h"

namespace cs200 {
  class MyTexturedMesh final : public TexturedMesh {
  public:
    MyTexturedMesh();

    // Vertex Functions
    int vertexCount() const override;
    const glm::vec4 *vertexArray() const override;

    // Edge Functions
    int edgeCount() const override;
    const Edge *edgeArray() const override;

    // Face Functions
    int faceCount() const override;
    const Face *faceArray() const override;

    // Dimension functions
    glm::vec4 dimensions() const override;
    glm::vec4 center() const override;

    // Texture Coordinate functions
    const glm::vec2 *texcoordArray() const override;
    const char *textureFileName() const override;

  private:
    std::vector<glm::vec4> vertices;
    std::vector<Edge> edges;
    std::vector<Face> faces;

    std::string texture_name;
    std::vector<glm::vec2> texture_coordinates;
  };

} // namespace cs200
