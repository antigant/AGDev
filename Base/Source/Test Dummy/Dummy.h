#pragma once
//#include "EntityBase.h"
#include "../SceneGraph.h"
#include <string>
using namespace std;


class Mesh;

class TestDummy: public CSceneGraph
{
public:
	TestDummy();
	~TestDummy();

	void Init();
	void Update();
	void Exit();

	void SetName(const string Name);
	string GetName()const;
	void SetID(const int ID);
	int GetID()const;
private:
	string name;
	int ID;
	//list of entity it has(if lose an arm)
	vector<CSceneNode> nodelist;
};

TestDummy::TestDummy()
{
}

TestDummy::~TestDummy()
{
}