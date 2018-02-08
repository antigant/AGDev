#include "WaypointManager.h"
#include <iostream>

CWaypointManager *CWaypointManager::s_instance = 0;

CWaypointManager::CWaypointManager(void)
	: m_iID(-1)
{
	listOfWaypoints.clear();
}

CWaypointManager::~CWaypointManager()
{
	// If listOfWaypoints is not empty, then proceed to delete
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		// Destroy the children first
		std::vector<CWaypoint*>::iterator it;
		for (it = listOfWaypoints.begin(); it != listOfWaypoints.end(); ++it)
			delete *it;
	}
	// We delete the waypoints here
	listOfWaypoints.clear();
}

int CWaypointManager::AddWaypoint(Vector3 position)
{
	++m_iID;
	CWaypoint *aNewWaypoint = new CWaypoint();
	aNewWaypoint->SetID(m_iID);
	aNewWaypoint->SetPosition(position);
	listOfWaypoints.push_back(aNewWaypoint);
	return m_iID;
}

int CWaypointManager::AddWaypoint(const int m_iID, Vector3 position)
{
	// If Waypoints is not empty, then we proceed to search
	if (listOfWaypoints.size() != 0)
	{
		for (unsigned i = 0; i < listOfWaypoints.size(); ++i)
		{
			CWaypoint *aWaypoint = listOfWaypoints[i];

			if (aWaypoint->GetID() == m_iID)
			{
				++(this->m_iID);
				CWaypoint *aNewWaypoint = new CWaypoint();
				aNewWaypoint->SetID(this->m_iID);
				aNewWaypoint->SetPosition(position);
				listOfWaypoints.push_back(aNewWaypoint);

				// Add the current Waypoint as a related waypoint to the new Waypoint
				aNewWaypoint->AddRelatedWaypoint(aWaypoint);
				// Add this new Waypoint as a related waypoint to the current Waypoint
				aWaypoint->AddRelatedWaypoint(aNewWaypoint);

				return this->m_iID;
			}
		}
	}
	return -1;
}

bool CWaypointManager::RemoveWaypoint(const int m_iID)
{
	bool result = false;

	// If Waypoint is not empty, then we proceed to search
	if (listOfWaypoints.size() != 0)
	{
		// We iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
		while (it != listOfWaypoints.end())
		{
			if ((*it)->GetID() == m_iID)
			{
				// Remove all references to this Waypoint from other Waypoints
				RemoveRelatedWaypoint(m_iID);

				// Remove this Waypoint from the WaypointManager
				delete *it;
				it = listOfWaypoints.erase(it);
				result = true;
				break;
			}
			if (it != listOfWaypoints.end())
				it++;
		}
	}
	return result;
}

CWaypoint * CWaypointManager::GetWaypoint(const int m_iID)
{
	// If Waypoints is not empty, then we proceed to search
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
		while (it != listOfWaypoints.end())
		{
			if ((*it)->GetID() == m_iID)
				return (CWaypoint*)*it;
			it++;
		}
	}
	return NULL;
}

CWaypoint * CWaypointManager::GetNearestWaypoint(Vector3 aPosition)
{
	CWaypoint *theNearestWaypoint = NULL;
	float m_fDistance = std::numeric_limits<float>::max();

	float xDistance;
	float zDistance;
	float distanceSquared;

	// If Waypoints has related Waypoints, then we proceed to search.
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
		while (it != listOfWaypoints.end())
		{
			Vector3 aRelatedWaypoint = (*it)->GetPosition();

			xDistance = aPosition.x - aRelatedWaypoint.x;
			zDistance = aPosition.z - aRelatedWaypoint.z;
			distanceSquared = (float)(xDistance*xDistance + zDistance*zDistance);
			if (m_fDistance > distanceSquared)
			{
				m_fDistance = distanceSquared;
				theNearestWaypoint = (CWaypoint*)*it;
			}
			it++;
		}
	}
	return theNearestWaypoint;
}

int CWaypointManager::GetNumberOfWaypoints(void) const
{
	return listOfWaypoints.size();
}

void CWaypointManager::PrintSelf(void)
{
	std::cout << "===============================================================" << std::endl;
	std::cout << "CWaypointManager::PrintSelf" << std::endl;
	// If Waypoints has related Waypoints, then we proceed to search.
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
		while (it != listOfWaypoints.end())
		{
			(*it)->PrintSelf();
			it++;
		}
	}
	std::cout << "===============================================================" << std::endl;
}

void CWaypointManager::RemoveRelatedWaypoint(const int m_iID)
{
	// Iterate through all the Waypoints
	std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
	while (it != listOfWaypoints.end())
	{
		(*it)->RemoveRelatedWaypoint(m_iID);
		it++;
	}
}