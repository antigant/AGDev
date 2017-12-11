#pragma once
#include "RenderHelper.h"
#include "GraphicsManager.h"

class Mesh;

class CParticlesGenerator
{
private:
	CParticlesGenerator(void);
	static CParticlesGenerator *instance;

	//Mesh *particles;
	double spawnTime, fixSpawnTime;

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
			instance = NULL;
			return true;
		}
		return false;
	}
	~CParticlesGenerator(void);

	//// Set particle mesh
	//void SetParticles(Mesh *particles);

	// Update the particles
	void Update(double dt = 0.0333);
	// Render the particles
	void Render(void);
};