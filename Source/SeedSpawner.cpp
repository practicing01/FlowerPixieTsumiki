/*
 * SeedSpawner.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: practicing01
 */

#include "SeedSpawner.h"
#include "Log.h"

SeedSpawner::SeedSpawner(Context* context) :
		LogicComponent(context)
{
	// Only the scene update event is needed: unsubscribe from the rest for optimization
	SetUpdateEventMask(USE_UPDATE);
	floorCollision_ = true;
}

void SeedSpawner::Update(float timeStep)
{
	if (collidingSeeds_.Size() == 0 && floorCollision_ == true)
	{
		//spawn seed
		Node* seedClone = seedNode_->Clone(LOCAL);
		vectoria_ = node_->GetPosition();
		vectoria_.y_ = 0.0f;
		seedClone->SetPosition(vectoria_);
	}
}
