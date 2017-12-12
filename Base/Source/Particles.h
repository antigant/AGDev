#pragma once
#include "EntityBase.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"

class Mesh;
class CParticles : public EntityBase
{
public:
	CParticles(void);
	CParticles(Mesh* meshModel);
	~CParticles(void);

	virtual void Update(double dt);
	virtual void Render();

private:
	Mesh *modelMesh;
	double lifeTime, fixLifeTime;
	float m_fSpeed;
};

namespace Create
{
	CParticles *particles(	const std::string &_meshName,
							const Vector3 &_position,
							const float m_fSpeed);
}