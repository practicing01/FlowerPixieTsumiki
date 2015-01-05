/*
 * Splash.cpp
 *
 *  Created on: Dec 24, 2014
 *      Author: practicing01
 */

#include "CoreEvents.h"
#include "Camera.h"
#include "Engine.h"
#include "Font.h"
#include "Graphics.h"
//#include "Log.h"
#include "Node.h"
#include "Octree.h"
#include "Renderer.h"
#include "ResourceCache.h"
#include "Scene.h"
#include "Sprite.h"
#include "Text.h"
#include "Texture2D.h"
#include "UI.h"
#include "Viewport.h"

#include "Splash.h"
#include "Gameplay.h"

#include "DebugNew.h"

Splash::Splash(Context* context, Urho3DPlayer* main) :
    Object(context)
{
	main_ = main;
	elapsedTime_ = 0.0f;

	SplashScreen();
	SubscribeToEvent(E_UPDATE, HANDLER(Splash, HandleUpdate));
}

Splash::~Splash()
{
}

void Splash::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	using namespace Update;

	float timeStep = eventData[P_TIMESTEP].GetFloat();

	//LOGERRORF("loop");
	elapsedTime_ += timeStep;

	if (elapsedTime_ >= 2.0f)
	{
		main_->scene_->RemoveAllChildren();
		main_->scene_->RemoveAllComponents();
		main_->GetSubsystem<UI>()->GetRoot()->RemoveAllChildren();
		main_->logicStates_.Remove(this);
		main_->logicStates_.Push(new Gameplay(context_, main_));
		delete this;
	}
}

void Splash::SplashScreen()
{
	main_->scene_ = new Scene(main_->GetContext());
	main_->scene_->CreateComponent<Octree>();

	main_->cameraNode_ = new Node(main_->GetContext());
	Camera* camera = main_->cameraNode_->CreateComponent<Camera>();
	camera->SetOrthographic(true);
	camera->SetAspectRatio(1.67f);
	camera->SetFov(45.0f);
	//camera->SetOrthoSize(Vector2(12.5f, 7.5f));
	camera->SetFarClip(300.0f);

	// Set an initial position for the camera scene node above the floor
	main_->cameraNode_->SetPosition(Vector3(0.0f, 10.0f, 0.0f));
	main_->cameraNode_->LookAt(Vector3(0.0f, 10.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));

	Renderer* renderer = main_->GetSubsystem<Renderer>();
	Graphics* graphics = main_->GetSubsystem<Graphics>();
	UI* ui = main_->GetSubsystem<UI>();

	// Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
	main_->viewport_ = new Viewport(main_->GetContext(), main_->scene_, main_->cameraNode_->GetComponent<Camera>());
	renderer->SetViewport(0, main_->viewport_);

	ResourceCache* cache = main_->GetSubsystem<ResourceCache>();

	Texture2D* decalTex = cache->GetResource<Texture2D>("Textures/fich.png");
	SharedPtr<Sprite> sprite(new Sprite(main_->GetContext()));
	sprite->SetTexture(decalTex);
	sprite->SetPosition(Vector2(((float)graphics->GetWidth())*0.5f, ((float)graphics->GetHeight())*0.5f));
	sprite->SetSize(IntVector2(128, 128));
	sprite->SetHotSpot(IntVector2(64, 64));
	sprite->SetColor(Color(Random(0.5f) + 0.5f, Random(0.5f) + 0.5f, Random(0.5f) + 0.5f));
	ui->GetRoot()->AddChild(sprite);

	Texture2D* mdsdecalTex = cache->GetResource<Texture2D>("Textures/mds.png");
	SharedPtr<Sprite> mdssprite(new Sprite(main_->GetContext()));
	mdssprite->SetTexture(mdsdecalTex);
	mdssprite->SetPosition(Vector2(128.0f, ((float)graphics->GetHeight())*0.5f));
	mdssprite->SetSize(IntVector2(256, 256));
	mdssprite->SetHotSpot(IntVector2(128, 128));
	ui->GetRoot()->AddChild(mdssprite);

	Texture2D* ogadecalTex = cache->GetResource<Texture2D>("Textures/oga.png");
	SharedPtr<Sprite> ogasprite(new Sprite(main_->GetContext()));
	ogasprite->SetTexture(ogadecalTex);
	ogasprite->SetPosition(Vector2(((float)graphics->GetWidth())-128.0f, ((float)graphics->GetHeight())*0.5f));
	ogasprite->SetSize(IntVector2(256, 256));
	ogasprite->SetHotSpot(IntVector2(128, 128));
	ui->GetRoot()->AddChild(ogasprite);

	Text* text_ = new Text(main_->GetContext());
	text_->SetText("Urho3D");
	// Set font and text color
	text_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
	text_->SetColor(Color(0.0f, 1.0f, 0.5f));
	text_->SetPosition(0,(graphics->GetHeight()/2)+64);
	// Align Text center-screen
	text_->SetHorizontalAlignment(HA_CENTER);
	text_->SetVerticalAlignment(VA_TOP);
	// Add Text instance to the UI root element
	main_->GetSubsystem<UI>()->GetRoot()->AddChild(text_);


	Text* mdstext_ = new Text(main_->GetContext());
	mdstext_->SetText("MourningDoveSoft");
	// Set font and text color
	mdstext_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
	mdstext_->SetColor(Color(0.0f, 0.5f, 0.5f));
	mdstext_->SetPosition(0,(graphics->GetHeight()/2)+128);
	// Align Text center-screen
	mdstext_->SetHorizontalAlignment(HA_LEFT);
	mdstext_->SetVerticalAlignment(VA_TOP);
	// Add Text instance to the UI root element
	main_->GetSubsystem<UI>()->GetRoot()->AddChild(mdstext_);
}
