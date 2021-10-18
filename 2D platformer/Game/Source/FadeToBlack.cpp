#include "FadeToBlack.h"

#include "App.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL_render.h"

FadeToBlack::FadeToBlack() : Module()
{
	name.Create("fadeToBlack");
	active = true;
}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Awake()
{
	return true;
}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool FadeToBlack::PreUpdate()
{
	return true;
}

bool FadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			moduleToDisable->active = false;
			moduleToEnable->active=true;

			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else if (currentStep == Fade_Step::FROM_BLACK)
	{
		--frameCount;
		if (frameCount == 0)
		{

			currentStep = Fade_Step::NONE;
		}
	}
	else
	{
	
	}

	return true;
}

bool FadeToBlack::PostUpdate()
{

	// Exit this function if we are not performing a fade
	if (currentStep != Fade_Step::NONE) {
		float fadeRatio = (float)frameCount / (float)maxFadeFrames;
		screenRect.w = app->render->camera.w;
		screenRect.h = app->render->camera.h;
		SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
		SDL_RenderFillRect(app->render->renderer, &screenRect);
	}
	

	return true;
}

bool FadeToBlack::Fade(Module* moduleToDisable, Module* moduleToEnable, float frames)
{

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;
	}

	return true;
}


bool FadeToBlack::CleanUp()
{
	LOG("Freeing scene");

	return true;
}