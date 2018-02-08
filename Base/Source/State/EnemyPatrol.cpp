#include "EnemyPatrol.h"

EnemyPatrol::EnemyPatrol(const std::string & stateID)
	: State(stateID)
{
}

EnemyPatrol::~EnemyPatrol()
{
}

void EnemyPatrol::Enter(CEnemy *go)
{

}

void EnemyPatrol::Update(double dt, CEnemy * go)
{
	if ((go->GetTarget() - go->GetPos()).LengthSquared() < 25.0f)
	{
		CWaypoint *nextWaypoint = go->GetNextWaypoint();
		if (nextWaypoint)
			go->SetTarget(nextWaypoint->GetPosition());
		else
			go->SetTarget(Vector3(0.0f, 0.0f, 0.0f));
		std::cout << "Next target: " << go->GetTarget() << std::endl;
	}
}

void EnemyPatrol::Exit(CEnemy * go)
{
}
