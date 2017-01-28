// assignment1.cpp : CSC 433/533 assignment 1 - Spring 2017.
//	based on the 9th edition of the OpenGL Programming Guide (red book)
//
#include "vgl.h"

#include "LoadShaders.h"

#include <iostream>
using namespace std;

#ifdef __APPLE__
#include <cmath>
#endif

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

GLuint program;

//----------------------------------------------------------------------------
//
// init
//

void init(void) {

  GLfloat vertices[NumVertices][2] = {
      {-0.90f, -0.90f}, {0.85f, -0.90f}, {-0.90f, 0.85f}, // Triangle 1
      {0.90f, -0.85f},  {0.90f, 0.90f},  {-0.85f, 0.90f}  // Triangle 2
  };

  glGenVertexArrays(NumVAOs, VAOs);
  glBindVertexArray(VAOs[Triangles]);

  // glCreateBuffers(NumBuffers, Buffers);		//OpenGL 4.5 method
  glGenBuffers(NumBuffers, Buffers);
  // glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);
  // OpenGL 4.5 method
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]); // ArrayBuffer is 0
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(0)); // vPosition is 0
  glEnableVertexAttribArray(vPosition);

  ShaderInfo shaders[] = {{GL_VERTEX_SHADER, "triangles.vert"},
                          {GL_FRAGMENT_SHADER, "triangles.frag"},
                          {GL_NONE, NULL}};

  program = LoadShaders(shaders);
  if (program == NULL) {
    cerr << "Failed shader load" << endl;
  }
  glUseProgram(program);
}

//----------------------------------------------------------------------------
//
// display
//

void display(void) {
  static const float black[] = {0.0f, 0.0f, 0.0f, 0.0f};

  glClearBufferfv(GL_COLOR, 0, black);

  glUseProgram(program);
  glBindVertexArray(VAOs[Triangles]);
  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

//----------------------------------------------------------------------------
//
// main
//

#undef _WIN32
#ifdef _WIN32
int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
  glfwInit();

#ifdef __APPLE__
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 4.1 latest version of OpenGL for OSX 10.9 Mavericks
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  GLFWwindow *window = glfwCreateWindow(800, 800, "Triangles", NULL, NULL);

  glfwMakeContextCurrent(window);

  gl3wInit();

#define CHECK_VERSION
#ifdef CHECK_VERSION
  // code from OpenGL 4 Shading Language cookbook, second edition
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *vendor = glGetString(GL_VENDOR);
  const GLubyte *version = glGetString(GL_VERSION);
  const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
  GLint major, minor;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);

  cout << "GL Vendor            :" << vendor << endl;
  cout << "GL Renderer          :" << renderer << endl;
  cout << "GL Version (string)  :" << version << endl;
  cout << "GL Version (integer) :" << major << " " << minor << endl;
  cout << "GLSL Version         :" << glslVersion << endl;
  cout << "major version: " << major << "  minor version: " << minor << endl;
#endif

  init();

  while (!glfwWindowShouldClose(window)) {
    display();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
}
