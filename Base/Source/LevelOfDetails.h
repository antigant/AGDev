#pragma once
#include "Vector3.h"
#include "Mesh.h"

class CLevelOfDetails
{
public:
	Mesh* modelMesh_High;
	Mesh* modelMesh_Mid;
	Mesh* modelMesh_Low;

	enum DETAIL_LEVEL
	{
		NO_DETAILS = 0,
		HIGH_DETAILS,
		MID_DETAILS,
		LOW_DETAILS,
		NUM_DETAIL_LEVEL,
	};

	CLevelOfDetails();
	virtual ~CLevelOfDetails();

	bool InitLOD(	const std::string &meshName_High,
					const std::string &meshName_Mid,
					const std::string &meshName_Low);
	bool DestoryLOD(void);

	void SetLODStatus(const bool bActive);
	bool GetLODStatus(void) const;

	bool SetLODMesh(Mesh *theMesh, const DETAIL_LEVEL theDetailLevel);
	Mesh *GetLODMesh(void) const;
	Mesh *GetLODMesh(const DETAIL_LEVEL theDetailLevel) const;
	int GetDetailLevel(void) const;
	bool SetDetailLevel(const DETAIL_LEVEL theDetailLevel);

protected:
	bool m_bActive;
	DETAIL_LEVEL theDetailLevel;
};