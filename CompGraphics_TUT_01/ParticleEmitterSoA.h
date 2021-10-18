// Code from intermediate CG tutorial

#pragma once

#include "VertexBufferObject.h"
#include "glm/vec3.hpp"
#include <memory>
#include <math.h>
#include "Texture.h"
#include "ShaderProgram.h"
#include "glm/gtc/type_ptr.hpp"

class ParticleEmitterSoA
{
	struct Particle
	{
		glm::vec3* positions;
		glm::vec3* velocities;
		glm::vec3* accelerations;
		float*	remainingLives;
		float* masses;
		// Other particle properties...
	} particles;

public:
	ParticleEmitterSoA();
	~ParticleEmitterSoA();

	// Initialize memory for particles
	void initialize(unsigned int numParticles);

	// playback control
	inline void play() { playing = true; }
	inline void pause() { playing = false; }

	// Updates each particle in system
	void update(float dt);

	// Draws particles to screen
	void draw(ShaderProgram &shader, glm::mat4 &cameraTransform, glm::mat4 & cameraProjection, Texture &tex);

	void freeMemory();

	// Emitter position in world space
	// aka where the particles spawn
	glm::vec3 initialPosition;

	// locomotion properties
	// Ranges store the min (x) and max(y) bound of random values for the property
	glm::vec3 lifeRange;
	glm::vec3 initialForceMin;
	glm::vec3 initialForceMax;


	VertexBufferObject vbo;

	bool playing;	// false is update is paused


	//std::shared_ptr<Texture> texture;

private:
	unsigned int numParticles; // Number of particles passed into initialize()
	bool allocated; // false if memory not allocated
};