/*
 * RigidBodyMoveTo.cpp
 *
 *  Created on: Dec 25, 2014
 *      Author: practicing01
 */

#include "ProcessUtils.h"
#include "RigidBody.h"
#include "RigidBodyMoveTo.h"
#include "Scene.h"
#include "SceneEvents.h"

#include "DebugNew.h"

RigidBodyMoveTo::RigidBodyMoveTo(Context* context) :
		LogicComponent(context)
{
	isMoving_ = false;
	// Only the physics update event is needed: unsubscribe from the rest for optimization
	SetUpdateEventMask(USE_FIXEDUPDATE);
}

void RigidBodyMoveTo::OnMoveToComplete()
{
	SendEvent(E_RIGIDBODYMOVETOCOMPLETE);
}

void RigidBodyMoveTo::MoveTo(Vector3 dest, float speed, bool stopOnCompletion)
{
	moveToSpeed_ = speed;
	moveToDest_ = dest;
	moveToLoc_ = node_->GetWorldPosition();
	moveToDir_ = dest - moveToLoc_;
	moveToDir_.Normalize();
	moveToTravelTime_ = (moveToDest_ - moveToLoc_).Length() / speed;
	moveToElapsedTime_ = 0.0f;
	moveToStopOnTime_ = stopOnCompletion;
	isMoving_ = true;
	node_->GetComponent<RigidBody>()->SetLinearVelocity(moveToDir_ * speed);
}

void RigidBodyMoveTo::FixedUpdate(float timeStep)
{
	if (isMoving_ == true)
	{
		moveToElapsedTime_ += timeStep;
		if (moveToElapsedTime_ >= moveToTravelTime_)
		{
			isMoving_ = false;
			if (moveToStopOnTime_ == true)
			{
				node_->GetComponent<RigidBody>()->SetLinearVelocity(Vector3::ZERO);
			}
			OnMoveToComplete();
		}
	}
}
