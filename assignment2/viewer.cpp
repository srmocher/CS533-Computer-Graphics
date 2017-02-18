// assignment1.cpp : CSC 433/533 assignment 1 - Spring 2017.
//	based on the 9th edition of the OpenGL Programming Guide (red book)
//
#include "vgl.h"

#include "LoadShaders.h"
#include "loadObj.h"
#include "scene.h"
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

objInfo *objInfos;
int objCount;
vector<objInfo> objectInfos;
vector<Object> sceneObjects;
mat4 viewingMatrix;
Scene scene;
vec3 initialEye, initialCenter, initialViewUp;
bool wireframe = false;

void setLightProperties(objInfo info, mat4 mvMatrix);
//----------------------------------------------------------------------------
//
// init
//

void init(void) {
	//vec3 eye(150, 150, 75), center(0, 0, 0), viewUp(0, 0, 1);
	//mat4 view = lookAt(eye, center, viewUp);
	//vec4 point(-19.506435, 30.299948, -37.876911, 1);
	////vec4 res = result*point;
	//int rx = 90;
	//vec3 axis(1, 0, 0);
	//mat4 rotation = glm::rotate(mat4(1.0), 90.0f*3.14159f / 180, axis);

	//ShaderInfo shaders[] = { {GL_VERTEX_SHADER,"BlinnPhong.vert"},
	//{GL_FRAGMENT_SHADER,"triangles.frag"},{GL_NONE,NULL} };

	//GLuint program = LoadShaders(shaders);
	//glUseProgram(program);
	////GLuint loc1 = glGetUniformLocation(program, "MVMatrix");
	//GLuint loc2 = glGetUniformLocation(program, "MVMatrix");
	//loadObjFile("OBJfiles/teapot.obj", &objects, &objCount);
	//mat4 modelingMatrix = objects->modelMatrix;
	//float *bounds = objects->bounds;
	//mat4 proj = frustum(bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5]);
	SceneParser parser;
	scene = parser.parseSceneFile("teapotScene.txt");
	initialEye = scene.eye;
	initialCenter = scene.center;
	initialViewUp = scene.viewUp;
	cout << "Center "<<scene.center.x<<","<<scene.center.y<<","<<scene.center.z<<endl;
	cout << "Eye " << scene.eye.x << "," << scene.eye.y << "," << scene.eye.z << endl;
	cout << "Num light sources :" << scene.numLightSources << endl;
	cout << "Num objects " << scene.objects.size() << endl;
	sceneObjects = scene.objects;
	vec3 eye = scene.eye;
	vec3 center = scene.center;
	vec3 viewUp = scene.viewUp;
	viewingMatrix = glm::lookAt(eye, center, viewUp);
	ShaderInfo shaders[] = { {GL_VERTEX_SHADER,"BlinnPhong.vert"},
	{GL_FRAGMENT_SHADER,"BlinnPhong.frag"},{GL_NONE,NULL} };
	
	program = LoadShaders(shaders);
	lightProperties props[4];
	int numLightSources = scene.numLightSources;
	
	
	for (auto i = 0;i < sceneObjects.size();i++)
	{
		int num;
		char *fileName = &sceneObjects[i].path[0];
		loadObjFile(fileName, &objInfos, &num);
		for (int j = 0;j < num;j++) {
			objInfos[j].modelMatrix = sceneObjects[i].modelingMatrix;
			objectInfos.push_back(objInfos[j]);
		}
		
		
	}

}
	
void printMatrix(mat4 matrix)
{
	cout << "Matrix is " << endl;
	for (int i = 0;i < 4;i++)
	{
		for (int j = 0;j < 4;j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

//----------------------------------------------------------------------------
//
// display
//

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	float black[] = { 0.227, 0.474, 0.501,0.0 };
	glClearBufferfv(GL_COLOR, 0, black);
	glDepthMask(GL_TRUE);
	glUseProgram(program);
	
	mat4 projectionMatrix = glm::frustum(-1.0f, 1.f, -1.f, 1.0f, 1.5f, 3200.0f);	
	for (int i = 0;i < scene.numLightSources;i++)
	{
		lightProperties temp = scene.lightVals[i];

		vec4 tempVec(temp.position, 1.0);
		vec4 t = viewingMatrix*tempVec;
		t = t / t.w;

		scene.lightVals[i].position = vec3(t.x, t.y, t.z);
	}
	for (auto i = 0;i < objectInfos.size();i++)
	{
		objInfo info = objectInfos[i];
		
		mat4 modelingMatrix = info.modelMatrix;
		mat4 mvMatrix = viewingMatrix*modelingMatrix;
		glBindVertexArray(info.VAO);
		setLightProperties(info, mvMatrix);
		//printMatrix(mvMatrix);
		
		GLuint loc1 = glGetUniformLocation(program, "MVMatrix");	
		glUniformMatrix4fv(loc1, 1, false, glm::value_ptr(mvMatrix));	

		GLuint location = glGetUniformLocation(program, "NormalMatrix");
		mat3 normalMatrix = mat3(mvMatrix);
		glUniformMatrix3fv(location, 1, false, glm::value_ptr(normalMatrix));
		
		loc1 = glGetUniformLocation(program, "ambient");
		glUniform3fv(loc1, 1, glm::value_ptr(info.Ka));

		loc1 = glGetUniformLocation(program, "diffuse");
		glUniform3fv(loc1, 1, glm::value_ptr(info.Kd));

		loc1 = glGetUniformLocation(program, "specular");
		glUniform3fv(loc1, 1, glm::value_ptr(info.Ks));

		loc1 = glGetUniformLocation(program, "shininess");
		glUniform1f(loc1, info.n);		
		
		//printMatrix(projectionMatrix);
		mat4 mvpMatrix = projectionMatrix*viewingMatrix*modelingMatrix;
		
		loc1 = glGetUniformLocation(program, "MVPMatrix");
		glUniformMatrix4fv(loc1, 1, false, glm::value_ptr(mvpMatrix));
		
		glDrawArrays(GL_TRIANGLES, 0, info.VAOsize);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << error << std::endl;
			break;
		}

	}
}

void setLightProperties(objInfo info,mat4 mvMatrix)
{
	mat3 normalMatrix = glm::transpose(glm::inverse(mat3(mvMatrix)));
	for (int i = 0;i < scene.numLightSources;i++)
	{
		lightProperties props = scene.lightVals[i];
		char buff[100];
		props.isEnabled = 1;
		sprintf(buff, "Lights[%1d].isEnabled", i);		
		GLuint loc = glGetUniformLocation(program, buff);
		glUniform1i(loc, props.isEnabled);		

		sprintf(buff, "Lights[%1d].isLocal", i);
		loc = glGetUniformLocation(program, buff);
		glUniform1i(loc, props.isLocal);

		sprintf(buff, "Lights[%1d].isSpot", i);
		loc = glGetUniformLocation(program, buff);
		glUniform1i(loc, props.isSpot);

		sprintf(buff, "Lights[%1d].ambient", i);
		loc = glGetUniformLocation(program, buff);
		glUniform3fv(loc, 1, glm::value_ptr(props.ambient));

		sprintf(buff, "Lights[%1d].color", i);
		loc = glGetUniformLocation(program, buff);
		glUniform3fv(loc, 1, glm::value_ptr(props.color));

		//vec3 temp = vec3(t.x, t.y, t.z);
		sprintf(buff, "Lights[%1d].position", i);
		loc = glGetUniformLocation(program, buff);
		glUniform3fv(loc, 1, glm::value_ptr(props.position));

		sprintf(buff, "Lights[%1d].halfVector", i);
		loc = glGetUniformLocation(program, buff);
		glUniform3fv(loc, 1, glm::value_ptr(props.halfVector));

		sprintf(buff, "Lights[%1d].coneDirection", i);
		loc = glGetUniformLocation(program, buff);
		props.coneDirection = vec3(mvMatrix * vec4(props.coneDirection, 0.0));
		glUniform3fv(loc, 1, glm::value_ptr(props.coneDirection));

		sprintf(buff, "Lights[%1d].spotCosCutoff", i);
		loc = glGetUniformLocation(program, buff);
		glUniform1f(loc, props.spotCosCutoff);

		sprintf(buff, "Lights[%1d].spotExponent", i);
		loc = glGetUniformLocation(program, buff);
		glUniform1f(loc, props.spotExponent);

		sprintf(buff, "Lights[%1d].constantAttenuation", i);
		loc = glGetUniformLocation(program, buff);
		glUniform1f(loc, props.constantAttenuation);

		sprintf(buff, "Lights[%1d].linearAttenuation", i);
		loc = glGetUniformLocation(program, buff);
		glUniform1f(loc, props.linearAttenuation);

		sprintf(buff, "Lights[%1d].quadraticAttenuation", i);
		loc = glGetUniformLocation(program, buff);
		glUniform1f(loc, props.quadraticAttenuation);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W || key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		vec3 gazeVector = scene.center - scene.eye;
		scene.eye = scene.eye + 0.1f * gazeVector;
		scene.center = scene.center + 0.1f*gazeVector;
		viewingMatrix = glm::lookAt(scene.eye, scene.center, scene.viewUp);

	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		vec3 gaze = -(scene.center - scene.eye);
		vec3 u = glm::cross(scene.viewUp, gaze);
		mat4 tm = glm::translate(mat4(1.0f), u);
		scene.center = scene.center + 0.2f*u;
		scene.eye = scene.eye + 0.2f*u;
		viewingMatrix = glm::lookAt(scene.eye, scene.center, scene.viewUp);
	}
	if (key == GLFW_KEY_S || key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		vec3 gazeVector = scene.center - scene.eye;
		scene.eye = scene.eye - 0.1f * gazeVector;
		scene.center = scene.center - 0.1f*gazeVector;
		viewingMatrix = glm::lookAt(scene.eye, scene.center, scene.viewUp);
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		vec3 gaze = -(scene.center - scene.eye);
		vec3 u = glm::cross(scene.viewUp, gaze);
		scene.center = scene.center - 0.2f*u;
		
		scene.eye = scene.eye - 0.2f*u;
		viewingMatrix = glm::lookAt(scene.eye, scene.center, scene.viewUp);
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		scene.eye = initialEye;
		scene.center = initialCenter;
		scene.viewUp = initialViewUp;
		viewingMatrix = glm::lookAt(scene.eye, scene.center, scene.viewUp);
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		if (!wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframe = true;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframe = false;
		}
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		exit(1);
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		exit(1);
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		mat4 rm = glm::rotate(mat4(1.0), -1.0f*3.14159f / 180.0f, scene.viewUp);
		scene.center = vec3(rm*vec4(scene.center, 1.0f));
		scene.eye = vec3(rm*vec4(scene.eye, 1.0f));
		viewingMatrix = glm::lookAt(scene.eye, scene.center, scene.viewUp);
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		mat4 rm = glm::rotate(mat4(1.0), 1.0f*3.14159f / 180.0f, scene.viewUp);
		scene.center = vec3(rm*vec4(scene.center, 1.0f));
		scene.eye = vec3(rm*vec4(scene.eye, 1.0f));
		viewingMatrix = glm::lookAt(scene.eye, scene.center, scene.viewUp);
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		mat4 rm = glm::rotate(mat4(1.0), 1.0f*3.14159f / 180.0f, scene.center - scene.eye);
		scene.viewUp = vec3(rm*vec4(scene.viewUp,1.0f));
		viewingMatrix = glm::lookAt(scene.eye, scene.center, scene.viewUp);
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		mat4 rm = glm::rotate(mat4(1.0), -1.0f*3.14159f / 180.0f, scene.center - scene.eye);
		scene.viewUp = vec3(rm*vec4(scene.viewUp, 1.0f));
		viewingMatrix = glm::lookAt(scene.eye, scene.center, scene.viewUp);
	}
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
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 4.1 latest version of OpenGL for OSX 10.9 Mavericks
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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
  glfwSetKeyCallback(window, key_callback);
 /* if (argc != 1)
  {
	  cout << "Enter fileName as parameter" << endl;
  }*/
  init();

  while (!glfwWindowShouldClose(window)) {
    display();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
}
