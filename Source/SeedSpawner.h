/*
 * SeedSpawner.h
 *
 *  Created on: Dec 29, 2014
 *      Author: practicing01
 */

#pragma once

#include "Object.h"
#include "LogicComponent.h"
#include "Node.h"

// All Urho3D classes reside in namespace Urho3D
using namespace Urho3D;

class SeedSpawner: public LogicComponent
{
	OBJECT(SeedSpawner);
public:
	SeedSpawner(Context* context);
	virtual void Update(float timeStep);

	bool floorCollision_;
	Vector<SharedPtr<Node> > collidingSeeds_;
	SharedPtr<Node> seedNode_;
	Vector3 vectoria_;
};
