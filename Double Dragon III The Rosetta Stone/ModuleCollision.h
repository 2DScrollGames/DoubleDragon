#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"

//struct SDL_Rect;


enum collider_type
{
	PLAYER = 0,
	ENEMY,
	EXIT,
	WALL_UP,
	WALL_DOWN,
	WALL_RIGHT,
	WALL_LEFT,
	SPAWN
};
struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	bool colliding = false;
	bool to_delete = false;

	collider_type type;

	bool collisionArray[7] = {false, false, false, false, false, false, false}; //Player, enemy, wallup, walldown, wallright, wallleft, exit, spawn

	Collider(SDL_Rect rectangle) : // expand this call if you need to
		rect(rectangle)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate();
	update_status Update();

	bool CleanUp();

	Collider* AddCollider(const SDL_Rect& rect, const collider_type& type);
	void DebugDraw();

	std::list<Collider*> colliders;

private:
	bool debug = false;
};

#endif // __ModuleCollision_H__