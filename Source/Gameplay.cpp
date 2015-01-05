/*
 * Gameplay.cpp
 *
 *  Created on: Dec 24, 2014
 *      Author: practicing01
 */

#include "AnimationController.h"
#include "AnimatedModel.h"
#include "Camera.h"
#include "CoreEvents.h"
#include "Drawable.h"
#include "File.h"
#include "FileSystem.h"
#include "Font.h"
#include "Graphics.h"
#include "InputEvents.h"
#include "Octree.h"
#include "OctreeQuery.h"
#include "ParticleEmitter.h"
#include "ParticleEffect.h"
#include "PhysicsEvents.h"
#include "ResourceCache.h"
#include "RigidBody.h"
#include "Scene.h"
#include "Skeleton.h"
#include "Text.h"
#include "UI.h"

#include "Gameplay.h"
#include "SeedSpawner.h"
#include "RigidBodyMoveTo.h"
#include "SceneObjectMoveTo.h"
#include "SceneObjectRotateTo.h"
#include "RigidBodyRotateTo.h"

//#include "Log.h"

Gameplay::Gameplay(Context* context, Urho3DPlayer* main) :
Object(context)
{
	context->RegisterFactory<SeedSpawner>();

	main_ = main;
	elapsedTime_ = 0.0f;
	score_ = 0;
	topScore_ = 0;
	chickenSpeed_ = 10.0f;
	chickenRotateSpeed_ = 2.5f;
	bumpForce_ = 150.0f;
	tsumikiSpeed_ = 40.0f;
	tsumikiRotateSpeed_ = 5.0f;

	LoadScene();
	SubscribeToEvent(E_UPDATE, HANDLER(Gameplay, HandleUpdate));
}

Gameplay::~Gameplay()
{
}

void Gameplay::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	using namespace Update;

	timeStep_ = eventData[P_TIMESTEP].GetFloat();

	elapsedTime_ += timeStep_;

	vectoria_ = tsumikiNode_->GetPosition();
	vectoria_.y_ = 96.0f;
	vectoria_.z_ += -100.0f;
	main_->cameraNode_->SetPosition(vectoria_);
}

void Gameplay::LoadScene()
{
	main_->cameraNode_->RemoveAllChildren();
	main_->cameraNode_->RemoveAllComponents();
	main_->cameraNode_->Remove();

	File loadFile(context_,
			GetSubsystem<FileSystem>()->GetProgramDir()
			+ "Data/Scenes/flowerScene.xml", FILE_READ);
	main_->scene_->LoadXML(loadFile);

	main_->cameraNode_ = main_->scene_->GetChild("camera");
	main_->viewport_->SetCamera(main_->cameraNode_->GetComponent<Camera>());

	tsumikiNode_ = main_->scene_->GetChild("tsumiki");
	RigidBodyMoveTo* banana = tsumikiNode_->CreateComponent<RigidBodyMoveTo>();
	tsumikiNode_->CreateComponent<RigidBodyRotateTo>();

	tsumikiNode_->GetComponent<AnimatedModel>()->GetSkeleton().GetBone("Ribs")->node_->GetChild(
			"wings")->GetComponent<AnimationController>()->PlayExclusive(
					"Models/flapAction.ani", 0, true, 0.0f);
	tsumikiNode_->GetComponent<AnimatedModel>()->GetSkeleton().GetBone("Ribs")->node_->GetChild(
			"wings")->GetComponent<AnimationController>()->SetSpeed(
					"Models/flapAction.ani", 4.0f);
	tsumikiNode_->GetComponent<AnimationController>()->PlayExclusive(
				"Models/flyAni.ani", 0, true, 0.0f);

	chickenNode_ = main_->scene_->GetChild("chicken");
	chickenNode_->GetComponent<AnimationController>()->PlayExclusive(
			"Models/Walk.ani", 0, true, 0.0f);
	chickenNode_->GetComponent<AnimationController>()->SetSpeed(
			"Models/Walk.ani", 0.25f);
	chickenNode_->CreateComponent<SceneObjectMoveTo>();
	chickenNode_->CreateComponent<SceneObjectRotateTo>();
	SubscribeToEvent(E_SCENEOBJECTMOVETOCOMPLETE,
			HANDLER(Gameplay, ChickenMoveToComplete));
	SubscribeToEvent(chickenNode_, E_NODECOLLISIONSTART,
			HANDLER(Gameplay, HandleNodeCollisionStart));
	vectoria_ = tsumikiNode_->GetPosition();
	vectoria_.y_ = chickenNode_->GetPosition().y_;
	chickenNode_->GetComponent<SceneObjectMoveTo>()->MoveTo(vectoria_,
			chickenSpeed_, true);
	chickenNode_->LookAt(vectoria_);
	chickenNode_->Rotate(Quaternion(0.0f, -90.0f, 0.0f));

	waterBallNode_ = main_->scene_->GetChild("ballWater");
	SubscribeToEvent(waterBallNode_, E_NODECOLLISIONSTART,
			HANDLER(Gameplay, HandleNodeCollisionStart));
	SubscribeToEvent(waterBallNode_, E_NODECOLLISIONEND,
			HANDLER(Gameplay, HandleNodeCollisionEnd));

	seedBallNode_ = main_->scene_->GetChild("ballSeed");
	SubscribeToEvent(seedBallNode_, E_NODECOLLISIONSTART,
			HANDLER(Gameplay, HandleNodeCollisionStart));
	SubscribeToEvent(seedBallNode_, E_NODECOLLISIONEND,
			HANDLER(Gameplay, HandleNodeCollisionEnd));
	seedBallNode_->CreateComponent<SeedSpawner>();

	flowerNode_ = main_->scene_->GetChild("flower");

	seedNode_ = main_->scene_->GetChild("seed");
	seedBallNode_->GetComponent<SeedSpawner>()->seedNode_ = seedNode_;

	Node* particleNode_;
	particleNode_ = main_->scene_->CreateChild("ParticleEmitter2D");
	ParticleEffect* particleEffect = main_->cache_->GetResource<ParticleEffect>(
			"Particle/spray.xml");
	ParticleEmitter* particleEmitter = particleNode_->CreateComponent<
			ParticleEmitter>();
	particleEmitter->SetEffect(particleEffect);

	waterBallNode_->AddChild(particleNode_);
	particleNode_->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	explosionNode_ = main_->scene_->CreateChild("explosion");
	ParticleEffect* explosionParticleEffect = main_->cache_->GetResource<
			ParticleEffect>("Particle/flowerExplosion.xml");
	ParticleEmitter* explosionParticleEmitter_ =
			explosionNode_->CreateComponent<ParticleEmitter>();
	explosionParticleEmitter_->SetEffect(explosionParticleEffect);
	explosionParticleEmitter_->SetEmitting(false);

	text_ = new Text(context_);
	text_->SetText(
			"Flowers Grown: " + String(score_) + " Most Grown: "
			+ String(topScore_));
	text_->SetFont(main_->cache_->GetResource<Font>("Fonts/Anonymous Pro.ttf"),
			12);
	text_->SetColor(Color(1.0f, 0.5f, 0.0f));
	text_->SetPosition(0, main_->graphics_->GetHeight() - 15);
	text_->SetHorizontalAlignment(HA_LEFT);
	text_->SetVerticalAlignment(VA_TOP);
	GetSubsystem<UI>()->GetRoot()->AddChild(text_);

	SubscribeToEvent(E_TOUCHBEGIN, HANDLER(Gameplay, TouchDown));
	SubscribeToEvent(E_TOUCHMOVE, HANDLER(Gameplay, TouchDrag));
}

void Gameplay::TouchDown(StringHash eventType, VariantMap& eventData)
{
	using namespace TouchBegin;

	Ray cameraRay = main_->cameraNode_->GetComponent<Camera>()->GetScreenRay(
			(float) eventData[P_X].GetInt() / main_->graphics_->GetWidth(),
			(float) eventData[P_Y].GetInt() / main_->graphics_->GetHeight());

	PODVector<RayQueryResult> results;

	RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, 1000.0f,
			DRAWABLE_GEOMETRY);

	main_->scene_->GetComponent<Octree>()->Raycast(query);

	if (results.Size())
	{
		for (int x = 0; x < results.Size(); x++)
		{
			if (results[x].node_->GetName() == "floor")
			{
				vectoria_ = results[x].position_;
			}
		}
	}
	else
	{
		return;
	}

	vectoria_.y_ = 4.0f;
	tsumikiNode_->GetComponent<RigidBodyMoveTo>()->MoveTo(vectoria_,
			tsumikiSpeed_, true);

	//Quaternion quaterOnion = tsumikiNode_->GetRotation();
	tsumikiNode_->LookAt(vectoria_);
	tsumikiNode_->Rotate(Quaternion(0.0f, 90.0f, 0.0f));
	//Quaternion quaterPounder = tsumikiNode_->GetRotation();
	//tsumikiNode_->SetRotation(quaterOnion);

	//tsumikiNode_->GetComponent<RigidBodyRotateTo>()->RotateTo(quaterPounder, tsumikiRotateSpeed_, true);
}

void Gameplay::TouchDrag(StringHash eventType, VariantMap& eventData)
{
	using namespace TouchMove;

	Ray cameraRay = main_->cameraNode_->GetComponent<Camera>()->GetScreenRay(
			(float) eventData[P_X].GetInt() / main_->graphics_->GetWidth(),
			(float) eventData[P_Y].GetInt() / main_->graphics_->GetHeight());

	PODVector<RayQueryResult> results;

	RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, 1000.0f,
			DRAWABLE_GEOMETRY);

	main_->scene_->GetComponent<Octree>()->Raycast(query);

	if (results.Size())
	{
		for (int x = 0; x < results.Size(); x++)
		{
			if (results[x].node_->GetName() == "floor")
			{
				vectoria_ = results[x].position_;
			}
		}
	}
	else
	{
		return;
	}

	vectoria_.y_ = 4.0f;
	tsumikiNode_->GetComponent<RigidBodyMoveTo>()->MoveTo(vectoria_,
			tsumikiSpeed_, true);


	//Quaternion quaterOnion = tsumikiNode_->GetRotation();
	tsumikiNode_->LookAt(vectoria_);
	tsumikiNode_->Rotate(Quaternion(0.0f, 90.0f, 0.0f));
	//Quaternion quaterPounder = tsumikiNode_->GetRotation();
	//tsumikiNode_->SetRotation(quaterOnion);

	//tsumikiNode_->GetComponent<RigidBodyRotateTo>()->RotateTo(quaterPounder, tsumikiRotateSpeed_, true);
}

void Gameplay::HandleNodeCollisionStart(StringHash eventType,
		VariantMap& eventData)
{
	using namespace NodeCollision;

	Node* noed = static_cast<RigidBody*>(eventData[P_BODY].GetPtr())->GetNode();
	Node* OtherNode = static_cast<Node*>(eventData[P_OTHERNODE].GetPtr());

	if (OtherNode->GetName() == "flower")
	{
		if (noed == waterBallNode_)
		{
			return;
		}

		//explosive effect here
		explosionNode_->SetPosition(OtherNode->GetPosition());
		explosionNode_->GetComponent<ParticleEmitter>()->SetEmitting(true);

		OtherNode->RemoveAllComponents();
		OtherNode->RemoveAllChildren();
		OtherNode->Remove();

		//subtract score here
		score_--;
		UpdateScore();
		return;
	}

	if (noed == seedBallNode_)
	{
		if (OtherNode->GetName() == "seed")
		{
			if (!noed->GetComponent<SeedSpawner>()->collidingSeeds_.Contains(
					SharedPtr<Node>(OtherNode)))
			{
				noed->GetComponent<SeedSpawner>()->collidingSeeds_.Push(
						SharedPtr<Node>(OtherNode));
			}
		}
		else if (OtherNode->GetName() == "floor")
		{
			noed->GetComponent<SeedSpawner>()->floorCollision_ = true;
		}
		else if (OtherNode->GetName() == "leftWall")
		{
			noed->GetComponent<RigidBody>()->ApplyImpulse(
					Vector3::RIGHT * bumpForce_);
		}
		else if (OtherNode->GetName() == "rightWall")
		{
			noed->GetComponent<RigidBody>()->ApplyImpulse(
					Vector3::LEFT * bumpForce_);
		}
		else if (OtherNode->GetName() == "topWall")
		{
			noed->GetComponent<RigidBody>()->ApplyImpulse(
					Vector3::BACK * bumpForce_);
		}
		else if (OtherNode->GetName() == "bottomWall")
		{
			noed->GetComponent<RigidBody>()->ApplyImpulse(
					Vector3::FORWARD * bumpForce_);
		}
	}
	else if (noed == chickenNode_)
	{
		if (OtherNode->GetName() == "seed")
		{
			seedBallNode_->GetComponent<SeedSpawner>()->collidingSeeds_.Remove(
					SharedPtr<Node>(OtherNode));
			OtherNode->RemoveAllComponents();
			OtherNode->RemoveAllChildren();
			OtherNode->Remove();
		}
	}
	else if (noed == waterBallNode_)
	{
		if (OtherNode->GetName() == "leftWall")
		{
			noed->GetComponent<RigidBody>()->ApplyImpulse(
					Vector3::RIGHT * bumpForce_);
		}
		else if (OtherNode->GetName() == "rightWall")
		{
			noed->GetComponent<RigidBody>()->ApplyImpulse(
					Vector3::LEFT * bumpForce_);
		}
		else if (OtherNode->GetName() == "topWall")
		{
			noed->GetComponent<RigidBody>()->ApplyImpulse(
					Vector3::BACK * bumpForce_);
		}
		else if (OtherNode->GetName() == "bottomWall")
		{
			noed->GetComponent<RigidBody>()->ApplyImpulse(
					Vector3::FORWARD * bumpForce_);
		}
	}
}

void Gameplay::HandleNodeCollisionEnd(StringHash eventType,
		VariantMap& eventData)
{
	using namespace NodeCollision;

	Node* noed = static_cast<RigidBody*>(eventData[P_BODY].GetPtr())->GetNode();
	Node* OtherNode = static_cast<Node*>(eventData[P_OTHERNODE].GetPtr());

	if (noed == seedBallNode_)
	{
		if (OtherNode->GetName() == "seed")
		{
			if (noed->GetComponent<SeedSpawner>()->collidingSeeds_.Contains(
					SharedPtr<Node>(OtherNode)))
			{
				noed->GetComponent<SeedSpawner>()->collidingSeeds_.Remove(
						SharedPtr<Node>(OtherNode));
			}
		}
		else if (OtherNode->GetName() == "floor")
		{
			noed->GetComponent<SeedSpawner>()->floorCollision_ = false;
		}
	}
	else if (noed == waterBallNode_)
	{
		if (OtherNode->GetName() == "seed")
		{
			//explosive effect
			explosionNode_->SetPosition(OtherNode->GetPosition());
			explosionNode_->GetComponent<ParticleEmitter>()->SetEmitting(true);
			//add score here
			score_++;
			UpdateScore();
			//turn to flower
			Node* flowerClone = flowerNode_->Clone(LOCAL);
			vectoria_ = OtherNode->GetPosition();
			vectoria_.y_ = 0.0f;
			flowerClone->SetPosition(vectoria_);
			//delete seed
			seedBallNode_->GetComponent<SeedSpawner>()->collidingSeeds_.Remove(
					SharedPtr<Node>(OtherNode));
			OtherNode->RemoveAllComponents();
			OtherNode->RemoveAllChildren();
			OtherNode->Remove();
		}
	}
}

void Gameplay::UpdateScore()
{
	if (score_ > topScore_)
	{
		topScore_ = score_;
	}
	text_->SetText(
			"Flowers Grown: " + String(score_) + " Most Grown: "
			+ String(topScore_));
}

void Gameplay::ChickenMoveToComplete(StringHash eventType,
		VariantMap& eventData)
{
	vectoria_ = tsumikiNode_->GetPosition();
	vectoria_.y_ = chickenNode_->GetPosition().y_;

	Quaternion quaterOnion = chickenNode_->GetRotation();
	chickenNode_->LookAt(vectoria_);
	chickenNode_->Rotate(Quaternion(0.0f, -90.0f, 0.0f));
	Quaternion quaterPounder = chickenNode_->GetRotation();
	chickenNode_->SetRotation(quaterOnion);

	chickenNode_->GetComponent<SceneObjectRotateTo>()->RotateTo(quaterPounder,
			chickenRotateSpeed_, true);
	chickenNode_->GetComponent<SceneObjectMoveTo>()->MoveTo(vectoria_,
			chickenSpeed_, true);

}
