#include "GameObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define BUFFER_OFFSET(i) ((char*)0+(i))

GameObject::GameObject()
{
	resetTimer();
}
GameObject::GameObject(const Mesh &m)
{
	resetTimer();
	mesh = m;
}
GameObject::~GameObject()
{
	//mesh.unload();
}

void GameObject::loadMesh(const std::string &meshFile)
{
	if (!mesh.loadFromFile(meshFile))
	{
		std::cout << "Model failed to load." << std::endl;
		system("pause");
		exit(0);
	}
}

void GameObject::loadTexture(TextureType type, const std::string &texFile) 
{
	mat.loadTexture(type, texFile);
}

void GameObject::draw(ShaderProgram &shader, glm::mat4 &cameraTransform, glm::mat4 & cameraProjection, std::vector<Light> &pointLights, Light &directionalLight)
{	
	// F = T * R * S
	transform = translate * rotate * glm::scale(glm::mat4(), glm::vec3(scale));

	shader.bind();
	shader.sendUniformMat4("uModel", glm::value_ptr(transform), false);
	shader.sendUniformMat4("uView", glm::value_ptr(cameraTransform), false);
	shader.sendUniformMat4("uProj", glm::value_ptr(cameraProjection), false);
	shader.sendUniform("T", MorphTime);

	shader.sendUniform("material.diffuse", 0);
	shader.sendUniform("material.specular", 1);
	shader.sendUniform("material.normal", 2);
	shader.sendUniform("material.hue", mat.hue);
	shader.sendUniform("material.specularExponent", mat.specularExponent);

	for (int i = 0; i < pointLights.size(); i++)
	{
		// It would look like this "pointights[i]."
		std::string prefix = "pointLights[" + std::to_string(i) + "].";

		shader.sendUniform(prefix + "position", cameraTransform * pointLights[i].positionOrDirection);
		shader.sendUniform(prefix + "ambient", pointLights[i].ambient);
		shader.sendUniform(prefix + "diffuse", pointLights[i].diffuse);
		shader.sendUniform(prefix + "specular", pointLights[i].specular);
		shader.sendUniform(prefix + "specularExponent", pointLights[i].specularExponent);
		shader.sendUniform(prefix + "constantAttenuation", pointLights[i].constantAttenuation);
		shader.sendUniform(prefix + "linearAttenuation", pointLights[i].linearAttenuation);
		shader.sendUniform(prefix + "quadraticAttenuation", pointLights[i].quadraticAttenuation);
	}

	shader.sendUniform("directionalLight.direction", cameraTransform * directionalLight.positionOrDirection);
	shader.sendUniform("directionalLight.ambient", directionalLight.ambient);
	shader.sendUniform("directionalLight.diffuse", directionalLight.diffuse);
	shader.sendUniform("directionalLight.specular", directionalLight.specular);
	shader.sendUniform("directionalLight.specularExponent", directionalLight.specularExponent);


	glActiveTexture(GL_TEXTURE0);
	mat.diffuse.bind();

	glActiveTexture(GL_TEXTURE1);
	mat.specular.bind();

	glActiveTexture(GL_TEXTURE2);
	mat.normal.bind();

	glBindVertexArray(mesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh.getNumVertices());
	glBindVertexArray(GL_NONE);

	glActiveTexture(GL_TEXTURE2);
	mat.normal.unbind();
	glActiveTexture(GL_TEXTURE1);
	mat.specular.unbind();
	glActiveTexture(GL_TEXTURE0);
	mat.diffuse.unbind();

	shader.unbind();
}

void GameObject::drawQuad(ShaderProgram &shader, FrameBufferObject &fboBlurredBrightPass)
{
	empty = glm::mat4();

	shader.bind();

	shader.sendUniformMat4("u_mvp", glm::value_ptr(empty), false);
	shader.sendUniform("u_bloomThreshold", 0.2f);
	shader.sendUniform("u_texelSize", glm::vec4(1.0f / fboBlurredBrightPass.getWidth(), 1.0f / fboBlurredBrightPass.getHeight(), 0.0f, 0.0f));

	glBindVertexArray(mesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh.getNumVertices());
	glBindVertexArray(GL_NONE);

	shader.unbind();

}

void GameObject::timer(float dt, int repairType)
{

	if (repairType == 1) { time = engineRepair; }
	if (repairType == 2) { time = paintRepair; }
	if (repairType == 3) { time = tireRepair; }

	if (onceTime == false)
	{
		startTime = dt;
		endTime = startTime + time;
		timeDone = false;
		onceTime = true;
	}

	tempTime = tempTime + dt;
	if (tempTime > endTime)
	{
		onceTime = false;
		timeDone = true;
		tempTime = 0.0f;
	}

	//std::cout << tempTime << " / " << time << std::endl;
}

void GameObject::levelTimer(float dt)
{
	if (onceLevelTime == false)
	{
		startLevelTime += dt;
		endLevelTime = startLevelTime + levelTime;
		levelTimeDone = false;
		onceLevelTime = true;
	}

	tempLevelTime = tempLevelTime + dt;

	if (displayTime != int(endLevelTime - tempLevelTime)) // Makes it display only when the number changes
	{
		displayTime = endLevelTime - tempLevelTime;
		std::cout << "Level Time: " << displayTime << std::endl;
	}

	if (tempLevelTime > endLevelTime)
	{
		onceLevelTime = false;
		levelTimeDone = true;
		tempLevelTime = 0.0f;
		gameDone = true;
	}
	
}

void GameObject::resetTimer()
{
	playerSpeed = 0.2f;

	energyDrink = false;

	repairDone = false;
	time = 0.0f;
	animationSpeed = 5.0f;

	// Level Timer
	onceLevelTime = false; // Checks if the start timer has been started
	startLevelTime = 0.f; // Start time
	endLevelTime = 0.f; // End time
	tempLevelTime = 0.0f;
	displayTime = 0.0f;
	levelTime = 99.f; // How long the level should be
	levelTimeDone = false;
	gameDone = false;

	//Repair Times
	engineRepair = 3.f;
	paintRepair = 2.5f;
	tireRepair = 2.f;
}

void GameObject::drawAnimation(ShaderProgram &shader, glm::mat4 &cameraTransform, glm::mat4 & cameraProjection, std::vector<Light> &pointLights, Light &directionalLight)
{
	transform = translate * rotate * glm::scale(glm::mat4(), glm::vec3(scale));

	shader.bind();
	shader.sendUniformMat4("uModel", glm::value_ptr(transform), false);
	shader.sendUniformMat4("uView", glm::value_ptr(cameraTransform), false);
	shader.sendUniformMat4("uProj", glm::value_ptr(cameraProjection), false);
	shader.sendUniform("T", MorphTime);

	shader.sendUniform("material.diffuse", 0);
	shader.sendUniform("material.specular", 1);
	shader.sendUniform("material.hue", mat.hue);
	shader.sendUniform("material.specularExponent", mat.specularExponent);


	for (int i = 0; i < pointLights.size(); i++)
	{
		// It would look like this "pointights[i]."
		std::string prefix = "pointLights[" + std::to_string(i) + "].";

		shader.sendUniform(prefix + "position", cameraTransform * pointLights[i].positionOrDirection);
		shader.sendUniform(prefix + "ambient", pointLights[i].ambient);
		shader.sendUniform(prefix + "diffuse", pointLights[i].diffuse);
		shader.sendUniform(prefix + "specular", pointLights[i].specular);
		shader.sendUniform(prefix + "specularExponent", pointLights[i].specularExponent);
		shader.sendUniform(prefix + "constantAttenuation", pointLights[i].constantAttenuation);
		shader.sendUniform(prefix + "linearAttenuation", pointLights[i].linearAttenuation);
		shader.sendUniform(prefix + "quadraticAttenuation", pointLights[i].quadraticAttenuation);
	}

	shader.sendUniform("directionalLight.direction", cameraTransform * directionalLight.positionOrDirection);
	shader.sendUniform("directionalLight.ambient", directionalLight.ambient);
	shader.sendUniform("directionalLight.diffuse", directionalLight.diffuse);
	shader.sendUniform("directionalLight.specular", directionalLight.specular);
	shader.sendUniform("directionalLight.specularExponent", directionalLight.specularExponent);

	glActiveTexture(GL_TEXTURE0);
	mat.diffuse.bind();

	glActiveTexture(GL_TEXTURE1);
	mat.specular.bind();

	glBindVertexArray(MorphTargets[currentFrame]->vao);
	glDrawArrays(GL_TRIANGLES, 0, MorphTargets[currentFrame]->getNumVertices());
	/*glBindVertexArray(mesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh.getNumVertices());*/
	glBindVertexArray(GL_NONE);

	mat.specular.unbind();
	glActiveTexture(GL_TEXTURE0);
	mat.diffuse.unbind();

	shader.unbind();
}

void GameObject::updateAnimation(float dt)
{
	MorphTime += dt * animationSpeed;
	//std::cout << animationSpeed << std::endl;

	if (MorphTime > 1)
	{
		MorphTime = 0;
		if (animationChangeDone) // Checks if the animation has smoothly changed
		{
			currentFrame = startFrame;
			nextFrame = currentFrame + 1;
			animationChangeDone = false;
			/*std::cout << "Current Frame2: " << currentFrame << std::endl;
			std::cout << "Next Frame2: " << nextFrame << std::endl;
			std::cout << "End Frame2: " << endFrame << std::endl;*/
		}
		else
		{
			currentFrame++;
			nextFrame++;
		}
	}
	if (nextFrame >= endFrame)
	{
		currentFrame = startFrame;
		nextFrame = currentFrame + 1;
	}
	if (animationName != IDs[ID]->getName()) // Checks if the animation has changed
	{
		animationName = IDs[ID]->getName();
		startFrame = IDs[ID]->getStartFrame();
		numFrames = IDs[ID]->getNumFrames();
		endFrame = startFrame + numFrames;

		animationChangeDone = true;
		nextFrame = startFrame; // Makes it so that the animation changes smoothly
		/*std::cout << "Current Frame: " << currentFrame << std::endl;
		std::cout << "Next Frame: " << nextFrame << std::endl;
		std::cout << "End Frame: " << endFrame << std::endl;*/

	}

	//std::cout << "Current Frame: " << currentFrame << std::endl;
	//std::cout << "Next Frame: " << nextFrame << std::endl;

	// Current Frame
	glBindVertexArray(MorphTargets[currentFrame]->vao);
	//Vertices Current Frame
	glBindBuffer(GL_ARRAY_BUFFER, MorphTargets[currentFrame]->vboVertices);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	//Normals Current Frame
	glBindBuffer(GL_ARRAY_BUFFER, MorphTargets[currentFrame]->vboNormals);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	//Vertices Next Frame
	glBindBuffer(GL_ARRAY_BUFFER, MorphTargets[nextFrame]->vboVertices);
	glVertexAttribPointer((GLuint)3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));
	//Normals Next Frame
	glBindBuffer(GL_ARRAY_BUFFER, MorphTargets[nextFrame]->vboNormals);
	glVertexAttribPointer((GLuint)4, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	//Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

void GameObject::loadMorphTarget(const std::string &meshFile)
{
	std::shared_ptr<Mesh> newTarget = std::make_shared<Mesh>();

	if (!newTarget->loadFromFile(meshFile))
	{
		std::cout << "Model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!meshAnimation)
	{
		meshAnimation = std::make_shared<Mesh>();
		meshAnimation->numVertices = newTarget->getNumVertices();

	}
	else
	{
		if (newTarget->getNumVertices() != meshAnimation->getNumVertices())
		{
			std::cout << meshFile << " New target does not have the same number of vertices as previous target!" << std::endl;
			std::cout << newTarget->getNumVertices() << " " << meshAnimation->getNumVertices() << std::endl;

			return;
		}
	}
	MorphTargets.push_back(newTarget);
}

void GameObject::setAnimations(std::string name, unsigned int nF)
{
	// Checks if the name is already used 
	for (int i = 0; i < IDs.size(); i++)
	{
		if (name == IDs[i]->getName())
		{
			std::cout << name << ": is already in use." << std::endl;
			return;
		}
	}

	std::shared_ptr<AnimationID> newID = std::make_shared<AnimationID>();
	if (!firstID) // If there are no animations set this will set the first one
	{
		firstID = std::make_shared<AnimationID>();
		newID->setName(name);
		newID->setNumFrames(nF);
		newID->setStartFrame(0);
		animationName = name;
		startFrame = 0;
		numFrames = nF;
		endFrame = startFrame + numFrames;

	}
	else
	{
		newID->setName(name);
		newID->setNumFrames(nF);
		newID->setStartFrame(MorphTargets.size() - nF);
	}

	IDs.push_back(newID);
}

void GameObject::playAnimation(std::string name)
{
	if (IDs[ID]->getName() != name)
	{
		// Checks if that animation name in the vector
		for (int i = 0; i <= IDs.size() - 1; i++)
		{
			if (name == IDs[i]->getName())
			{
				ID = i;
				return;
			}
		}

		std::cout << name << ": is not a valid animation name." << std::endl;
	}
	
}