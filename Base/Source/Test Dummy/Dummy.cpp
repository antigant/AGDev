#include "Dummy.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "../GenericEntity.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

TestDummy::TestDummy()
	: name("Nameless")
	, ID(0)
	, parts(NULL)
	, isHit(false)
{
}


TestDummy::TestDummy(Mesh *part)
	: name("Nameless")
	, ID(0)
	, parts(part)
	, isHit(false)
{
}


TestDummy::~TestDummy()
{
}

void TestDummy::Init()
{
}

void TestDummy::Update(double dt)
{
	//int decidesleftright = Math::RandIntMinMax(-1, 1);
	float move = 0.9f;
	/*float move;
	if (decidesleftright = 1)
	{
		move = 0.9f;
	}
	else if (decidesleftright = -1)
	{
		move = -0.9f;
	}
	else
	{
		move = 0.f;
	}*/

	position += move * dt;

	if (!isHit)
		return;

	scale -= 0.5f * dt;
	if (scale.x <= 0 || scale.y <= 0 || scale.z <= 0)
		isDone = true;
}

void TestDummy::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(parts);
	modelStack.PopMatrix();
}

void TestDummy::SetIsDone(const bool _value)
{
	isHit = _value;
}

//
//TestDummy * Create::Torso(const std::string & _meshHead, const std::string & _meshBody, const Vector3 & _position)
//{
//	Mesh *modelHead = MeshBuilder::GetInstance()->GetMesh(_meshHead);
//	if (modelHead == nullptr)
//		return nullptr;
//	Mesh *modelBody = MeshBuilder::GetInstance()->GetMesh(_meshBody);
//	if (modelHead == nullptr)
//		return nullptr;
//
//	//TestDummy *result = new TestDummy(modelHead,modelBody);
//	result->SetPosition(_position);
//	result->SetCollider(true);
//
//	/*GenericEntity *HeadDummycube = Create::Entity("cube", _position);
//	HeadDummycube->SetCollider(true);
//	HeadDummycube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
//	CSceneNode *HeadDummyNode = CSceneGraph::GetInstance()->AddNode(HeadDummycube);
//
//	GenericEntity *BodyDummyCube = Create::Entity("cube", _position + Vector3(0,-1.1f,0));
//	BodyDummyCube->SetCollider(true);
//	BodyDummyCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
//	CSceneNode* BodyDummyNode = HeadDummyNode->AddChild(BodyDummyCube);
//
//*/
//	EntityManager::GetInstance()->AddEntity(result);
//	/*
//	result->Set(_position, _direction, m_fLifetime, m_fSpeed);
//	result->SetStatus(true);
//	result->SetCollider(true);
//	result->SetSource(_source);
//	EntityManager::GetInstance()->AddEntity(result);*/
//
//	return result;
//}

TestDummy * Create::dummy_part(const std::string & _meshHead, const Vector3 & _position)
{
	Mesh *modelHead = MeshBuilder::GetInstance()->GetMesh(_meshHead);
	if (modelHead == nullptr)
		return nullptr;


	TestDummy *result = new TestDummy(modelHead);
	result->SetPosition(_position);
	result->SetCollider(true);
	result->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));

	EntityManager::GetInstance()->AddEntity(result,true);
	/*
	result->Set(_position, _direction, m_fLifetime, m_fSpeed);
	result->SetStatus(true);
	result->SetCollider(true);
	result->SetSource(_source);
	EntityManager::GetInstance()->AddEntity(result);*/

	return result;
}