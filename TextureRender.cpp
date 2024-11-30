/**
 * File: TextureRender.cpp
 * Name: Edgar Jose Donoso Mansilla (e.donosomansilla)
 * Assignment: 6
 * Course: CS200
 * Term: FALL24
 */

#include "TextureRender.h"
#include <stdexcept>

static const GLchar *const fragment_shader_source = "#version 130\n\
     uniform sampler2D usamp;\
     in vec2 vtexcoord;\
     out vec4 frag_color;\
     void main(void) {\
       frag_color = texture(usamp, vtexcoord);\
     }";

static const GLchar *const vertex_shader_source = "#version 130\n\
     in vec4 position;\
     in vec2 texcoord;\
     uniform mat4 transform;\
     out vec2 vtexcoord;\
     void main() {\
       gl_Position = transform * position;\
       vtexcoord = texcoord;\
     }";

namespace cs200 {
  static GLuint
  compile_shader(const GLchar *const shader_source, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
      GLint log_length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

      std::string error_log;
      error_log.reserve(log_length);
      glGetShaderInfoLog(shader, log_length, nullptr, &error_log[0]);

      glDeleteShader(shader);

      throw std::runtime_error(error_log.data());
    }
    return shader;
  }
} // namespace cs200

cs200::TextureRender::TextureRender() :
    program(0), texture_buffer(0), vertex_buffer(0), texcoord_buffer(0),
    face_buffer(0), vao(0), utransform(0), mesh_face_count(0) {
  GLuint fragment_shader_id =
      cs200::compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
  GLuint vertex_shader_id =
      cs200::compile_shader(vertex_shader_source, GL_VERTEX_SHADER);

  program = glCreateProgram(); // NOLINT

  glAttachShader(program, fragment_shader_id);
  glAttachShader(program, vertex_shader_id);

  glLinkProgram(program);
  GLint link_ok = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (link_ok == GL_FALSE) {
    GLint log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

    std::string error_log;
    error_log.reserve(log_length);
    glGetProgramInfoLog(program, log_length, &log_length, &error_log[0]);

    glDeleteProgram(program);
    glDeleteShader(fragment_shader_id);
    glDeleteShader(vertex_shader_id);

    throw std::runtime_error(error_log.data());
  }

  glDeleteShader(fragment_shader_id);
  glDeleteShader(vertex_shader_id);

  utransform = glGetUniformLocation(program, "transform");
}

cs200::TextureRender::~TextureRender() { glDeleteProgram(program); }

void cs200::TextureRender::clearFrame(const glm::vec4 &c) {
  glClearColor(c.x, c.y, c.z, c.w); // NOLINT
  glClear(GL_COLOR_BUFFER_BIT);
}

void cs200::TextureRender::loadTexture(
    unsigned char *rgbdata, int width, int height) {
  glGenTextures(1, &texture_buffer);
  glBindTexture(GL_TEXTURE_2D, texture_buffer);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGB,
      width,
      height,
      0,
      GL_RGB,
      GL_UNSIGNED_BYTE,
      rgbdata);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void cs200::TextureRender::unloadTexture() {
  glUseProgram(0);

  if (texture_buffer != 0) {
    glDeleteTextures(1, &texture_buffer);
    texture_buffer = 0;
  }
}

void cs200::TextureRender::setTransform(const glm::mat4 &M) {
  glUseProgram(program);
  glUniformMatrix4fv(utransform, 1, false, &M[0][0]);
}

void cs200::TextureRender::loadMesh(const TexturedMesh &m) {
  // Updating the mesh count variables needed here
  mesh_face_count = m.faceCount();

  // Binding vertex buffer
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  // Sending data to GPU
  glBufferData(
      GL_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(m.vertexCount() * sizeof(glm::vec4)),
      m.vertexArray(),
      GL_STATIC_DRAW);

  // Binding Texture Coords Buffer
  glGenBuffers(1, &texcoord_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
  glBufferData(
      GL_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(m.vertexCount() * sizeof(glm::vec2)),
      m.texcoordArray(),
      GL_STATIC_DRAW);

  // Binding face buffer
  glGenBuffers(1, &face_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_buffer);
  // Sending data to GPU
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(m.faceCount() * sizeof(Mesh::Face)),
      m.faceArray(),
      GL_STATIC_DRAW);

  // Getting the attribute for the VAOs
  GLint position_attribute = glGetAttribLocation(program, "position");
  GLint tex_coord_attribute = glGetAttribLocation(program, "texcoord");

  /// Creating the VAO for the faces & texture coordinates ///
  glGenVertexArrays(1, &vao);

  // Recording start
  glBindVertexArray(vao);

  // Binding the vertices, enabling attributes and binding the edges as elements
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glVertexAttribPointer(position_attribute, 4, GL_FLOAT, false, 0, nullptr);
  glEnableVertexAttribArray(position_attribute);
  glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
  glVertexAttribPointer(tex_coord_attribute, 2, GL_FLOAT, false, 0, nullptr);
  glEnableVertexAttribArray(tex_coord_attribute);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_buffer);

  // Recording end
  glBindVertexArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void cs200::TextureRender::unloadMesh() {
  glUseProgram(0);

  if (vao != 0) {
    glDeleteVertexArrays(1, &vao);
    vao = 0;
  }

  if (face_buffer != 0) {
    glDeleteBuffers(1, &face_buffer);
    face_buffer = 0;
  }

  if (vertex_buffer != 0) {
    glDeleteBuffers(1, &vertex_buffer);
    vertex_buffer = 0;
  }

  if (texcoord_buffer != 0) {
    glDeleteBuffers(1, &texcoord_buffer);
    texcoord_buffer = 0;
  }

  mesh_face_count = 0;
}

void cs200::TextureRender::displayFaces() {
  glUseProgram(program);

  glBindTexture(GL_TEXTURE_2D, texture_buffer);

  glBindVertexArray(vao);

  glDrawElements(GL_TRIANGLES, mesh_face_count * 3, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
}
