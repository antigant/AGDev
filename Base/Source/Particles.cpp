#include "Particles.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "EntityManager.h"

CParticles::CParticles(void)
	: fixLifeTime(3.0)
	, modelMesh(NULL)
	, m_fSpeed(2.5f)
{
	lifeTime = fixLifeTime;
}

CParticles::CParticles(Mesh* meshModel)
	: fixLifeTime(3.0)
	, modelMesh(meshModel)
	, m_fSpeed(2.5f)
{
	lifeTime = fixLifeTime;
}

CParticles::~CParticles(void)
{

}

void CParticles::Update(double dt)
{
	if (IsDone())
		return;

	lifeTime -= (float)dt;
	if (lifeTime <= 0.0)
	{
		SetIsDone(true);
		return;
	}

	// Update position
	position.Set(position.x, position.y + (float)(m_fSpeed * dt), position.z);
}

void CParticles::Render()
{
	if (IsDone() || lifeTime <= 0.0)
		return;

	MS &modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

CParticles *Create::particles(	const std::string &_meshName,
								const Vector3 &_position,
								const float m_fSpeed)
{
	Mesh *modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CParticles *result = new CParticles(modelMesh);
	result->SetCollider(false);
	result->SetIsDone(false);
	EntityManager::GetInstance()->AddEntity(result);

	return result;
}