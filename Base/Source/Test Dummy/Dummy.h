#pragma once
#include "EntityBase.h"
#include "../SceneGraph.h"
#include "Collider\Collider.h"
#include <string>
using namespace std;


class Mesh;

class TestDummy: public EntityBase,public CCollider
{
public:
	TestDummy();
	TestDummy(Mesh *part);
	~TestDummy();

	void Init();
	virtual void Update(double dt);
	virtual void Render();	
	virtual void SetIsDone(const bool _value);

	void SetName(const string Name);
	string GetName()const;
	void SetID(const int ID);
	int GetID()const;
	
private:
	string name;
	int ID;
	Mesh *parts;
	bool isHit;
	//list of entity it has(if lose an arm)
	//vector<CSceneNode> nodelist;
};

namespace Create
{
	//call parts 
	//TestDummy *Torso(const std::string& _meshHead, const std::string& _meshBody, const Vector3& _position);
	TestDummy *dummy_part(const std::string& _meshHead, const Vector3& _position);
		//const Vector3& _direction);
}