#include "Waypoint.h"
#include <limits>
#include <iostream>

CWaypoint::CWaypoint()
	: position(Vector3(0.0f, 0.0f, 0.0f))
	, m_iID(-1)
{
	relatedWaypoints.clear();
}

CWaypoint::~CWaypoint()
{
	// We clear the relatedWaypoints without deleting
	// as WaypointManager's destructor will delete the memory
	relatedWaypoints.clear();
}

void CWaypoint::SetPosition(Vector3 position)
{
	this->position = position;
}

Vector3 CWaypoint::GetPosition(void) const
{
	return position;
}

void CWaypoint::SetID(const int m_iID)
{
	this->m_iID = m_iID;
}

int CWaypoint::GetID(void) const
{
	return m_iID;
}

void CWaypoint::AddRelatedWaypoint(CWaypoint *relatedWaypoint)
{
	relatedWaypoints.push_back(relatedWaypoint);
}

bool CWaypoint::RemoveRelatedWaypoint(const int m_iID)
{
	// If Waypoints has related Waypoints, then we proceed to search
	if (relatedWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
		while (it != relatedWaypoints.end())
		{
			if ((*it)->GetID() == m_iID)
			{
				// We erase from the vector, relatedWaypoints, only. DO NOT DELETE
				it = relatedWaypoints.erase(it);
				return true;
			}
			it++;
		}
	}
	return false;
}

CWaypoint * CWaypoint::GetNearestWaypoint(void)
{
	CWaypoint *theNearestWaypoint = NULL;
	float m_fDistance = std::numeric_limits<float>::max();

	float xDistance;
	float zDistance;
	float distanceSquared;

	// If Waypoints has related Waypoints, then we process to search
	if (relatedWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
		while (it != relatedWaypoints.end())
		{
			Vector3 aRelatedWaypoint = (*it)->GetPosition();

			xDistance = position.x - aRelatedWaypoint.x;
			zDistance = position.z - aRelatedWaypoint.z;
			distanceSquared = (float)(xDistance*xDistance + zDistance*zDistance);
			if (m_fDistance > distanceSquared)
			{
				m_fDistance = distanceSquared;
				theNearestWaypoint = (CWaypoint*)(*it);
			}
			it++;
		}
	}
	return theNearestWaypoint;
}

int CWaypoint::GetNumberOfWaypoints(void) const
{
	return relatedWaypoints.size();
}

void CWaypoint::PrintSelf(void)
{
	std::cout << "Waypoint ID    : " << m_iID << std::endl;
	std::cout << " position      : " << position << std::endl;
	std::cout << " related waypoints : ";
	// If Waypoints has related Waypoints, then we proceed to search
	if (relatedWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
		while (it != relatedWaypoints.end())
		{
			std::cout << (*it)->GetID() << std::endl;
			it++;
		}
	}
	else
		std::cout << "NIL" << std::endl;
}
