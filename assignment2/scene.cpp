#include "scene.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <iterator>

Scene::Scene()
{
	numLightSources = 0;
}
std::vector<std::string> split(std::string const &input) {
	std::istringstream buffer(input);
	std::vector<std::string> ret;

	std::copy(std::istream_iterator<std::string>(buffer),
		std::istream_iterator<std::string>(),
		std::back_inserter(ret));
	return ret;
}


vec3 getVec3FromStrings(vector<string> vals)
{
	vec3 value;
	value.x = stof(vals[1]);
	value.y = stof(vals[2]);
	value.z = stof(vals[3]);
	return value;
}

Scene SceneParser::parseSceneFile(string fileName)
{
	ifstream file(fileName);
	string str;
	Scene scene;
	std::stringstream buffer;
	buffer << file.rdbuf();

	string content = buffer.str();
	vector<string> properties = split(content);
	for (auto i = 0;i < properties.size();i++)
	{
		auto prop = properties[i];
		if (prop.compare("view")==0)
		{
			vec3 eye;
			eye.x = stof(properties[i + 2]);
			eye.y = stof(properties[i + 3]);
			eye.z = stof(properties[i + 4]);
			scene.eye = eye;

			vec3 center;
			center.x = stof(properties[i + 6]);
			center.y = stof(properties[i + 7]);
			center.z = stof(properties[i + 8]);
			scene.center = center;

			vec3 viewUp;
			center.x = stof(properties[i + 10]);
			center.y = stof(properties[i + 11]);
			center.z = stof(properties[i + 12]);
			scene.viewUp = viewUp;
		}
		else if (prop.compare("light")==0)
		{
			lightProperties lightProps = scene.lightVals[scene.numLightSources];
			scene.numLightSources += 1;
			string type = properties[i + 2];
			if (type.compare("directional")==0)
				lightProps.isLocal = 0;
			else if (type.compare("spot")==0)
				lightProps.isSpot = 1;
			else
				lightProps.isLocal = 1;

			int j = i + 3;

			if (properties[j].compare("ambient")==0) 
			{
				
				vec3 ambient;
				ambient.x = stof(properties[j + 1]);
				ambient.y = stof(properties[j + 2]);
				ambient.z = stof(properties[j + 3]);
				j += 4;
				lightProps.ambient = ambient;
			}
			
			if (properties[j].compare("color")==0) 
			{
				vec3 color;
				color.x = stof(properties[j + 1]);
				color.y = stof(properties[j + 2]);
				color.z = stof(properties[j + 3]);
				j += 4;
				lightProps.color = color;
			}


			if (properties[j].compare("position")==0)
			{
				vec3 position;
				position.x = stof(properties[j + 1]);
				position.y = stof(properties[j + 2]);
				position.z = stof(properties[j + 3]);
				j += 4;
				lightProps.position = position;
			}

			if (properties[j].compare("constAtt")==0)
			{
				lightProps.constantAttenuation = stof(properties[j + 1]);
				j += 2;
				
			}

			if (properties[j].compare("linearAtt")==0)
			{
				lightProps.linearAttenuation = stof(properties[j + 1]);
				j += 2;
			}

			if (properties[j].compare("quadAtt")==0)
			{
				lightProps.quadraticAttenuation = stof(properties[j + 1]);
				j += 2;
			}
			if (properties[j].compare("coneDirection")==0)
			{
				vec3 coneDirection;
				coneDirection.x = stof(properties[j + 1]);
				coneDirection.y = stof(properties[j + 2]);
				coneDirection.z = stof(properties[j + 3]);
				j += 4;
				lightProps.coneDirection = coneDirection;
			}
			if (properties[j].compare("spotCosCutoff")==0)
			{
				float spotCosCutoff;
				spotCosCutoff = stof(properties[j + 1]);
				
				j += 2;
				lightProps.spotCosCutoff = spotCosCutoff;
			}
			if (properties[j].compare("spotExponent")==0)
			{
				float spotExponent;
				spotExponent = stof(properties[j + 1]);
				
				j += 2;
				lightProps.spotExponent = spotExponent;
			}
			scene.lightVals[scene.numLightSources - 1] = lightProps;
		}
		else if (prop.compare("object")==0)
		{
			Object obj;
			mat4 modelingMatrix(1.0);
			obj.path = properties[i + 1];
			obj.shader = properties[i + 3];

			int j = i + 4;
			if (j >= properties.size()) {
				obj.modelingMatrix = modelingMatrix;
				scene.objects.push_back(obj);
				break;
			}
			if (properties[j].compare("rx")==0)
			{

				float angle = stof(properties[j + 1]);
				vec3 axis(1, 0, 0);
				mat4 rotMatrix = glm::rotate(modelingMatrix, angle*3.14159f / 180.0f, axis);
				modelingMatrix = modelingMatrix*rotMatrix;
				obj.rx = angle;
				j += 2;
			}
			if (j >= properties.size()) {
				obj.modelingMatrix = modelingMatrix;
				scene.objects.push_back(obj);
				break;
			}
			if (properties[j].compare("ry")==0)
			{
				float angle = stof(properties[j + 1]);
				obj.ry = angle;
				vec3 axis(0, 1, 0);
				mat4 rotMatrix = glm::rotate(modelingMatrix, angle*3.14159f / 180.0f, axis);
				modelingMatrix = modelingMatrix*rotMatrix;
				j += 2;
			}
			if (j >= properties.size()) {
				obj.modelingMatrix = modelingMatrix;
				scene.objects.push_back(obj);
				break;
			}
			if (properties[j].compare("rz")==0)
			{
				float angle = stof(properties[j + 1]);
				obj.rz = angle;
				vec3 axis(0, 0, 1);
				mat4 rotMatrix = glm::rotate(modelingMatrix, angle*3.14159f / 180.0f, axis);
				modelingMatrix = modelingMatrix*rotMatrix;
				j += 2;
			}
			if (j >= properties.size()) {
				obj.modelingMatrix = modelingMatrix;
				scene.objects.push_back(obj);
				break;
			}
			if (properties[j].compare("t")==0)
			{
				vec3 t;
				t.x = stof(properties[j + 1]);
				t.y = stof(properties[j + 2]);
				t.z = stof(properties[j + 3]);
				j += 4;
				obj.t = t;
				mat4 translationMatrix = glm::translate(modelingMatrix, t);
				modelingMatrix = modelingMatrix*translationMatrix;
			}
			if (j >= properties.size()) {
				obj.modelingMatrix = modelingMatrix;
				scene.objects.push_back(obj);
				break;
			}
			if (properties[j].compare("s") == 0)
			{
				vec3 s;
				s.x = stof(properties[j + 1]);
				s.y = stof(properties[j + 2]);
				s.z = stof(properties[j + 3]);
				j += 4;
				mat4 scaleMatrix = glm::scale(modelingMatrix, s);
				modelingMatrix = modelingMatrix * scaleMatrix;
			}
			if (j >= properties.size()) {
				obj.modelingMatrix = modelingMatrix;
				scene.objects.push_back(obj);
				break;
			}
			obj.modelingMatrix = modelingMatrix;
			scene.objects.push_back(obj);
		}
	}
	return scene;
}