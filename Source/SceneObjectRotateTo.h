/*
 * SceneObjectRotateTo.h
 *
 *  Created on: Jan 3, 2015
 *      Author: practicing01
 */

#pragma once

#include "Object.h"
#include "LogicComponent.h"
#include "Quaternion.h"

// All Urho3D classes reside in namespace Urho3D
using namespace Urho3D;

static const StringHash E_SCENEOBJECTROTATETOCOMPLETE("SceneObjectRotateToComplete");

class SceneObjectRotateTo: public LogicComponent
{
	OBJECT(SceneObjectRotateTo);
public:
	SceneObjectRotateTo(Context* context);
	/// Handle scene update. Called by LogicComponent base class.
	virtual void Update(float timeStep);
	void OnRotateToComplete();
	void RotateTo(Quaternion dest, float speed, bool stopOnCompletion);

	Quaternion rotateToDest_;
	Quaternion rotateToLoc_;
	float rotateToSpeed_;
	float rotateToTravelTime_;
	float rotateToElapsedTime_;
	float inderp_;
	float remainingDist_;
	bool rotateToStopOnTime_;
	bool isRotating_;
};
