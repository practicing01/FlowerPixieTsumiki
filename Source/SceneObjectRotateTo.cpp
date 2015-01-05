/*
 * SceneObjectRotateTo.cpp
 *
 *  Created on: Jan 3, 2015
 *      Author: practicing01
 */

#include "math.h"
#include "ProcessUtils.h"
#include "Scene.h"
#include "SceneEvents.h"
#include "SceneObjectRotateTo.h"

#include "DebugNew.h"

SceneObjectRotateTo::SceneObjectRotateTo(Context* context) :
LogicComponent(context)
{
	isRotating_ = false;
	// Only the scene update event is needed: unsubscribe from the rest for optimization
	SetUpdateEventMask(USE_UPDATE);
}

void SceneObjectRotateTo::OnRotateToComplete()
{
	SendEvent(E_SCENEOBJECTROTATETOCOMPLETE);
}

void SceneObjectRotateTo::RotateTo(Quaternion dest, float speed, bool stopOnCompletion)
{
	rotateToSpeed_ = speed;
	rotateToDest_ = dest;
	rotateToLoc_ = node_->GetRotation();
	rotateToTravelTime_ = sqrt((rotateToDest_ - rotateToLoc_).LengthSquared()) / speed;
	rotateToElapsedTime_ = 0.0f;
	rotateToStopOnTime_ = stopOnCompletion;
	isRotating_ = true;
}

void SceneObjectRotateTo::Update(float timeStep)
{
	if (isRotating_ == true)
	{
		inderp_ = timeStep * rotateToSpeed_;
		remainingDist_ = sqrt((node_->GetRotation() - rotateToDest_).LengthSquared());
		node_->SetRotation(node_->GetRotation().Slerp(rotateToDest_, inderp_ / remainingDist_));
		rotateToElapsedTime_ += timeStep;
		if (rotateToElapsedTime_ >= rotateToTravelTime_)
		{
			isRotating_ = false;
			if (rotateToStopOnTime_ == true)
			{
			}
			OnRotateToComplete();
		}
	}
}
