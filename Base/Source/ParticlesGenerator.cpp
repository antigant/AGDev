#include "ParticlesGenerator.h"
#include "Mesh.h"

CParticlesGenerator::CParticlesGenerator(void)
	: fixSpawnTime(0.5)
{
	spawnTime = fixSpawnTime;
}

CParticlesGenerator::~CParticlesGenerator(void)
{
}

//// Set particle mesh
//void CParticlesGenerator::SetParticles(Mesh *particles)
//{
//	//this->particles = particles;
//}

// Update the particles
void CParticlesGenerator::Update(double dt)
{
	spawnTime -= dt;
	if (spawnTime > 0.0)
		return;

	// After t amount of time, start spawning particles

	spawnTime = fixSpawnTime;
}

// Render the particles
void CParticlesGenerator::Render(void)
{

}