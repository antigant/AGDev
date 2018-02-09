#include "Enemy.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "Waypoint\WaypointManager.h"

CEnemy::CEnemy(void)
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.f, 0.f, 0.f))
	, defaultTarget(Vector3(0.f, 0.f, 0.f))
	, defaultUp(Vector3(0.f, 0.f, 0.f))
	, target(Vector3(0.f, 0.f, 0.f))
	, up(Vector3(0.f, 0.f, 0.f))
	, maxBoundary(Vector3(0.f, 0.f, 0.f))
	, minBoundary(Vector3(0.f, 0.f, 0.f))
	, m_pTerrain(NULL)
	, m_iWaypointIndex(-1)
	, m_currState(NULL)
	, m_nextState(NULL)
	, active(false)
	, type("No Type")
	, prev_waypoint(NULL)
{
	listOfWaypoints.clear();
}

CEnemy::~CEnemy()
{
}

void CEnemy::Init(void)
{
	// Set the default values
	defaultPosition.Set(0.f, 0.f, 10.f);
	defaultTarget.Set(0.f, 0.f, 0.f);
	defaultTarget.Set(0.f, 1.f, 0.f);

	// Set up the waypoints
	listOfWaypoints.push_back(0);
	listOfWaypoints.push_back(1);
	listOfWaypoints.push_back(2);

	m_iWaypointIndex = 0;

	// Set the current values
	position.Set(10.f, 0.f, 0.f);
	//target.Set(10.f, 0.f, 450.f);
	CWaypoint *nextWaypoint = GetNextWaypoint();
	if (nextWaypoint)
		target = nextWaypoint->GetPosition();
	else
		target.SetZero();
	std::cout << "Next target: " << target << std::endl;
	up.Set(0.f, 1.f, 0.f);

	// Set Boundary
	maxBoundary.Set(1.f, 1.f, 1.f);
	minBoundary.Set(-1.f, -1.f, -1.f);

	// Set speed
	m_dSpeed = 20.0;

	// Initialised the LOD meshes
	InitLOD("cube", "sphere", "cubeSG");

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1.f, 1.f, 1.f), Vector3(-1.f, -1.f, -.1f));

	scale.Set(10.0f, 10.0f, 10.0f);

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);
}

// Reset this player instance to defualt
void CEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set Position
void CEnemy::SetPos(const Vector3 &pos)
{
	position = pos;
}

// Set Target
void CEnemy::SetTarget(const Vector3 &target)
{
	this->target = target;
}

// Set up
void CEnemy::SetUp(const Vector3 &up)
{
	this->up = up;
}

// Set the boundary for the player info
void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Set the terrain for the player info
void CEnemy::SetTerrain(GroundEntity *m_pTerrain)
{
	if (m_pTerrain)
	{
		this->m_pTerrain = m_pTerrain;
		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 CEnemy::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CEnemy::GetTarget(void) const
{
	return target;
}

// Get up
Vector3 CEnemy::GetUp(void) const
{
	return up;
}

// Get the terrain for the player info
GroundEntity *CEnemy::GetTerrain(void)
{
	return m_pTerrain;
}

// Get next Waypoint for this CEnemy
CWaypoint *CEnemy::GetNextWaypoint(void)
{
	if ((int)listOfWaypoints.size() > 0)
	{
		m_iWaypointIndex++;
		if (m_iWaypointIndex >= (int)listOfWaypoints.size())
			m_iWaypointIndex = 0;
		return CWaypointManager::GetInstance()->GetWaypoint(listOfWaypoints[m_iWaypointIndex]);
	}
	else
		return NULL;
}

// Update
void CEnemy::Update(double dt)
{
	Vector3 viewVector = (target - position).Normalized();
	position += viewVector * (float)m_dSpeed * (float)dt;

	// Constain the position
	Constrain();

	// Update the target
	//if (position.z > 400.f)
	//	target.z = position.z * -1.f;
	//else if (position.z < -400.f)
	//	target.z = position.z * -1.f;

	//if ((target - position).LengthSquared() < 25.0f)
	//{
	//	CWaypoint *nextWaypoint = GetNextWaypoint();
	//	if (nextWaypoint)
	//		target = nextWaypoint->GetPosition();
	//	else
	//		target.SetZero();
	//	std::cout << "Next target: " << target << std::endl;
	//}
}

// Constrain the position within the borders
void CEnemy::Constrain(void)
{
	if (position.x > maxBoundary.x - 1.f)
		position.x = maxBoundary.x - 1.f;
	if (position.z > maxBoundary.z - 1.f)
		position.z = maxBoundary.z - 1.f;
	if (position.x < minBoundary.x + 1.f)
		position.x = minBoundary.x + 1.f;
	if (position.z < minBoundary.z + 1.f)
		position.z = minBoundary.z + 1.f;

	// if the y position is not equal to terrain height at that position
	// update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

// Render
void CEnemy::Render(void)
{
	MS &modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	//if (GetLODStatus())
	//{
	//	if (theDetailLevel != NO_DETAILS)
	//		RenderHelper::RenderMesh(GetLODMesh());
	//}
	RenderHelper::RenderMesh(model);
	modelStack.PopMatrix();
}

void CEnemy::SetPreviousWaypoint(CWaypoint *waypoint)
{
	prev_waypoint = waypoint;
}

CWaypoint *CEnemy::GetPreviousWaypoint(void)
{
	return prev_waypoint;
}
