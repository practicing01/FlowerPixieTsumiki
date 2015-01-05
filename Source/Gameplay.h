/*
 * Gameplay.h
 *
 *  Created on: Dec 24, 2014
 *      Author: practicing01
 */

#pragma once

#include "Object.h"
#include "Urho3DPlayer.h"

using namespace Urho3D;

class Gameplay : public Object
{
	OBJECT(Gameplay);
public:
	Gameplay(Context* context, Urho3DPlayer* main);
	~Gameplay();

	void HandleUpdate(StringHash eventType, VariantMap& eventData);
	void TouchDown(StringHash eventType, VariantMap& eventData);
	void TouchDrag(StringHash eventType, VariantMap& eventData);
	void HandleNodeCollisionStart(StringHash eventType, VariantMap& eventData);
	void HandleNodeCollisionEnd(StringHash eventType, VariantMap& eventData);
	void LoadScene();
	void UpdateScore();
	void ChickenMoveToComplete(StringHash eventType, VariantMap& eventData);

	Urho3DPlayer* main_;
	float elapsedTime_;
	float timeStep_;
	SharedPtr<Node> tsumikiNode_;
	SharedPtr<Node> chickenNode_;
	SharedPtr<Node> waterBallNode_;
	SharedPtr<Node> seedBallNode_;
	SharedPtr<Node> flowerNode_;
	SharedPtr<Node> seedNode_;
	SharedPtr<Node> explosionNode_;
	Vector3 vectoria_;
	SharedPtr<Text> text_;
	int score_;
	int topScore_;
	float chickenSpeed_;
	float chickenRotateSpeed_;
	float bumpForce_;
	float tsumikiSpeed_;
	float tsumikiRotateSpeed_;

};
