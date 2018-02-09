#pragma once
#include "GenericEntity.h"
#include "GroundEntity.h"
#include "Waypoint\Waypoint.h"

#include <vector>

#include "Mesh.h"
#include "State\State.h"

//class Mesh;

class CEnemy : public GenericEntity
{
protected:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity *m_pTerrain;

	// Vector containing IDs of Waypoints
	std::vector<int> listOfWaypoints;
	// Current ID of Waypoint
	int m_iWaypointIndex;

	double m_dSpeed;
	double m_dAcceleration;

	// States
	State *m_currState;
	State *m_nextState;

	Mesh *model;
	bool active;
	std::string type;
	// previous waypoint
	CWaypoint *prev_waypoint;

public:
	CEnemy(void);
	virtual ~CEnemy();

	void Init(void);
	// Reset this player instance to defualt
	void Reset(void);

	// Set Position
	void SetPos(const Vector3 &pos);
	// Set Target
	void SetTarget(const Vector3 &target);
	// Set up
	void SetUp(const Vector3 &up);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity *m_pTerrain);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get up
	Vector3 GetUp(void) const;
	// Get the terrain for the player info
	GroundEntity *GetTerrain(void);
	// Get next Waypoint for this CEnemy
	CWaypoint *GetNextWaypoint(void);

	// Update
	void Update(double dt = 0.0333);
	
	// Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);

	void SetMesh(Mesh *model) { this->model = model; }

	bool GetActive(void) const { return active; }
	void SetActive(const bool active) { this->active = active; }

	std::string GetType(void) const { return type; }
	void SetType(const std::string &type) { this->type = type; }

	// States
	void SetCurrentState(State *m_currState) { this->m_currState = m_currState; }
	void SetNextState(State *m_nextState) { this->m_nextState = m_nextState; }

	State *GetCurrentState(void) { return m_currState; }
	State *GetNextState(void) { return m_nextState; }

	// Set previous waypoint
	void SetPreviousWaypoint(CWaypoint *waypoint);
	CWaypoint *GetPreviousWaypoint(void);
};