#include "ParticlesGenerator.h"
#include "Mesh.h"

CParticlesGenerator::CParticlesGenerator(void)
	: fixSpawnTime(0.75)
	, spawnCount(0)
	, maxParticles(500)
{
	spawnTime = fixSpawnTime;
}

CParticlesGenerator::~CParticlesGenerator(void)
{
}

// Init the generator
void CParticlesGenerator::Init(void)
{
	particlesContainer.resize(maxParticles);
	std::fill(particlesContainer.begin(), particlesContainer.end(), nullptr);
}

// Update the particles
void CParticlesGenerator::Update(double dt)
{
	spawnTime -= dt;

	if (spawnTime > 0.0)
	{
		spawnTime = fixSpawnTime;
		if (particlesContainer.size() >= maxParticles)
			return;

		// After t amount of time, start spawning particles
		spawnCount = Math::RandIntMinMax(20, 50);
		for (int i = 0; i < spawnCount; ++i)
		{
			//CParticles *result = Create::particles("particle", )
		}
	}

	// Update all the particles in the container
	for (std::vector<CParticles*>::iterator it = particlesContainer.begin(); it != particlesContainer.end(); ++it)
	{
		CParticles *particle = (CParticles*)*it;
		if (particle->IsDone())
			continue;

		particle->Update(dt);
	}
}

// Render the particles
void CParticlesGenerator::Render(void)
{

}