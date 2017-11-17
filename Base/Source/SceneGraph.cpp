#include "SceneGraph.h"

CSceneGraph::CSceneGraph()
	: ID(0)
	, theRoot(NULL)
{
	theRoot = new CSceneNode();
	// Assign the first ID to the root. Default is 0
	theRoot->SetID(this->GenerateID());
}

CSceneGraph::~CSceneGraph()
{
}

// Update the Scene Graph
void CSceneGraph::Update(void)
{
	theRoot->Update();
}

// Render the Scene Graph
void CSceneGraph::Render(void)
{
	theRoot->Render();
}


void CSceneGraph::Destroy()
{
	if (theRoot)
	{
		theRoot->DeleteAllChildren();
		delete theRoot;
	}
	Singleton<CSceneGraph>::Destroy();
}
//Get the root of this Scene Graph
CSceneNode *CSceneGraph::GetRoot()
{
	return theRoot;
}
//Add a Node to this Scene Graph
CSceneNode *CSceneGraph::AddNode(EntityBase *theEntity)
{
	CSceneNode* aNewSceneNode = theRoot->AddChild(theEntity);
	//aNewSceneNode->SetID(this->GenerateID());
	return aNewSceneNode;
}

// Delete a Node from this Scene Graph using the pointer to the node
bool CSceneGraph::DeleteNode(EntityBase *theEntity)
{
	return theRoot->DeleteChild(theEntity);
}

// Delete a Node from this Scene Graph using its ID
bool CSceneGraph::DeleteNode(const int ID)
{
	return theRoot->DeleteChild(ID);
}

//Detach a Node from this Scene Graph using the pointer to the node
CSceneNode *CSceneGraph::DetachNode(CSceneNode *theNode)
{
	//---------------------------------need check
	//return NULL;// theRoot->DetachChild(theNode);
	return theRoot->DetachChild(theNode->GetEntity());
}

//Detach a Node from this Scene Graph using its ID
CSceneNode *CSceneGraph::DetachNode(const int ID)
{
	return theRoot->DetachChild(ID);
}

//Get a Node using the pointer to the node
CSceneNode *CSceneGraph::GetNode(EntityBase *theEntity)
{
	return theRoot->GetEntity(theEntity);
}

//Get a Node using its ID
CSceneNode *CSceneGraph::GetNode(const int ID)
{
	return theRoot->GetEntity(ID);
}


//Return the number of nodes in this Scene Graph
int CSceneGraph::GetNumOfNode(void)
{
	return theRoot->GetNumOfChild();
}


//Generate an ID for a Scene Node
int CSceneGraph::GenerateID(void)
{
	//Return the ID and increment the value by 1
	return ID++;
}
//PrintSelf for debug purposes
inline void CSceneGraph::PrintSelf(void)
{
	theRoot->PrintSelf();
}
