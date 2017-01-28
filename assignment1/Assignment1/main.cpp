// assignment1.cpp : CSC 433/533 assignment 1 - Spring 2017.
//	based on the 9th edition of the OpenGL Programming Guide (red book)
//
#include "vgl.h"
#include "vertex_metadata.h"
#include "LoadShaders.h"

#include <iostream>
#include "circle.h"
#include "heart.h"
using namespace std;

#ifdef __APPLE__
#include <cmath>
#endif

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[1];
GLuint Buffers[1];

GLuint circVAOs[1];
GLuint circBuffers[1];

GLuint heartVAOs[1];
GLuint heartBuffers[1];

GLuint triangleVAOs[2];
GLuint triangleBuffers[2];

const GLuint NumVertices = 6;
bool isWireframe = false, circle = false, heart=false, twoTriangles = true,singleTriangle = true;

GLuint trianglesProgram, circlesProgram, heartProgram, triangleProgram;
int numPointsCircle, numPointsHeart;
void setupTriangles();
void setupCircle(double radius,int steps);
void setupHeart(int steps);
void setupTriangle();
void drawTriangle();
void drawTriangles();
void drawCircle();
void drawHeart();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void readAndSetColor();

//----------------------------------------------------------------------------
//
// init
//

void init(void) {
  setupTriangles();
 
  
  setupTriangle();
}


void setupCircle(double radius, int steps) {
	Circle c(radius, steps);
	GLfloat** points = c.generatePoints();
	numPointsCircle = c.getNumberOfPoints();
	ShaderInfo shaders[] = { {GL_VERTEX_SHADER,"circle.vert"},
	{GL_FRAGMENT_SHADER,"circle.frag"},
	{GL_NONE,NULL} };

	Point *pts = new Point[numPointsCircle];
	for (auto i = 0;i < numPointsCircle;i++) {
		pts[i].coord[0] = points[i][0];
		pts[i].coord[1] = points[i][1];
	}
	
	circlesProgram = LoadShaders(shaders);
	if (circlesProgram == NULL) {
		cout << "Failed to load circle shaders" << endl;
	}
	glUseProgram(circlesProgram);
	float color[] = { 0.0f,0.0f,1.0f,1.0f };
	GLint loc = glGetUniformLocation(circlesProgram, "inputColor");
	if (loc != -1) 
	{
		glUniform4fv(loc, 1, color);
	}
	glGenVertexArrays(1, circVAOs);
	glBindVertexArray(circVAOs[0]);


	glGenBuffers(1, circBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, circBuffers[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Point)*numPointsCircle, pts, GL_STATIC_DRAW);
	
	
}	
void setupTriangles() {

	ShaderInfo shaders[] = { { GL_VERTEX_SHADER, "triangles.vert" },
	{ GL_FRAGMENT_SHADER, "triangles.frag" },
	{ GL_NONE, NULL } };

	trianglesProgram = LoadShaders(shaders);
	if (trianglesProgram == NULL) {
		cout << "Triangles shaders load failed" << endl;
	}
	GLfloat vertices[NumVertices][2] = {
		{ -0.90f, -0.90f },{ 0.85f, -0.90f },{ -0.90f, 0.85f }, // Triangle 1
		{ 0.90f, -0.85f },{ 0.90f, 0.90f },{ -0.85f, 0.90f }  // Triangle 2
	};
	glUseProgram(trianglesProgram);
	GLint loc = glGetUniformLocation(trianglesProgram, "inputColor");
	float color[] = { 0.0f,0.0f,1.0f,1.0f };
	if (loc != -1)
	{
		glUniform4fv(loc, 1, color);
	}
	glGenVertexArrays(1, VAOs);
	glBindVertexArray(VAOs[0]);

	// glCreateBuffers(NumBuffers, Buffers);		//OpenGL 4.5 method
	glGenBuffers(NumBuffers, Buffers);
	// glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);
	// OpenGL 4.5 method
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]); // ArrayBuffer is 0
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0)); // vPosition is 0
	glEnableVertexAttribArray(vPosition);

}
void setupHeart(int steps) {
	Heart heart(steps);
	Point* points = heart.generatePoints();
	numPointsHeart = heart.getNumberOfPoints();
	ShaderInfo shaders[] = { {GL_VERTEX_SHADER,"heart.vert"},{GL_FRAGMENT_SHADER,"heart.frag"},{GL_NONE,NULL} };
	heartProgram = LoadShaders(shaders);

	if (heartProgram == NULL) {
		cout << "Failed to load heart shaders" << endl;
	}
	for (auto i = 0;i < numPointsHeart;i++) {
		cout << "Point :(" << points[i].coord[0] << "," << points[i].coord[1] << ")" << endl;
	}
	
	glGenVertexArrays(1, heartVAOs);
	glGenBuffers(1, heartBuffers);
	
	glBindVertexArray(heartVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, heartBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point)*numPointsHeart, points, GL_STATIC_DRAW);
	
	
}

void setupTriangle() {
	ShaderInfo shaders[] = { {GL_VERTEX_SHADER,"triangle.vert"},
	{GL_FRAGMENT_SHADER,"triangle.frag"},
	{GL_NONE,NULL} };

	triangleProgram = LoadShaders(shaders);
	Point points[] = {  { 0.0f,  0.50f },
						{ 0.50f,  -0.50f },
					    { -0.50f,  -0.50f }};
	Color colors[] = { {1.0f,0.0f,0.0f,1.0f}
					   ,{0.0f,1.0f,0.0f,1.0f},
						{0.0f,0.0f,1.0f,1.0f} };
	glGenVertexArrays(1, triangleVAOs);
	glGenBuffers(2, triangleBuffers);
	
	glBindVertexArray(triangleVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, triangleBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Point), points, GL_STATIC_DRAW);

	
	glBindBuffer(GL_ARRAY_BUFFER, triangleBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Color), colors, GL_STATIC_DRAW);
	glBindVertexArray(0);
}
//----------------------------------------------------------------------------
//
// display
//

void display(void) {
  static const float black[] = {0.0f, 0.0f, 0.0f, 0.0f};

  glClearBufferfv(GL_COLOR, 0, black);

  if (isWireframe) {
	  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	  glLineWidth(5.0f);
  }
  else {
	  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
 
  if (twoTriangles) {
	  drawTriangles();
  }

  if (singleTriangle) {
	  drawTriangle();
  }

 

  if (circle) {
	  drawCircle();
  }
  if (heart) {
	  drawHeart();
  }
  //drawCircle();
 // drawHeart();

}

void drawTriangle() {
	glUseProgram(triangleProgram);

	glBindVertexArray(triangleVAOs[0]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, triangleBuffers[0]);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,sizeof(Point), BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, triangleBuffers[1]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Color), BUFFER_OFFSET(0));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	
}

void drawCircle() {
	glUseProgram(circlesProgram);

	glBindVertexArray(circVAOs[0]);
	

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, circBuffers[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point),
		BUFFER_OFFSET(0));
	glDrawArrays(GL_TRIANGLE_FAN, 0, numPointsCircle);
}

void drawHeart() {
	glUseProgram(heartProgram);
	glBindVertexArray(heartVAOs[0]);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, heartBuffers[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point),
	 BUFFER_OFFSET(0));
	glDrawArrays(GL_LINE_STRIP, 0, numPointsHeart);
}

void drawTriangles() {
	glUseProgram(trianglesProgram);
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

  GLFWwindow *window = glfwCreateWindow(800, 800, "Assignment 1", NULL, NULL);

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
  glfwSetKeyCallback(window, key_callback);

  while (!glfwWindowShouldClose(window)) {
    display();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		readAndSetColor();
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		//render shaded
		isWireframe = false;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		//render shaded
		isWireframe = true;
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		cout << "Enter the radius and number of steps for the circle: ";
		double radius;
		int steps;
		cin >> radius >> steps;
		if (radius >= 0 && radius <= 1.0 && steps >= 0) {
			setupCircle(radius, steps);
			circle = true;
		}
		else 
		{
			cout << "Invalid input values. Please enter a radius less than 1 and number of steps > 0" << endl;
		}
	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		cout << "Enter the number of steps for the heart: ";
		int steps;
		cin >> steps;
		if (steps > 0) 
		{
			setupHeart(steps);
			heart = true;
		}
		else 
		{
			cout << "Invalid input. Enter number of steps greater than zero" << endl;
		}
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		twoTriangles = !twoTriangles;
	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		singleTriangle = !singleTriangle;
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		circle = !circle;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		heart = !heart;
	}
	if ((key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS)
	{
		exit(0);
	}
}

void readAndSetColor() {
	cout << "Enter the color values for the two triangles and the circle: ";
	GLfloat r, g, b;
	cin >> r >> g >> b;
	if (r >= 0 && g >= 0 && b >= 0 && r <= 1 && g <= 1 && b <= 1) {
		glUseProgram(trianglesProgram);
		float color[] = { r,g,b,1.0f };
		GLint loc = glGetUniformLocation(trianglesProgram, "inputColor");
		if (loc != -1) {
			glUniform4fv(loc, 1, color);
		}

		if (circlesProgram != NULL) {
			glUseProgram(circlesProgram);
			loc = glGetUniformLocation(circlesProgram, "inputColor");
			if (loc != -1) {
				glUniform4fv(loc, 1, color);
			}
		}

		if (heartProgram != NULL) {
			glUseProgram(heartProgram);
			loc = glGetUniformLocation(heartProgram, "inputColor");
			if (loc != -1) {
				glUniform4fv(loc, 1, color);
			}
		}
	}
	else
	{
		cout << "Invalid color values. Please enter RGB values in [0,1] range" << endl;
	}
}