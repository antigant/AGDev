#pragma once
#include "EntityBase.h"

class CParticles : public EntityBase
{
public:
	CParticles(void);
	~CParticles(void);

	virtual void Update(double _dt);
	virtual void Render();

private:

};