/*
 * RigidBodyRotateTo.cpp
 *
 *  Created on: Jan 4, 2015
 *      Author: practicing01
 */

#include "math.h"
#include "ProcessUtils.h"
#include "RigidBody.h"
#include "Scene.h"
#include "SceneEvents.h"
#include "DebugNew.h"

#include "RigidBodyRotateTo.h"


RigidBodyRotateTo::RigidBodyRotateTo(Context* context) :
LogicComponent(context)
{
	isRotating_ = false;
	// Only the physics update event is needed: unsubscribe from the rest for optimization
	SetUpdateEventMask(USE_FIXEDUPDATE);
}

void RigidBodyRotateTo::OnRotateToComplete()
{
	SendEvent(E_RIGIDBODYROTATETOCOMPLETE);
}

void RigidBodyRotateTo::RotateTo(Quaternion dest, float speed, bool stopOnCompletion)
{
	rotateToSpeed_ = speed;
	rotateToDest_ = dest;
	rotateToLoc_ = node_->GetRotation();
	rotateToTravelTime_ = sqrt((rotateToDest_ - rotateToLoc_).LengthSquared()) / speed;
	rotateToElapsedTime_ = 0.0f;
	rotateToStopOnTime_ = stopOnCompletion;
	isRotating_ = true;
	//The following spins wildly.
	//node_->GetComponent<RigidBody>()->SetAngularVelocity(dest.Normalized().EulerAngles() * speed);
	//The following rotates but towards incorrect destination.
	node_->GetComponent<RigidBody>()->SetAngularVelocity( rotateToLoc_.Slerp(dest, 1.0f).EulerAngles().Normalized() * speed );
}

void RigidBodyRotateTo::FixedUpdate(float timeStep)
{
	if (isRotating_ == true)
	{
		rotateToElapsedTime_ += timeStep;
		if (rotateToElapsedTime_ >= rotateToTravelTime_)
		{
			isRotating_ = false;
			if (rotateToStopOnTime_ == true)
			{
				node_->GetComponent<RigidBody>()->SetAngularVelocity(Vector3::ZERO);
			}
			OnRotateToComplete();
		}
	}
}
