#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	detector = bounds;
	texture = app->tex->Load(text);

	normalRec = bounds;
	normalRec.y = 0;
	focusedRec = normalRec;
	focusedRec.y += focusedRec.h;
	pressedRec = focusedRec;
	pressedRec.y+=pressedRec.h;

	canClick = true;
	drawBasic = false;
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L14: TODO 3: Update the state of the GUiButton according to the mouse position
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		

		if ((mouseX > detector.x ) && (mouseX < (detector.x + detector.w )) &&
			(mouseY > detector.y ) && (mouseY < (detector.y + detector.h )))
		{
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiButton::Draw(Render* render)
{

	iPoint p = { -app->render->camera.x, -app->render->camera.y };


	// Draw the right button depending on state
	switch (state)
	{

	case GuiControlState::DISABLED: 
	{
		render->DrawRectangle(bounds, 0, 0, 0, 0);
	} break;

	case GuiControlState::NORMAL:
	{
		//render->DrawRectangle(bounds, 255, 0, 0, 255);

		render->DrawTexture(texture, bounds.x, bounds.y, &normalRec);

	} break;

	//L14: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		//render->DrawRectangle(bounds, 255, 255, 255, 160);
		render->DrawTexture(texture, bounds.x, bounds.y, &focusedRec);
	} break;
	case GuiControlState::PRESSED:
	{
		//render->DrawRectangle(bounds, 255, 255, 255, 255);
		render->DrawTexture(texture, bounds.x, bounds.y, &pressedRec);
	} break;

	/******/

	case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;

	default:
		break;
	}

	//render->DrawRectangle(detector, 255, 255, 255, 160);



	return false;
}

void GuiButton::SetDetector(int x, int y)
{
	//detector = { bounds.x - x,bounds.y - y,bounds.w + (2 * x), bounds.h + (2 * y) };
	detector.x += x;
	detector.y += y;
	detector.w -= 2 * x;
	detector.h -= 2 * y;
}