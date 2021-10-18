#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Light.h"
#include "Material.h"
#include "AnimationID.h"
#include "FrameBufferObject.h"

#include <memory>

enum Repair_Type
{
	TIRE,
	ENGINE,
	PAINT
};

class GameObject
{
public:
	GameObject();
	GameObject(const Mesh &m);
	~GameObject();

	Mesh mesh;

	glm::mat4 empty = glm::mat4();

	glm::mat4 transform;
	
	glm::mat4 translate;
	glm::mat4 rotate;
	float scale = 1;

	Material mat;

	void loadMesh(const std::string &meshFile);
	void loadTexture(TextureType type, const std::string &texFile);

	void draw(ShaderProgram &shader, glm::mat4 &cameraTransform, glm::mat4 & cameraProjection, std::vector<Light> &pointLights, Light &directionalLight);

	void drawQuad(ShaderProgram &shader, FrameBufferObject &fboBlurredBrightPass);

	float playerSpeed = 0.2f;

	bool energyDrink = false;

	// For collision
	glm::vec3 position;
	glm::vec3 previousPosition;
	float left = 0;
	float right = 0;
	float down = 0;
	float up = 0;

	// For timer
	void timer(float dt, int repairType);
	bool onceTime = false; // Checks if the start timer has been started
	float startTime; // Start time
	float endTime; // End time
	float time = 1.f; // 
	float tempTime = 0.0f;
	bool timeDone = false;

	// For level timer
	void levelTimer(float dt);
	bool onceLevelTime = false; // Checks if the start timer has been started
	float startLevelTime; // Start time
	float endLevelTime; // End time
	float levelTime = 99.f; // Total Level time
	float tempLevelTime = 0.0f;
	int displayTime;
	bool levelTimeDone = false;
	bool gameDone = false;
	/*int tempTimeStore = 1;
	int tempTimeStore2 = 0;*/
	void resetTimer();

	//Repair Times
	float engineRepair = 3.f;
	float paintRepair = 2.5f;
	float tireRepair = 2.f;

	// Animation Stuff
	std::shared_ptr<Mesh> meshAnimation;
	std::vector<std::shared_ptr<Mesh>> MorphTargets;
	void drawAnimation(ShaderProgram &shader, glm::mat4 &cameraTransform, glm::mat4 & cameraProjection, std::vector<Light> &pointLights, Light &directionalLight);
	void updateAnimation(float dt);
	float MorphTime = 0;
	void loadMorphTarget(const std::string &meshFile);
	unsigned int currentFrame = 0;
	unsigned int nextFrame = 1;
	std::string animationName; // Name of the animation
	unsigned int ID; // Which place in the vector is the animation playing at
	unsigned int startFrame; // Where in the vector does the animation start from
	unsigned int endFrame; // Where in the vector does the animation end
	unsigned int numFrames; // How many frames are in the animation
	bool animationChangeDone = false; // Checks if the aniamtion has smoothly changed
	bool playAnimationOnce = false; // Makes it so that the function gets called only once
	float animationSpeed = 5.0f; // How fast the animation plays

								 // String ID, numFrames
	void setAnimations(std::string name, unsigned int nF);
	std::shared_ptr<AnimationID> firstID;
	std::vector<std::shared_ptr<AnimationID>> IDs;
	void playAnimation(std::string name);


	// Repair
	int currentRepair;
	bool repairDone = false;
};