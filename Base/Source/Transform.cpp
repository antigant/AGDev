#include "Transform.h"
#include <iostream>

// Default constructor
CTransform::CTransform(void)
	: theUpdateTransformation(NULL)
{
	Mtx.SetToIdentity();
	DefaultMtx.SetToIdentity();
}

// Overloaded constructor
CTransform::CTransform(const float dx, const float dy, const float dz)
{
	Mtx.SetToTranslation(dx, dy, dz);
}

// Destructor
CTransform::~CTransform(void)
{
	if (theUpdateTransformation)
	{
		delete theUpdateTransformation;
		theUpdateTransformation = NULL;
	}
}

// Apply a translation to the Transformation Matrix
void CTransform::ApplyTranslate(const float dx, const float dy, const float dz)
{
	Mtx44 TempMtx;
	TempMtx.SetToTranslation(dx, dy, dz);
	Mtx = Mtx * TempMtx;
}

// Get the translation from the Transfromation Matrix
void CTransform::GetTranslate(float &x, float &y, float &z)
{
	x = Mtx.a[12];
	y = Mtx.a[13];
	z = Mtx.a[14];
}

// Apply a rotation to the Transformation Matrix
void CTransform::ApplyRotate(const float angle, const float rx, const float ry, const float rz)
{
	Mtx44 TempMtx;
	TempMtx.SetToRotation(angle, rx, ry, rz);
	Mtx = Mtx * TempMtx;
}

// Get the rotation from the Transformation Matrix
float CTransform::GetRotate(const AXIS theAxis)
{
	if (theAxis == X_AXIS)
	{
		float y = acos(Mtx.a[5]);			// Equals to cos a
		float z = asin(-Mtx.a[9]);			// Equals to sin a

		if (abs(y - z) < Math::EPSILON)
			return y;
	}
	else if (theAxis == Y_AXIS)
	{
		float x = acos(Mtx.a[0]);		    // Equals to cos a
		float z = asin(Mtx.a[8]);		    // Equals to -sin a

		if (abs(x - z) < Math::EPSILON)
			return x;
	}
	else if (theAxis == Z_AXIS)
	{
		float x = acos(Mtx.a[0]);			// Equals to cos a
		float y = asin(-Mtx.a[4]);			// Equals to -sin a

		if (abs(x - y) < Math::EPSILON)
			return x;
	}
	return 0.f;
}

// Set the scale of the Transformation Matrix
void CTransform::SetScale(const float sx, const float sy, const float sz)
{
	float scaleX = sx, scaleY = sy, scaleZ = sz;
	if (scaleX == 0.f)
		scaleX = 1.f;
	if (scaleY == 0.f)
		scaleY = 1.f;
	if (scaleZ == 0.f)
		scaleZ == 1.f;

	Mtx.SetToScale(scaleX, scaleY, scaleZ);
}

// Get the scale from the Transformation Matrix
void CTransform::GetScale(float &x, float &y, float &z)
{
	x = Mtx.a[0];
	y = Mtx.a[5];
	z = Mtx.a[10];
}

// Apply a Transformation Matrix to the Transformation Matrix here
void CTransform::ApplyTransform(Mtx44 newMTX)
{
	Mtx = Mtx * newMTX;
}

// Reset the transformation matrix to identity matrix
void CTransform::Reset(void)
{
	Mtx.SetToIdentity();
}

// Get the transformation matrix
Mtx44 CTransform::GetTransform(void)
{
	return Mtx;
}

// Set the Update Transformation
void CTransform::SetUpdateTransformation(CUpdateTransformation *theUpdateTransformation)
{
	this->theUpdateTransformation = theUpdateTransformation;
}

// Get the update transformation matrix
Mtx44 CTransform::GetUpdateTransform(void)
{
	if (!theUpdateTransformation)
		return DefaultMtx;

	theUpdateTransformation->Update();

	return theUpdateTransformation->GetUpdateTransformation();
}

// Print self
void CTransform::PrintSelf(void) const
{
	std::cout << "=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+" << std::endl;
	std::cout << "CTransform::PrintSelf" << std::endl;
	std::cout << "[\t" << Mtx.a[0] << "\t" << Mtx.a[4] << "\t" << Mtx.a[8] << "\t" << Mtx.a[12] << "\t]" << std::endl;
	std::cout << "[\t" << Mtx.a[1] << "\t" << Mtx.a[5] << "\t" << Mtx.a[9] << "\t" << Mtx.a[13] << "\t]" << std::endl;
	std::cout << "[\t" << Mtx.a[2] << "\t" << Mtx.a[6] << "\t" << Mtx.a[10] << "\t" << Mtx.a[14] << "\t]" << std::endl;
	std::cout << "[\t" << Mtx.a[3] << "\t" << Mtx.a[7] << "\t" << Mtx.a[11] << "\t" << Mtx.a[15] << "\t]" << std::endl;	
	std::cout << "=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+" << std::endl;
}