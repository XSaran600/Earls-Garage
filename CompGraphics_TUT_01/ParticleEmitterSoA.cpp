// Code from intermediate CG tutorial

#include "ParticleEmitterSoA.h"
#include <algorithm>
#include "glm/gtc/random.hpp"

ParticleEmitterSoA::ParticleEmitterSoA()
{
	// Initialize memory pointers to null
	particles.positions = nullptr;
	particles.velocities = nullptr;
	particles.remainingLives = nullptr;
	particles.accelerations = nullptr;
	particles.masses = nullptr;
	numParticles = 0;

	playing = false;
}

ParticleEmitterSoA::~ParticleEmitterSoA()
{
	freeMemory();
}


void ParticleEmitterSoA::initialize(unsigned int newNumParticles)
{
	// Clear existing memory
	if (allocated)
		freeMemory();

	if (newNumParticles) // make sure new array size is not 0
	{
		particles.positions = new glm::vec3[newNumParticles];
		particles.velocities = new glm::vec3[newNumParticles];
		particles.accelerations = new glm::vec3[newNumParticles];
		particles.remainingLives = new float[newNumParticles];
		particles.masses = new float[newNumParticles];
		memset(particles.remainingLives, 0, sizeof(float) * newNumParticles);

		numParticles = newNumParticles;
		allocated = true; // mark that memory has been allocated

		// Set up VBO
		// **** LOOK HERE ****
		AttributeDescriptor posAttrib;
		posAttrib.attributeLocation = VERTEX;
		posAttrib.data = particles.positions; // data points to particles.positions
		posAttrib.elementSize = sizeof(float);
		posAttrib.elementType = GL_FLOAT;
		posAttrib.numElements = newNumParticles * 3; 
		posAttrib.numElementsPerAttrib = 3;
		
		vbo.addAttributeArray(posAttrib);

		vbo.primitiveType = GL_POINTS;

		vbo.createVBO(GL_DYNAMIC_DRAW);
	}
}

void ParticleEmitterSoA::update(float dt)
{
	// Make sure particle memory is initialized
	if (allocated && playing)
	{
		// loop through each particle
		for (int i = 0; i < numParticles; i++)
		{
			// Get each property for the particle using pointer arithmetic
			glm::vec3* pos = particles.positions + i;
			glm::vec3* vel = particles.velocities + i;
			glm::vec3* accel = particles.accelerations + i;
			float* life = particles.remainingLives + i;
			float* mass = particles.masses + i;
			// other properties... 

			// check if alive
			if (*life <= 0)
			{
				// if dead respawn
				// could put additional logic here...
				*pos = initialPosition;

				(*vel).x = glm::mix(initialForceMin.x, initialForceMax.x, glm::linearRand(0.0f, 1.0f));
				(*vel).y = glm::mix(initialForceMin.y, initialForceMax.y, glm::linearRand(0.0f, 1.0f));
				(*vel).z = glm::mix(initialForceMin.z, initialForceMax.z, glm::linearRand(0.0f, 1.0f));

				*life = glm::linearRand(lifeRange.x, lifeRange.y);
				*mass = glm::linearRand(0.5f, 1.0f);
				*accel = *vel / *mass;
			}

			// Update position and velocity
			*pos += *vel * dt + *accel * 0.5f * (dt*dt);
			//*vel += dt;
			*life -= dt;
		}
	}
}

void ParticleEmitterSoA::draw(ShaderProgram &shader, glm::mat4 &cameraTransform, glm::mat4 &cameraProjection, Texture &tex)
{
	// Edited by Saran Krishnaraja 100621699

	// Update data in VBO
	AttributeDescriptor* attrib = vbo.getAttributeDescriptor(VERTEX);

	glBindVertexArray(vbo.getVAO());

	glEnableVertexAttribArray(attrib->attributeLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vbo.getVBO(VERTEX));
	glBufferSubData(GL_ARRAY_BUFFER, 0, numParticles * 3 * sizeof(float), particles.positions);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glm::mat4 model = cameraTransform * cameraProjection;

	shader.bind();
	shader.sendUniformMat4("u_mvp", glm::value_ptr(model), false);
	shader.sendUniformMat4("u_mv", glm::value_ptr(cameraTransform), false);
	shader.sendUniformMat4("u_proj", glm::value_ptr(cameraProjection), false);

	// Bind Texture
	tex.bind();
	glEnable(GL_BLEND); // Tells OpenGL to use the alpha channel
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	// Draw
	vbo.draw();

	glDepthMask(GL_TRUE);

	glDisable(GL_BLEND);
	// Unbind texture and shader
	tex.unbind();
	shader.unbind();
}

void ParticleEmitterSoA::freeMemory()
{
	// Free up all arrays here
	if (allocated)
	{
		delete[] particles.positions;
		delete[] particles.velocities;
		delete[] particles.remainingLives;
		delete[] particles.accelerations;
		delete[] particles.masses;

		particles.positions = nullptr;
		particles.velocities = nullptr;
		particles.remainingLives = nullptr;
		particles.accelerations = nullptr;
		particles.masses = nullptr;

		allocated = false;
		numParticles = 0;
	}
}

