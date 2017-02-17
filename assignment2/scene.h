#pragma once
#include "loadObj.h"
#include <string>

class Object
{
public:
	string path;
	float rx, ry, rz;
	vec3 t;
	string shader;
	mat4 modelingMatrix;
};
class Scene
{
	public:
		colorValues colorVals;
		lightProperties lightVals[4];
		
		vec3 eye;
		vec3 center;
		vec3 viewUp;
		int numLightSources;
		vector<Object> objects;
		Scene();

};

class SceneParser
{
	public:
		Scene parseSceneFile(string fileName);
};
