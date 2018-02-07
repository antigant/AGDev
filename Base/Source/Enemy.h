#pragma once
#include "GenericEntity.h"
#include "GroundEntity.h"
#include "Waypoint\Waypoint.h"

#include <vector>

class Mesh;

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
};