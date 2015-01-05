/*
 * RigidBodyRotateTo.h
 *
 *  Created on: Jan 4, 2015
 *      Author: practicing01
 */

#pragma once

#include "Object.h"
#include "LogicComponent.h"
#include "Quaternion.h"

// All Urho3D classes reside in namespace Urho3D
using namespace Urho3D;

static const StringHash E_RIGIDBODYROTATETOCOMPLETE("RigidBodyRotateToComplete");

class RigidBodyRotateTo: public LogicComponent
{
	OBJECT(RigidBodyRotateTo);
public:
	RigidBodyRotateTo(Context* context);
	/// Handle scene update. Called by LogicComponent base class.
	virtual void FixedUpdate(float timeStep);
	void OnRotateToComplete();
	void RotateTo(Quaternion dest, float speed, bool stopOnCompletion);

	Quaternion rotateToDest_;
	Quaternion rotateToLoc_;
	float rotateToSpeed_;
	float rotateToTravelTime_;
	float rotateToElapsedTime_;
	bool rotateToStopOnTime_;
	bool isRotating_;
};
