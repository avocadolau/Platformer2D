#include "GuiManager.h"
#include "App.h"
#include "Textures.h"
#include "Fonts.h"
#include "SceneGame.h"
#include "SceneStart.h"

#include "GuiControl.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "Point.h"

#include "Audio.h"

GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{

	heart = app->tex->Load("Assets/textures/heart.png");
	hudImg = app->tex->Load("Assets/textures/hud background.png");
	mainMenuTitle = app->tex->Load("Assets/GUI/MainMenu.png");

	int offset = 160;
	iPoint p = {500,20};
	GuiControl* playButton = CreateGuiControl(GuiControlType::BUTTON, 1, "Assets/GUI/playButton.png", { 0,0 + offset,1280,120 }, app->sceneStart);
	playButton->SetDetector( p.x,p.y );
	mainMenu.add(playButton);

	GuiControl* continueButton = CreateGuiControl(GuiControlType::BUTTON, 2, "Assets/GUI/continueButton.png", { 0,100 + offset,1280,120 }, app->sceneStart);
	continueButton->SetDetector(p.x, p.y);
	mainMenu.add(continueButton);

	GuiControl* settingsButton = CreateGuiControl(GuiControlType::BUTTON, 3, "Assets/GUI/settingsButton.png", { 0,200 + offset,1280,120 }, this);
	settingsButton->SetDetector(p.x, p.y);
	mainMenu.add(settingsButton);

	GuiControl* creditsButton = CreateGuiControl(GuiControlType::BUTTON, 4, "Assets/GUI/creditsButton.png", { 0,300 + offset,1280,120 }, app->sceneStart);
	creditsButton->SetDetector(p.x, p.y);
	mainMenu.add(creditsButton);

	GuiControl* exitButton = CreateGuiControl(GuiControlType::BUTTON, 5, "Assets/GUI/exitButton.png", { 0,400 + offset,1280,120 }, app->sceneStart);
	exitButton->SetDetector(p.x, p.y);
	mainMenu.add(exitButton);


	GuiControl* resumeButton = CreateGuiControl(GuiControlType::BUTTON, 6, "Assets/GUI/resumeButton.png", { 0,0 + offset,1280,120 }, app->sceneGame);
	resumeButton->SetDetector(p.x, p.y);
	pauseMenu.add(resumeButton);

	GuiControl* settingsButton2 = CreateGuiControl(GuiControlType::BUTTON, 7, "Assets/GUI/settingsButton.png", { 0,100 + offset,1280,120 }, this);
	settingsButton2->SetDetector(p.x, p.y);
	pauseMenu.add(settingsButton2);

	GuiControl* mainMenuButton = CreateGuiControl(GuiControlType::BUTTON, 8, "Assets/GUI/mainMenuButton.png", { 0,200 + offset,1280,120 }, app->sceneGame);
	mainMenuButton->SetDetector(p.x, p.y);
	pauseMenu.add(mainMenuButton);

	GuiControl* exitButton2 = CreateGuiControl(GuiControlType::BUTTON, 9, "Assets/GUI/exitButton.png", { 0,300 + offset,1280,120 }, app->sceneGame);
	exitButton2->SetDetector(p.x, p.y);
	pauseMenu.add(exitButton2);


	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	// L14: TODO1: Create a GUI control and add it to the list of controls

	GuiControl* control = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		control = new GuiButton(id, bounds, text);
		break;
	
	// More Gui Controls can go here

	default:
		break;
	}

	//Set the observer

	control->SetObserver(observer);
	//control->SetTexture(texture);

	// Created GuiControls are added to the list of controls
	if (control != nullptr) controls.add(control);

	return control;
}

bool GuiManager::Update(float dt)
{	
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	if (app->sceneGame->active == true)
	{
		hud = true;
		pauseMenuActive = app->sceneGame->pause;
	}
	else
	{
		hud = false;
		pauseMenuActive = false;
	}

	if (app->sceneStart->active == true && app->sceneStart->credits == false) mainMenuActive = true;
	else mainMenuActive = false;


	UpdateAll(dt,doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}
	
	return true;
}

bool GuiManager::UpdateAll(float dt, bool doLogic) {

	if (doLogic) {

		if (mainMenuActive == true)
		{
			ListItem<GuiControl*>* control = mainMenu.start;
			while (control != nullptr)
			{
				if (control->data->id == 2 && app->sceneGame->saved == false)
				{
					control = control->next;
					continue;
				}
				control->data->Update(dt);
				control = control->next;
			}
		}

		if (settingsMenuActive == true)
		{
			ListItem<GuiControl*>* control = settingsMenu.start;
			while (control != nullptr)
			{
				control->data->Update(dt);
				control = control->next;
			}
		}

		if (pauseMenuActive == true)
		{
			ListItem<GuiControl*>* control = pauseMenu.start;
			while (control != nullptr)
			{
				control->data->Update(dt);
				control = control->next;
			}
		}
		

	}
	return true; 

}

bool GuiManager::PostUpdate()
{
	Draw();
	return true;
}

bool GuiManager::Draw() {

	/*ListItem<GuiControl*>* control = controls.start;

	while (control != nullptr)
	{
		control->data->Draw(app->render);
		control = control->next;
	}*/

	iPoint origin = {-app->render->camera.x, -app->render->camera.y};

	if (mainMenuActive==true)
	{
		SDL_Rect rec = { origin.x,origin.y,1280,720 };
		app->render->DrawRectangle(rec, 0, 0, 0, 80);
		app->render->DrawTexture(mainMenuTitle, origin.x, origin.y);

		ListItem<GuiControl*>* control = mainMenu.start;
		while (control != nullptr)
		{
			control->data->Draw(app->render);
			control = control->next;
		}
	}

	if (hud == true)
	{

		app->render->DrawTexture(hudImg, origin.x, origin.y);
		// hearts

		for (int i = 0; i < app->player->lives; i++)
		{
			app->render->DrawTexture(heart, origin.x + 20 + (i * 120), origin.y + 17);
		}

		// score
		SString score("%6d", (int)app->player->scoreHud);
		app->fonts->BlitText(origin.x + 700, origin.y + 60, fontColor, score.GetString());
		SString hscore("%6d", app->sceneGame->highScore);
		app->fonts->BlitText(origin.x + 700, origin.y + 15, 0, hscore.GetString());

		// level
		SString level("%1d", app->sceneGame->level);
		app->fonts->BlitText(origin.x + 1220, origin.y + 15, 0, level.GetString());

		// time
		SString min("%2d", (int)app->sceneGame->timer / 60);
		SString sec("%2d", (int)app->sceneGame->timer % 60);
		app->fonts->BlitText(origin.x + 1113, origin.y + 60, 0, min.GetString());
		app->fonts->BlitText(origin.x + 1163, origin.y + 60, 0, sec.GetString());

	}

	if (pauseMenuActive==true) // nose pq no funciona
	{

		SDL_Rect rec = { origin.x,origin.y,1280,720 };
		app->render->DrawRectangle(rec, 0, 0, 0, 80);
		//app->render->DrawTexture(pauseMenuTitle, origin.x, origin.y);
		ListItem<GuiControl*>* control = pauseMenu.start;
		while (control != nullptr)
		{
			control->data->Draw(app->render);
			control = control->next;
		}
	}

	if (settingsMenuActive==true)
	{
		//app->render->DrawTexture(settingsMenuTitle, origin.x, origin.y);
		ListItem<GuiControl*>* control = settingsMenu.start;
		while (control != nullptr)
		{
			control->data->Draw(app->render);
			control = control->next;
		}
	}

	
	

	return true;

}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = controls.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;
}

bool GuiManager::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case 3:
	case 7:			settingsMenuActive = true;		break;

	default:
		break;
	}

	return true;
}

