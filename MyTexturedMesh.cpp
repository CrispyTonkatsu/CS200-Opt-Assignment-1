/**
 * @file: MyTexturedMesh
 * @author: Edgar Jose Donoso Mansilla
 * @course: CS200
 * @assignment: Opt-Assignment-1
 */

#include "MyTexturedMesh.h"
#include "Affine.h"

// How many peaks the star will have, any
static constexpr unsigned peak_count = 5;
static constexpr float spacing_angle =
    static_cast<float>(360) / static_cast<float>(peak_count);
// How far the spikes are supposed to
static constexpr float peak_distance = 3.f;
const glm::vec4 peak_location = glm::vec4(peak_distance, 0, 0, 1);
// The "radius" of the core
static constexpr float core_width = 2.f;
const glm::vec4 core_location =
    cs200::rotate(spacing_angle / 2) * glm::vec4(core_width, 0, 0, 1);
// Whether to display specific edges
const bool inner_edges = false;
const bool outer_edges = true;

// Image to display (bottom left is image (0,0))
static constexpr int image_x = 2;
static constexpr int image_y = 1;

namespace cs200 {
  MyTexturedMesh::MyTexturedMesh() {
    // Calculating the texture coordinate matrix
    const glm::mat4 texture_transform =
        // Positioning at image
        translate(vector(
            (1.f / 8.f) + (1.f / 4.f) * (image_x),
            (1.f / 4.f) + (1.f / 2.f) * (image_y))) *
        // Adding some padding room due to images not being aligned in a grid
        scale(0.85f) *
        // To individual image aspect ratio
        scale(0.25f, 0.5f) *
        // To unit square size
        scale(1 / (peak_distance * 2));

    // Adding the origin to the list
    vertices.emplace_back(center());
    texture_coordinates.emplace_back(texture_transform * vertices.back());

    // Adding the core vertices
    for (unsigned i = 1; i < peak_count + 1; i++) {
      glm::mat4 rotation = rotate(static_cast<float>(i) * spacing_angle);

      vertices.emplace_back(rotation * core_location);

      texture_coordinates.emplace_back(texture_transform * vertices.back());

      if (inner_edges) edges.emplace_back(0, i);

      if (i != 1 && inner_edges) edges.emplace_back(i - 1, i);

      faces.emplace_back(0, i, i + 1);
    }

    // Adding the last edge and face
    if (inner_edges) edges.emplace_back(1, peak_count);
    faces.emplace_back(0, 1, peak_count);

    // Adding the peak vertices
    for (unsigned i = 1; i < peak_count + 1; i++) {
      glm::mat4 rotation = rotate(static_cast<float>(i) * spacing_angle);

      vertices.emplace_back(rotation * peak_location);

      texture_coordinates.emplace_back(texture_transform * vertices.back());

      unsigned adjusted_index = peak_count + i;

      if (outer_edges) {
        if (i != 1) edges.emplace_back(i - 1, adjusted_index);
        edges.emplace_back(i, adjusted_index);
      }

      if (i != 1) faces.emplace_back(adjusted_index, i, i - 1);
    }

    // Adding the last edge and face
    if (outer_edges) edges.emplace_back(peak_count, peak_count + 1);
    faces.emplace_back(1, peak_count, peak_count + 1);
  }

  int MyTexturedMesh::vertexCount() const {
    return static_cast<int>(vertices.size());
  }

  const glm::vec4 *MyTexturedMesh::vertexArray() const { return &vertices[0]; }

  int MyTexturedMesh::edgeCount() const {
    return static_cast<int>(edges.size());
  }

  const MyTexturedMesh::Edge *MyTexturedMesh::edgeArray() const {
    return &edges[0];
  }

  int MyTexturedMesh::faceCount() const {
    return static_cast<int>(faces.size());
  }

  const MyTexturedMesh::Face *MyTexturedMesh::faceArray() const {
    return &faces[0];
  }

  glm::vec4 MyTexturedMesh::dimensions() const {
    return vector(peak_distance * 2, peak_distance * 2);
  }

  glm::vec4 MyTexturedMesh::center() const { return point(0, 0); }

  const glm::vec2 *MyTexturedMesh::texcoordArray() const {
    return &texture_coordinates[0];
  }
  const char *MyTexturedMesh::textureFileName() const {
    return texture_name.c_str();
  }
} // namespace cs200
