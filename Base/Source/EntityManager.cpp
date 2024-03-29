#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "Projectile\Laser.h"

#include <iostream>
#include "SceneGraph.h"

using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Update();

	//Render the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Update();
	// Check for Collision amongst entities with collider properties
	CheckForCollision();

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			CSceneGraph::GetInstance()->DeleteNode(*it);


			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Render();
	}

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Render();

	//Render the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Render();
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
//void EntityManager::AddEntity(EntityBase* _newEntity)
//{
//	entityList.push_back(_newEntity);
//}
//Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase *_newEntity, bool bAddToSpatialPartition)
{
	entityList.push_back(_newEntity);

	//Add to the Spatial Partition
	if (theSpatialPartition && bAddToSpatialPartition)
		theSpatialPartition->Add(_newEntity);
}
// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);

		//Remove from SceneNode too
		if (!CSceneGraph::GetInstance()->DeleteNode(_existingEntity))
		{
			cout << "EntityManager::RemoveEntity: Unable to remove SceneNode" << endl;
		}
		else
		{
			//Remove from the Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEntity);
		}

		return true;	
	}
	// Return false if not found
	return false;
}

bool EntityManager::MarkForDeletion(EntityBase * _existingEntity)
{
	//Find the Entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	//Delete the entity if found
	if (findIter != entityList.end())
	{
		(*findIter)->SetIsDone(true);
		return true;
	}
	//Return false if not found
	return false;
}

void EntityManager::SetSpatialPartition(CSpatialPartition * theSpatialPartition)
{
	this->theSpatialPartition = theSpatialPartition;
}

void EntityManager::Clean()
{
	while (!entityList.empty())
	{
		delete entityList.back();
		entityList.back() = NULL;
		entityList.pop_back();
	}
}

// Constructor
EntityManager::EntityManager()
	: theSpatialPartition(NULL)
{
}

// Destructor
EntityManager::~EntityManager()
{
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{	
	//// Check if this object is overlapping that object
	//if (((thatMinAABB.x >= thisMinAABB.x) && (thatMinAABB.x <=thisMaxAABB.x) &&
	//	(thatMinAABB.y >= thisMinAABB.y) && (thatMinAABB.y <= thisMaxAABB.y) &&
	//	(thatMinAABB.z >= thisMinAABB.z) && (thatMinAABB.z <= thisMaxAABB.z))
	//	||
	//	((thatMaxAABB.x >= thisMinAABB.x) && (thatMaxAABB.x <= thisMaxAABB.x) &&
	//	(thatMaxAABB.y >= thisMinAABB.y) && (thatMaxAABB.y <= thisMaxAABB.y) &&
	//	(thatMaxAABB.z >= thisMinAABB.z) && (thatMaxAABB.z <= thisMaxAABB.z)))
	//	return true;	
	//// Check if that object is overlapping this object
	//if (((thisMinAABB.x >= thatMinAABB.x) && (thisMinAABB.x <= thatMaxAABB.x) &&
	//	(thisMinAABB.y >= thatMinAABB.y) && (thisMinAABB.y <= thatMaxAABB.y) &&
	//	(thisMinAABB.z >= thatMinAABB.z) && (thisMinAABB.z <= thatMaxAABB.z))
	//	||
	//	((thisMaxAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	//	(thisMaxAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	//	(thisMaxAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z)))
	//	return true;
	//// Check if this object is within that object
	//if (((thisMinAABB.x >= thatMinAABB.x) && (thisMinAABB.x <= thatMaxAABB.x) &&
	//	(thisMinAABB.y >= thatMinAABB.y) && (thisMinAABB.y <= thatMaxAABB.y) &&
	//	(thisMinAABB.z >= thatMinAABB.z) && (thisMinAABB.z <= thatMaxAABB.z))
	//	&&
	//	((thisMaxAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	//	(thisMaxAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	//	(thisMaxAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z)))
	//	return true;
	//// Check if that object is within this object
	//if (((thatMinAABB.x >= thisMinAABB.x) && (thatMinAABB.x <= thisMaxAABB.x) &&
	//	(thatMinAABB.y >= thisMinAABB.y) && (thatMinAABB.y <= thisMaxAABB.y) &&
	//	(thatMinAABB.z >= thisMinAABB.z) && (thatMinAABB.z <= thisMaxAABB.z))
	//	&&
	//	((thatMaxAABB.x >= thisMinAABB.x) && (thatMaxAABB.x <= thisMaxAABB.x) &&
	//	(thatMaxAABB.y >= thisMinAABB.y) && (thatMaxAABB.y <= thisMaxAABB.y) &&
	//	(thatMaxAABB.z >= thisMinAABB.z) && (thatMaxAABB.z <= thisMaxAABB.z)))
	//	return true;

	// Check if this object is overlapping that object
	if (thatMinAABB >= thisMinAABB && thatMinAABB <= thisMaxAABB
		||
		thatMaxAABB >= thisMinAABB && thatMaxAABB <= thisMaxAABB)
		return true;

	// Check if that object is overlapping this object
	if (thisMinAABB >= thatMinAABB && thisMinAABB <= thatMaxAABB
		||
		thisMaxAABB >= thatMinAABB && thisMaxAABB <= thatMaxAABB)
		return true;

	// Check if this object is within that object
	if (thisMinAABB >= thatMinAABB && thisMinAABB <= thatMaxAABB
		&&
		thisMaxAABB >= thatMinAABB && thisMaxAABB <= thatMaxAABB)
		return true;

	// Check if that object is within this object
	if (thatMinAABB >= thisMinAABB && thatMinAABB <= thisMaxAABB
		&&
		thatMaxAABB >= thisMinAABB && thatMaxAABB <= thisMaxAABB)
		return true;

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entites
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entitiy
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// if Radius of bounding sphere of ThisEntity plus Radius of bounding sphere of ThatEntity is
	// greater than the disance squared between the 2 reference points of the 2 entities,
	// then it could mean that there are colliding with each other.
	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) > DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()))
		return true;

	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entties
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entitiy
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// Check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	// if AABB collision check fails, then we need to check the other corners of the bounding boxes to
	// do more collision checks with other points on each bounding box

	//Vector3 altThisMinAABB, altThisMaxAABB;
	//altThisMaxAABB.Set(thisMaxAABB.x - thisCollider->GetMaxAABB().x, thisMaxAABB.y - thisCollider->GetMaxAABB().y, thisMaxAABB.z - thisCollider->GetMaxAABB().z);
	//altThisMinAABB.Set(thisMinAABB.x - thisCollider->GetMinAABB().x, thisMinAABB.y - thisCollider->GetMinAABB().y, thisMinAABB.z - thisCollider->GetMinAABB().z);

	Vector3 altXThisMinAABB, altXThisMaxAABB, altYThisMinAABB, altYThisMaxAABB, altZThisMinAABB, altZThisMaxAABB;
	altXThisMinAABB.Set(thisMaxAABB.x, thisMinAABB.y, thisMinAABB.z);
	altXThisMaxAABB.Set(thisMinAABB.x, thisMaxAABB.y, thisMaxAABB.z);

	altYThisMinAABB.Set(thisMinAABB.x, thisMaxAABB.y, thisMinAABB.z);
	altYThisMaxAABB.Set(thisMaxAABB.x, thisMinAABB.y, thisMaxAABB.z);

	altZThisMinAABB.Set(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	altZThisMaxAABB.Set(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);
	// Check for overlap
	if (CheckOverlap(altXThisMinAABB, altXThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;
	if (CheckOverlap(altYThisMinAABB, altYThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;
	if (CheckOverlap(altZThisMinAABB, altZThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}

// Check for intersection between a line segment and a plane
bool EntityManager::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit)
{
	if (fDst1 * fDst2 >= 0.f)
		return false;
	if (fDst1 == fDst2)
		return false;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return true;
}

// Check for intersection between a line segment and a plane
bool EntityManager::CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end, Vector3 minAABB, Vector3 maxAABB, Vector3 &Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y - minAABB.y, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z - minAABB.z, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x - maxAABB.x, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y - maxAABB.y, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z - maxAABB.z, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 3)))
		return true;

	return false;
}

// Check two position are within a box region
bool EntityManager::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
	return false;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(void)
{
	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = entityList.end();
	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		// Add spartial partition codes here

		// Check if this entity is a CLaser type
		if ((*colliderThis)->GetIsLaser())
		{
			// Dyanmic cast it to a CLaser class
			CLaser *thisEntity = dynamic_cast<CLaser*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					Vector3 hitPosition;

					// Get the minAABB and maxAABB for (*colliderThat)
					CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
					Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
					Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();

					if (CheckLineSegmentPlane(thisEntity->GetPosition(), thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(), thatMinAABB, thatMaxAABB, hitPosition))
					{
						(*colliderThis)->SetIsDone(true);
						(*colliderThat)->SetIsDone(true);

						// Remove from Scene Graph
						if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)))
							cout << "*** This Entity removed" << endl;

						// Remove from Scene Graph
						if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)))
							cout << "*** That Entity removed" << endl;
					}
				}
			}
		}

		else if ((*colliderThis)->HasCollider())
		{
			// This object was derived from a CCollider class, then it will have Collision Detection methods
			// CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = colliderThis; colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					// This object was derived from a collider class, then it will have Collision Detection methods
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
					if (CheckSphereCollision(thisEntity, thatEntity))
					{
						if (CheckAABBCollision(thisEntity, thatEntity))
						{
							thisEntity->SetIsDone(true);
							thatEntity->SetIsDone(true);
							cout << "Collided" << endl;
						}
					}
				}
			}
		}
	}

	return false;
}

