// MyMeshTexturedTest.cpp
// -- test of textured mesh derived from TexturedMesh class.
// cs200 10/20
//
// To compile from the Visual Studio command prompt:
//   cl /EHsc MyTexturedMeshTest.cpp MyTexturedMesh.cpp Affine.lib
//   Texture.lib TextureRender.lib opengl32.lib glew32.lib SDL2.lib
//   SDL2main.lib /link /subsystem:console
//
// Note: the file 'textures.bmp' is assumed to exist.

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "MyTexturedMesh.h"
#include "Texture.h"
#include "Affine.h"
#include "TextureRender.h"
#include "SolidRender.h"
using namespace std;


const glm::vec4 O = cs200::point(0,0),
                BLACK(0,0,0,1);


class Client {
  public:
    Client(void);
    ~Client(void);
    void draw(double dt);
    void resize(int W, int H);
  private:
    cs200::SolidRender srender;
    cs200::TextureRender render;
    vector<glm::mat4> bg_obj2world;
    glm::mat4 fg_obj2world;
    float fg_rotation_rate;
};


Client::Client(void) {

  // load bitmap file
  cs200::Bitmap *texture = 0;
  try {
    texture = new cs200::Bitmap("textures.bmp");
    cs200::reverseRGB(*texture);
  }
  catch (exception &e) {
    cout << "failed to open 'textures.bmp'" << endl;
    cout << e.what() << endl;
    texture = new cs200::Bitmap(2046,764);
  }
  render.loadTexture(texture->data(),texture->width(),texture->height());
  delete texture;

  // load mesh
  cs200::MyTexturedMesh mesh;
  render.loadMesh(mesh);
  srender.loadMesh(mesh);

  // background object modeling transformations
  const int COUNT = 8;
  float aspect = mesh.dimensions().x/mesh.dimensions().y,
        dx = (aspect < 1) ? 2.0f/COUNT : 2.0f*aspect/COUNT,
        dy = (aspect < 1) ? 2.0f/COUNT/aspect : 2.0f/COUNT,
        mscale = (aspect < 1) ? 0.95f*dx/mesh.dimensions().x
                              : 0.95f*dy/mesh.dimensions().y;
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < COUNT; ++j) {
      float sign_x = (i%2 == 0) ? 1 : -1,
            sign_y = (j%2 == 0) ? 1 : -1;
      glm::mat4 obj2world = cs200::translate(cs200::vector((i+0.5f)*dx-1,(j+0.5f)*dy-1))
                            * cs200::scale(sign_x*mscale,sign_y*mscale)
                            * cs200::translate(O-mesh.center());
      bg_obj2world.push_back(obj2world);
    }
  }

  // foreground object modeling transformation (initial)
  fg_rotation_rate = 360.0f/5.0f;
  fg_obj2world = ((aspect < 1) ? cs200::scale(1.0f/mesh.dimensions().y)
                               : cs200::scale(1.0f/mesh.dimensions().x))
                 * cs200::translate(O-mesh.center());
}


Client::~Client(void) {
  render.unloadMesh();
  srender.unloadMesh();
  render.unloadTexture();
}


void Client::draw(double dt) {
  render.clearFrame(glm::vec4(1,0,1,1));

  // draw background objects
  for (unsigned i=0; i < bg_obj2world.size(); ++i) {
    render.setTransform(bg_obj2world[i]);
    render.displayFaces();
    srender.setTransform(bg_obj2world[i]);
    srender.displayEdges(BLACK);
  }

  // draw foreground object
  /*fg_obj2world = cs200::rotate(fg_rotation_rate*dt) * fg_obj2world;*/
  render.setTransform(fg_obj2world);
  render.displayFaces();
  srender.setTransform(fg_obj2world);
  srender.displayEdges(BLACK);
}


void Client::resize(int W, int H) {
  int D = min(W,H);
  glViewport(0,0,D,D);
}


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int , char *[]) {

  // SDL: initialize and create a window
  SDL_Init(SDL_INIT_VIDEO);
  const char *title = "CS 200: MyTextureMesh Test";
  int width = 600,
      height = 600;
  SDL_Window *window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,width,height,
                                        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  SDL_GLContext context = SDL_GL_CreateContext(window);

  // GLEW: get function bindings (if possible)
  glewInit();
  if (!GLEW_VERSION_2_0) {
    cout << "needs OpenGL version 2.0 or better" << endl;
    return -1;
  }

  // animation loop
  bool done = false;
  Client *client = new Client();
  Uint32 ticks_last = SDL_GetTicks();
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          done = true;
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE)
            done = true;
          break;
        case SDL_WINDOWEVENT:
          if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            client->resize(event.window.data1,event.window.data2);
          break;
      }
    }
    Uint32 ticks = SDL_GetTicks();
    double dt = 0.001*(ticks - ticks_last);
    ticks_last = ticks;
    client->draw(dt);
    SDL_GL_SwapWindow(window);
  }

  // clean up
  delete client;
  SDL_GL_DeleteContext(context);
  SDL_Quit();
  return 0;
}

