// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// 
// Copyright 2015 Heiko Fink, All Rights Reserved.
//
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
// 
// Perception Neuron (TM) is a trademark of Beijing Noitom Technology Ltd.
// 
// Description:
// This header capsules some math needed by the Perception Neuron (TM) plugin.
// 

#pragma once
#include "Engine.h"

// Supported rotation matrices
enum ChannelOrderEnum { XYZ, XZY, YXZ, YZX, ZXY, ZYX };

static FORCEINLINE FQuat CalculateQuat(float XR, float YR, float ZR, ChannelOrderEnum RotOrder)
{
	float SX = sin(XR);
	float CX = cos(XR);
	float SY = sin(YR);
	float CY = cos(YR);
	float SZ = sin(ZR);
	float CZ = cos(ZR);

	// Below rotation matrices are calculated by multiplying following 
	// single BVH (right-handed) rotation matrices:
	//
	// Rotation around X:   Rotation around Y:   Rotation around Z:
	// ------------------   ------------------   ------------------
	// 1  0       0         cos(y)  0  sin(y)    cos(z)  -sin(z)  0
	// 0  cos(x)  -sin(x)   0       1  0         sin(z)  cos(z)   0
	// 0  sin(x)  cos(x)    -sin(y) 0  cos(y)    0       0        1


	FMatrix RotMatrix;
	switch (RotOrder)
	{
		case XYZ:
		{
			RotMatrix.M[0][0] = CY*CZ;
			RotMatrix.M[0][1] = -CY*SZ;
			RotMatrix.M[0][2] = SY;
			RotMatrix.M[1][0] = CZ*SX*SY + CX*SZ;
			RotMatrix.M[1][1] = CX*CZ - SX*SY*SZ;
			RotMatrix.M[1][2] = -CY*SX;
			RotMatrix.M[2][0] = SX*SZ - CX*CZ*SY;
			RotMatrix.M[2][1] = CZ*SX + CX*SY*SZ;
			RotMatrix.M[2][2] = CX*CY;
			break;
		}
		case XZY:
		{
			RotMatrix.M[0][0] = CY*CZ;
			RotMatrix.M[0][1] = -SZ;
			RotMatrix.M[0][2] = CZ*SY;
			RotMatrix.M[1][0] = SX*SY + CX*CY*SZ;
			RotMatrix.M[1][1] = CX*CZ;
			RotMatrix.M[1][2] = CX*SY*SZ - CY*SX;
			RotMatrix.M[2][0] = CY*SX*SZ - CX*SY;
			RotMatrix.M[2][1] = CZ*SX;
			RotMatrix.M[2][2] = CX*CY + SX*SY*SZ;
			break;
		}
		case YXZ:
		{
			RotMatrix.M[0][0] = CY*CZ + SX*SY*SZ;
			RotMatrix.M[0][1] = CZ*SX*SY - CY*SZ;
			RotMatrix.M[0][2] = CX*SY;
			RotMatrix.M[1][0] = CX*SZ;
			RotMatrix.M[1][1] = CX*CZ;
			RotMatrix.M[1][2] = -SX;
			RotMatrix.M[2][0] = CY*SX*SZ - CZ*SY;
			RotMatrix.M[2][1] = CY*CZ*SX + SY*SZ;
			RotMatrix.M[2][2] = CX*CY;
			break;
		}
		case YZX:
		{
			RotMatrix.M[0][0] = CY*CZ;
			RotMatrix.M[0][1] = SX*SY - CX*CY*SZ;
			RotMatrix.M[0][2] = CX*SY + CY*SX*SZ;
			RotMatrix.M[1][0] = SZ;
			RotMatrix.M[1][1] = CX*CZ;
			RotMatrix.M[1][2] = -CZ*SX;
			RotMatrix.M[2][0] = -CZ*SY;
			RotMatrix.M[2][1] = CY*SX + CX*SY*SZ;
			RotMatrix.M[2][2] = CX*CY - SX*SY*SZ;
			break;
		}
		case ZXY:
		{
			RotMatrix.M[0][0] = CY*CZ - SX*SY*SZ;
			RotMatrix.M[0][1] = -CX*SZ;
			RotMatrix.M[0][2] = CZ*SY + CY*SX*SZ;
			RotMatrix.M[1][0] = CZ*SX*SY + CY*SZ;
			RotMatrix.M[1][1] = CX*CZ;
			RotMatrix.M[1][2] = SY*SZ - CY*CZ*SX;
			RotMatrix.M[2][0] = -CX*SY;
			RotMatrix.M[2][1] = SX;
			RotMatrix.M[2][2] = CX*CY;
			break;
		}
		default:
		case ZYX:
		{
			RotMatrix.M[0][0] = CY*CZ;
			RotMatrix.M[0][1] = CZ*SX*SY - CX*SZ;
			RotMatrix.M[0][2] = CX*CZ*SY + SX*SZ;
			RotMatrix.M[1][0] = CY*SZ;
			RotMatrix.M[1][1] = CX*CZ + SX*SY*SZ;
			RotMatrix.M[1][2] = CX*SY*SZ - CZ*SX;
			RotMatrix.M[2][0] = -SY;
			RotMatrix.M[2][1] = CY*SX;
			RotMatrix.M[2][2] = CX*CY;
			break;
		}
	}

	RotMatrix.M[0][3] = 0;
	RotMatrix.M[1][3] = 0;
	RotMatrix.M[2][3] = 0;

	RotMatrix.M[3][0] = 0;
	RotMatrix.M[3][1] = 0;
	RotMatrix.M[3][2] = 0;
	RotMatrix.M[3][3] = 1;

	// Rotation Matrix => Quaternion
	return(RotMatrix.ToQuat());
}
