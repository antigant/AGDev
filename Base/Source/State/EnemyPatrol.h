#ifndef	ENEMY_PATROL_H
#define ENEMY_PATROL_H

#include "State.h"
#include "../Enemy.h"

class EnemyPatrol : public State
{
public:
	EnemyPatrol(const std::string &stateID);
	~EnemyPatrol();

	virtual void Enter(CEnemy *go);
	virtual void Update(double dt, CEnemy *go);
	virtual void Exit(CEnemy *go);
};

#endif