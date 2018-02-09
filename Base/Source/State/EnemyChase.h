#ifndef	ENEMY_CHASE_H
#define ENEMY_CHASE_H

#include "State.h"
#include "../Enemy.h"

class EnemyChase : public State
{
	double chase_time;
public:
	EnemyChase(const std::string &stateID);
	~EnemyChase();

	virtual void Enter(CEnemy *go);
	virtual void Update(double dt, CEnemy *go);
	virtual void Exit(CEnemy *go);
};

#endif