#include "scene.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <iterator>

Scene::Scene()
{
	numLightSources = 0;
	for (int i = 0;i < 4;i++) {
		lightVals[i].isEnabled = 0;
		lightVals[i].isSpot = 0;
		lightVals[i].isLocal = 0;
		lightVals[i].spotCosCutoff = 0;
		lightVals[i].spotExponent = 0;
		lightVals[i].constantAttenuation = 0;
		lightVals[i].linearAttenuation = 0;
		lightVals[i].quadraticAttenuation = 0;
	}
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
			viewUp.x = stof(properties[i + 10]);
			viewUp.y = stof(properties[i + 11]);
			viewUp.z = stof(properties[i + 12]);
			scene.viewUp = vec3(viewUp);
		}
		else if (prop.compare("light")==0)
		{
			lightProperties lightProps = scene.lightVals[scene.numLightSources];
			scene.numLightSources += 1;
			string type = properties[i + 2];
			if (type.compare("directional")==0)
				scene.lightVals[scene.numLightSources - 1].isLocal = 0;
			else if (type.compare("spot") == 0) {
				scene.lightVals[scene.numLightSources - 1].isSpot = 1;
				scene.lightVals[scene.numLightSources - 1].isLocal = 1;
			}
			else {
				scene.lightVals[scene.numLightSources - 1].isLocal = 1;
				//scene.lightVals[scene.numLightSources - 1].isSpot = 0;
			}

			int j = i + 3;

			if (properties[j].compare("ambient")==0)
			{

				vec3 ambient;
				ambient.x = stof(properties[j + 1]);
				ambient.y = stof(properties[j + 2]);
				ambient.z = stof(properties[j + 3]);
				j += 4;
				scene.lightVals[scene.numLightSources - 1].ambient = ambient;
			}

			if (properties[j].compare("color")==0)
			{
				vec3 color;
				color.x = stof(properties[j + 1]);
				color.y = stof(properties[j + 2]);
				color.z = stof(properties[j + 3]);
				j += 4;
				scene.lightVals[scene.numLightSources - 1].color = color;
			}


			if (properties[j].compare("position")==0)
			{
				vec3 position;
				position.x = stof(properties[j + 1]);
				position.y = stof(properties[j + 2]);
				position.z = stof(properties[j + 3]);
				j += 4;
				scene.lightVals[scene.numLightSources - 1].position = position;
			}

			if (properties[j].compare("constAtt")==0)
			{
				scene.lightVals[scene.numLightSources - 1].constantAttenuation = stof(properties[j + 1]);
				j += 2;

			}

			if (properties[j].compare("linearAtt")==0)
			{
				scene.lightVals[scene.numLightSources - 1].linearAttenuation = stof(properties[j + 1]);
				j += 2;
			}

			if (properties[j].compare("quadAtt")==0)
			{
				scene.lightVals[scene.numLightSources - 1].quadraticAttenuation = stof(properties[j + 1]);
				j += 2;
			}
			if (properties[j].compare("coneDirection")==0)
			{
				vec3 coneDirection;
				coneDirection.x = stof(properties[j + 1]);
				coneDirection.y = stof(properties[j + 2]);
				coneDirection.z = stof(properties[j + 3]);
				j += 4;
				scene.lightVals[scene.numLightSources - 1].coneDirection = coneDirection;
			}
			if (properties[j].compare("spotCosCutoff")==0)
			{
				float spotCosCutoff;
				spotCosCutoff = stof(properties[j + 1]);

				j += 2;
				scene.lightVals[scene.numLightSources - 1].spotCosCutoff = spotCosCutoff;
			}
			if (properties[j].compare("spotExponent")==0)
			{
				float spotExponent;
				spotExponent = stof(properties[j + 1]);

				j += 2;
				scene.lightVals[scene.numLightSources - 1].spotExponent = spotExponent;
			}
		//	scene.lightVals[scene.numLightSources - 1] = lightProps;
		}
		else if (prop.compare("object")==0)
		{
			Object obj;
			mat4 modelingMatrix(1.0);
			obj.path = properties[i + 1];
			if (properties[i+2].compare("shader") != 0)
				{
					cout<<"Shader property not present for object in scene file. ERROR!"<<endl;
					exit(1);
				}
			obj.shader = properties[i + 3];

			int j = i + 4;
			if (j >= properties.size()) {
				obj.modelingMatrix = modelingMatrix;
				scene.objects.push_back(obj);
				return scene;
			}
			if (properties[j].compare("object") == 0)
            {
                obj.modelingMatrix = modelingMatrix;
                scene.objects.push_back(obj);
                continue;
            }
			while (j<properties.size()) {
				if (properties[j].compare("rx") == 0)
				{

					float angle = stof(properties[j + 1]);
					vec3 axis(1, 0, 0);
					mat4 rotMatrix = glm::rotate(mat4(1.0), angle*3.14159f / 180.0f, axis);
					modelingMatrix = rotMatrix*modelingMatrix;
					obj.rx = angle;
					j += 2;
				}
				if (j >= properties.size()) {
					obj.modelingMatrix = modelingMatrix;
					scene.objects.push_back(obj);
					return scene;
				}
				if (properties[j].compare("ry") == 0)
				{
					float angle = stof(properties[j + 1]);
					obj.ry = angle;
					vec3 axis(0, 1, 0);
					mat4 rotMatrix = glm::rotate(mat4(1.0), angle*3.14159f / 180.0f, axis);
					modelingMatrix = rotMatrix*modelingMatrix;
					j += 2;
				}
				if (j >= properties.size()) {
					obj.modelingMatrix = modelingMatrix;
					scene.objects.push_back(obj);
					return scene;
				}
				if (properties[j].compare("rz") == 0)
				{
					float angle = stof(properties[j + 1]);
					obj.rz = angle;
					vec3 axis(0, 0, 1);
					mat4 rotMatrix = glm::rotate(mat4(1.0), angle*3.14159f / 180.0f, axis);
					modelingMatrix = rotMatrix * modelingMatrix;
					j += 2;
				}
				if (j >= properties.size()) {
					obj.modelingMatrix = modelingMatrix;
					scene.objects.push_back(obj);
					return scene;
				}
				if (properties[j].compare("t") == 0)
				{
					vec3 t;
					t.x = stof(properties[j + 1]);
					t.y = stof(properties[j + 2]);
					t.z = stof(properties[j + 3]);
					j += 4;
					obj.t = t;
					mat4 translationMatrix = glm::translate(mat4(1.0), t);
					modelingMatrix = translationMatrix * modelingMatrix;
				}
				if (j >= properties.size()) {
					obj.modelingMatrix = modelingMatrix;
					scene.objects.push_back(obj);
					return scene;
				}
				if (properties[j].compare("s") == 0)
				{
					vec3 s;
					s.x = stof(properties[j + 1]);
					s.y = stof(properties[j + 2]);
					s.z = stof(properties[j + 3]);
					j += 4;
					mat4 scaleMatrix = glm::scale(mat4(1.0), s);
					modelingMatrix = scaleMatrix*modelingMatrix;
				}
				if (j >= properties.size()) {
					obj.modelingMatrix = modelingMatrix;
					scene.objects.push_back(obj);
					return scene;
				}
				if (properties[j].compare("object") == 0)
					break;
			}
			obj.modelingMatrix = modelingMatrix;
			scene.objects.push_back(obj);
		}
	}
	return scene;
}
