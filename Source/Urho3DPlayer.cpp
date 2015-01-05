//
// Copyright (c) 2008-2014 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "CoreEvents.h"
#include "Engine.h"
#include "Font.h"
#include "Graphics.h"
#include "Input.h"
#include "Log.h"
#include "Main.h"
#include "Node.h"
#include "ProcessUtils.h"
#include "Scene.h"
#include "Text.h"
#include "UI.h"
#include "ResourceCache.h"
#include "ResourceEvents.h"

#include "Urho3DPlayer.h"
#include "Splash.h"
#include "RigidBodyMoveTo.h"
#include "SceneObjectMoveTo.h"
#include "SceneObjectRotateTo.h"
#include "RigidBodyRotateTo.h"

#include "DebugNew.h"

DEFINE_APPLICATION_MAIN(Urho3DPlayer);

Urho3DPlayer::Urho3DPlayer(Context* context) :
    Application(context)
{
	context->RegisterFactory<RigidBodyMoveTo>();
	context->RegisterFactory<SceneObjectMoveTo>();
	context->RegisterFactory<SceneObjectRotateTo>();
	context->RegisterFactory<RigidBodyRotateTo>();
}

void Urho3DPlayer::Setup()
{
	engineParameters_["WindowWidth"] = 800;
	engineParameters_["WindowHeight"] = 480;
	engineParameters_["FullScreen"] = false;
	engineParameters_["VSync"] = true;
}

void Urho3DPlayer::Start()
{
	SetRandomSeed(GetSubsystem<Time>()->GetSystemTime());
	input_ = GetSubsystem<Input>();
	input_->SetMouseVisible(true);
	input_->SetTouchEmulation(true);
	graphics_ = GetSubsystem<Graphics>();
	logicStates_.Push(new Splash(context_, this));
	cache_ = GetSubsystem<ResourceCache>();
	SubscribeToEvents();
}

void Urho3DPlayer::Stop()
{
}

void Urho3DPlayer::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    //SubscribeToEvent(E_UPDATE, HANDLER(Urho3DPlayer, HandleUpdate));
}

void Urho3DPlayer::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	using namespace Update;

	timeStep_ = eventData[P_TIMESTEP].GetFloat();

}
