#include "EnemyPatrol.h"
#include "StateMachineManager.h"
#include "../PlayerInfo/PlayerInfo.h"

#define CHASE_DISTANCE 40.0f

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
	if ((CPlayerInfo::GetInstance()->GetPos() - go->GetPos()).LengthSquared() < CHASE_DISTANCE * CHASE_DISTANCE)
		StateMachineManager::GetInstance()->SetNextState(go, "enemy_chase");

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
