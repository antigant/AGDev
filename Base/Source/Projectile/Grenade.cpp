#include "Grenade.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GL\glew.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../SceneGraph.h"

#include <iostream>
using namespace std;

CGrenade::CGrenade(void)
	: CProjectile(NULL)
	,m_fGravity(-10.0f)
	,m_fElapsedTime(0.0f)
	,m_pTerrain(NULL)
{
}

CGrenade::CGrenade(Mesh *_modelMesh)
	: CProjectile(_modelMesh)
	, m_fGravity(-10.0f)
	, m_fElapsedTime(0.0f)
	, m_pTerrain(NULL)
{
}

CGrenade::~CGrenade()
{
	m_pTerrain = NULL;//Don't delete this as the terrain is deleted in CPlayerInfo
	modelMesh = NULL;
	theSource = NULL;
}

//Update the status of this projectile
void CGrenade::Update(double dt)
{
	if (m_bStatus == false)
		return;

	// Update TimeLife of projectile. Set to inactive if too long
	m_fLifetime -= (float)dt;
	if (m_fLifetime < 0.0f)
	{
		SetStatus(false);
		SetIsDone(true);	// This method is to inform the EntityManager that it should remove this instance
		for (int i = 0; i < CSpatialPartition::GetInstance()->GetObjects(position, 0.f).size(); ++i)
		{
			CSceneGraph::GetInstance()->DeleteNode(CSpatialPartition::GetInstance()->GetObjects(position, 0.f)[i]);
			CSpatialPartition::GetInstance()->Remove(CSpatialPartition::GetInstance()->GetObjects(position, 0.f)[i]);
		}
		return;
	}

	//Check if the Grenade is already on the ground
	if (position.y >= m_pTerrain->GetTerrainHeight(position)-10.0f + Math::EPSILON)
	{
	
	// Update Position
	m_fElapsedTime += dt;
	
	position.Set(position.x + (float)(theDirection.x * dt * m_fSpeed), position.y + (float)(theDirection.y * m_fElapsedTime * m_fSpeed) + (0.5 * m_fGravity * m_fElapsedTime * m_fElapsedTime), position.z + (float)(theDirection.z * dt * m_fSpeed));

		if (position.y < m_pTerrain->GetTerrainHeight(position) - 9.2f )
		{
			position.y = m_pTerrain->GetTerrainHeight(position) - 9.2f;
			m_fSpeed = 0.0f;
			return;
		}
	}
}

//Set the terrain for the player info
void CGrenade::SetTerrain(GroundEntity *m_pTerrain)
{
	this->m_pTerrain = m_pTerrain;
}

// Create a projectile and add it into EntityManager
CGrenade *Create::Grenade(const std::string &_meshName,
	const Vector3 &_position,
	const Vector3 &_direction,
	const float m_fLifetime,
	const float m_fSpeed,
	CPlayerInfo *_source)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CGrenade* result = new CGrenade(modelMesh);
	result->Set(_position, _direction * 10.f, m_fLifetime, m_fSpeed);
	result->SetStatus(true);
	result->SetCollider(true);
	result->SetSource(_source);
	result->SetTerrain(_source->GetTerrain());
	EntityManager::GetInstance()->AddEntity(result,true);
	return result;
}