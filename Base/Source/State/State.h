#ifndef STATE_H
#define STATE_H

#include <string>

class CEnemy;

class State
{
	const std::string m_stateID;
protected:
	State(const std::string &stateID);
public:
	virtual ~State();
	const std::string& GetStateID();

	//To be implemented by concrete states
	virtual void Enter(CEnemy *go) = 0;
	virtual void Update(double dt, CEnemy *go) = 0;
	virtual void Exit(CEnemy *go) = 0;
};

#endif