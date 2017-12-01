#include "LevelOfDetails.h"
#include "MeshBuilder.h"

CLevelOfDetails::CLevelOfDetails()
	: modelMesh_High(NULL)
	, modelMesh_Mid(NULL)
	, modelMesh_Low(NULL)
	, m_bActive(false)
	, theDetailLevel(HIGH_DETAILS)
{
}

CLevelOfDetails::~CLevelOfDetails()
{
	modelMesh_High = NULL;
	modelMesh_Mid = NULL;
	modelMesh_Low = NULL;
}


bool CLevelOfDetails::InitLOD(const std::string &meshName_High,
	const std::string &meshName_Mid,
	const std::string &meshName_Low)
{
	// REtrive the Meshes from MeshBuilder's library and assign to the 3 Mesh pointers
	modelMesh_High = MeshBuilder::GetInstance()->GetMesh(meshName_High);
	modelMesh_Mid = MeshBuilder::GetInstance()->GetMesh(meshName_Mid);
	modelMesh_Low = MeshBuilder::GetInstance()->GetMesh(meshName_Low);

	// If any of the mesh is not loaded, the return false and avoid setting the LOD to active
	if ((modelMesh_High == nullptr) || (modelMesh_Mid == nullptr) || (modelMesh_Low == nullptr))
		return false;

	// Set the LOD to active
	SetLODStatus(true);
	return true;
}

bool CLevelOfDetails::DestoryLOD(void)
{
	if (modelMesh_High)
	{
		delete modelMesh_High;
		modelMesh_High = NULL;
	}
	if (modelMesh_Mid)
	{
		delete modelMesh_Mid;
		modelMesh_Mid = NULL;
	}
	if (modelMesh_Low)
	{
		delete modelMesh_Low;
		modelMesh_Low = NULL;
	}

	return false;
}


void CLevelOfDetails::SetLODStatus(const bool bActive)
{
	m_bActive = true;
}

bool CLevelOfDetails::GetLODStatus(void) const
{
	return m_bActive;
}

bool CLevelOfDetails::SetLODMesh(Mesh *theMesh, const DETAIL_LEVEL theDetailLevel)
{
	if (theDetailLevel == HIGH_DETAILS)
		modelMesh_High = theMesh;
	else if (theDetailLevel == MID_DETAILS)
		modelMesh_Mid = theMesh;
	else if (theDetailLevel == LOW_DETAILS)
		modelMesh_Low = theMesh;
	else
		return false;
	return true;
}

Mesh *CLevelOfDetails::GetLODMesh(void) const
{
	if (theDetailLevel == HIGH_DETAILS)
		return modelMesh_High;
	else if (theDetailLevel == MID_DETAILS)
		return modelMesh_Mid;
	else if (theDetailLevel == LOW_DETAILS)
		return modelMesh_Low;
	return NULL;
}

Mesh *CLevelOfDetails::GetLODMesh(const DETAIL_LEVEL theDetailLevel) const
{
	if (theDetailLevel == HIGH_DETAILS)
		return modelMesh_High;
	else if (theDetailLevel == MID_DETAILS)
		return modelMesh_Mid;
	else if (theDetailLevel == LOW_DETAILS)
		return modelMesh_Low;
	return NULL;
}

int CLevelOfDetails::GetDetailLevel(void) const
{
	return theDetailLevel;
}

bool CLevelOfDetails::SetDetailLevel(const DETAIL_LEVEL theDetailLevel)
{
	if ((theDetailLevel >= NO_DETAILS) && (theDetailLevel < NUM_DETAIL_LEVEL))
	{
		this->theDetailLevel = theDetailLevel;
		return true;
	}
	return false;
}