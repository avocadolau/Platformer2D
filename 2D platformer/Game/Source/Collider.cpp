#include "Collider.h"

Collider::Collider()
{

}

Collider::Collider(SDL_Rect rectangle, Type type, Module* listener, Entity* entity) : rect(rectangle), type(type), entity(entity)
{
	listeners[0] = listener;
	rect = rectangle;
}

Collider::~Collider()
{
	
}

void Collider::SetPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

bool Collider::Intersects(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}

bool Collider::Inside(const SDL_Rect& r) const
{
	bool ret = false;
	if (rect.x >= r.x && rect.x < r.x + r.w)
		if (rect.y >= r.y && rect.y < r.y + r.h)
			ret = true;

	return ret;
}

void Collider::AddListener(Module* listener)
{
	for (int i = 0; i < MAX_LISTENERS; ++i)
	{
		if (listeners[i] == nullptr)
		{
			listeners[i] = listener;
			break;
		}

		else if (listeners[i] == listener)
			break;
	}
}