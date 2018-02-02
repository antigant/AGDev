#pragma once

#include "Vector3.h"
#include <vector>

class CWaypoint
{
public:
	CWaypoint();
	virtual ~CWaypoint();

	// Set position
	void SetPosition(Vector3 position);
	// Get position
	Vector3 GetPosition(void) const;

	// Set ID
	void SetID(const int m_iID);
	// Get ID
	int GetID(void) const;

	// Add related waypoint
	void AddRelatedWaypoint(CWaypoint *relatedWaypoint);
	// Remove related waypoint
	bool RemoveRelatedWaypoint(const int m_iID);
	// Get nearest waypoint amongst related Waypoints
	CWaypoint *GetNearestWaypoint(void);
	// Get the number of related Waypoints
	int GetNumberOfWaypoints(void) const;

	// Print self
	void PrintSelf(void);

protected:
	Vector3 position;
	int m_iID;

	std::vector<CWaypoint*> relatedWaypoints;
};