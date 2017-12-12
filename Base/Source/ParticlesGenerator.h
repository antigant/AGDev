#pragma once
#include <vector>
#include "Particles.h"

class Mesh;

class CParticlesGenerator
{
private:
	CParticlesGenerator(void);
	static CParticlesGenerator *instance;

	//Mesh *particles;
	double spawnTime, fixSpawnTime;
	std::vector<CParticles*> particlesContainer;
	int spawnCount, maxParticles;

public:
	static CParticlesGenerator *GetInstance()
	{
		if (!instance)
			instance = new CParticlesGenerator;
		return instance;
	}
	static bool DropInstance()
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
			return true;
		}
		return false;
	}
	~CParticlesGenerator(void);

	//// Set particle mesh
	//void SetParticles(Mesh *particles);

	// Init the generator
	void Init(void);
	// Update the particles
	void Update(double dt = 0.0333);
	// Render the particles
	void Render(void);
};