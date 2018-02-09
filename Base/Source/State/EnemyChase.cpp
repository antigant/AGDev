#include "EnemyChase.h"
#include "StateMachineManager.h"
#include "../PlayerInfo/PlayerInfo.h"

#define TIME 5.0f

EnemyChase::EnemyChase(const std::string &stateID)
	: State(stateID)
{
}

EnemyChase::~EnemyChase()
{
}

void EnemyChase::Enter(CEnemy *go)
{
	chase_time = 0.0f;
}

void EnemyChase::Update(double dt, CEnemy *go)
{
	chase_time += dt;
	if (chase_time > TIME)	// stop chasing, go back to patrol state
		StateMachineManager::GetInstance()->SetNextState(go, "enemy_patrol");

	go->SetTarget(CPlayerInfo::GetInstance()->GetPos());
}

void EnemyChase::Exit(CEnemy *go)
{
	CWaypoint *nextWaypoint = go->GetPreviousWaypoint();
	if (nextWaypoint)
		go->SetTarget(nextWaypoint->GetPosition());
	else
		go->SetTarget(Vector3(0.0f, 0.0f, 0.0f));
}