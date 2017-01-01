#include "Globals.h"
#include "Application.h"
#include "CreaturePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "src\pugixml.hpp"
#include "CreatureEnemy.h"
#include "EntityManager.h"

CreaturePlayer::CreaturePlayer(bool start_enabled) : EntityCreature(PLAYER1, start_enabled)
{
	position.x = 120;
	position.y = 216;
	speed = 1;
	//int n = 20+2+2;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data.xml");
	pugi::xml_node config = doc.document_element();
	pugi::xml_node entities = config.child("entities");
	pugi::xml_node player1 = entities.child("player1");	


	// Rotate kick jump
	pugi::xml_node rotate_kick_jumpXML = player1.child("rotate_kick_jump");
	rotate_kick_jump.frames.push_back(
	{
		rotate_kick_jumpXML.child("frame1").attribute("x").as_int(),
		rotate_kick_jumpXML.child("frame1").attribute("y").as_int(),
		rotate_kick_jumpXML.child("frame1").attribute("w").as_int(),
		rotate_kick_jumpXML.child("frame1").attribute("h").as_int()
	});
	rotate_kick_jump.frames.push_back(
	{
		rotate_kick_jumpXML.child("frame2").attribute("x").as_int(),
		rotate_kick_jumpXML.child("frame2").attribute("y").as_int(),
		rotate_kick_jumpXML.child("frame2").attribute("w").as_int(),
		rotate_kick_jumpXML.child("frame2").attribute("h").as_int()
	});
	rotate_kick_jump.frames.push_back(
	{
		rotate_kick_jumpXML.child("frame3").attribute("x").as_int(),
		rotate_kick_jumpXML.child("frame3").attribute("y").as_int(),
		rotate_kick_jumpXML.child("frame3").attribute("w").as_int(),
		rotate_kick_jumpXML.child("frame3").attribute("h").as_int()
	});
	rotate_kick_jump.frames.push_back(
	{
		rotate_kick_jumpXML.child("frame4").attribute("x").as_int(),
		rotate_kick_jumpXML.child("frame4").attribute("y").as_int(),
		rotate_kick_jumpXML.child("frame4").attribute("w").as_int(),
		rotate_kick_jumpXML.child("frame4").attribute("h").as_int()
	});

	rotate_kick_jump.speed = rotate_kick_jumpXML.attribute("speed").as_float();

	// Butting
	pugi::xml_node buttingXML = player1.child("head_butt");
	head_butt =
	{
		buttingXML.attribute("x").as_int(),
		buttingXML.attribute("y").as_int(),
		buttingXML.attribute("w").as_int(),
		buttingXML.attribute("h").as_int()
	};

}

CreaturePlayer::~CreaturePlayer()
{}

// Load assets
bool CreaturePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("Genesis 32X SCD - Double Dragon III The Rosetta Stone - Billy & Jimmy2.png");

	creature_state = IDLE;

	return true;
}

// Unload assets
bool CreaturePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update
update_status CreaturePlayer::Update()
{
	UpdateProfundity();

	SDL_Rect billy = right_down.frames[0];
	static bool flip = false; // When the character goes left is true
	int newSpeed = getSpeed();
	//state previous_state;
	static int counter = 0;

	/*if (creatureCollider->collisionMatrix[0][1]) //Player - Enemy
	{
		//screature_state = previous_state;
		for (list<ModuleEntity*>::iterator it = App->entityManager->entities.begin(); it != App->entityManager->entities.end(); ++it)
		{
			CreatureEnemy* e = (CreatureEnemy*)(*it);
			/*if (e->enemy = e->punch.frames[2])
			{
				creature_state = DAMAGED;
			}
			creature_state = DAMAGED;
		}
	}*/


	switch (creature_state)
	{
	//case IDLE:
		
		//break;
	case ATTACKING:
		if (!isAttacking())
			break;
		billy = getAttack();
		break;
	case JUMPING:
		isJumping();
		if (isAttacking())
		{
			billy = getAttack();
			break;
		}
		billy = jump;
		break;
	case DAMAGED:
		++counter;
		if (counter < 24)
		{
			billy = damaged;
		}
		else
		{
			creature_state = IDLE;
			counter = 0;
		}
		break;
	default:
		if (isAttacking())
		{
			creature_state = ATTACKING;
			break;
		}
		if (isJumping())
			break;

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			flip = false;
			position.x += newSpeed;
			billy = right_down.GetCurrentFrame();
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			flip = true;
			position.x -= newSpeed;
			billy = right_down.GetCurrentFrame();
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			if (!creatureCollider->collisionMatrix[0][2])//Player-Wall
				position.y -= speed;
			billy = up.GetCurrentFrame();
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			position.y += speed;
			billy = right_down.GetCurrentFrame();
		}
		break;
	}

	creatureCollider->SetPos(position.x, position.y - 64);

	App->renderer->Blit(graphics, position.x + speed, position.y - billy.h, &(billy), 1.0f, flip);
	return UPDATE_CONTINUE;
}

/**************************************************************/
int CreaturePlayer::getSpeed()
{
	//static bool running = false;
	static bool walking = false;
	static int time = 0;
	static int maxTime = 24; 

	if (running)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			running = false;
			return speed;
		}
		return speed * 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		if (walking && time < maxTime)
		{
			running = true;
			walking = false;
			time = 0;
			return speed * 2;
		}
		else
		{
			walking = true;
			time = 0;
		}
	}
	++time;
	
	return speed;
}

//------------------------------------------------------------------
bool CreaturePlayer::isAttacking()
{
	if (currentAttack != NULL)
	{
		if (punch.AnimationFinished())
		{
			currentAttack = 0;
			creature_state = IDLE;
			return false;
		}
		if (kick.AnimationFinished())
		{
			currentAttack = 0;
			creature_state = IDLE;
			return false;
		}

		return true;
	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_COMMA) == KEY_DOWN)
		{
			currentAttack = SDL_SCANCODE_COMMA;
			if (running)
				isButting = true;
		}
		else if (App->input->GetKey(SDL_SCANCODE_PERIOD) == KEY_DOWN)
		{
			currentAttack = SDL_SCANCODE_PERIOD;
		}
		return false;
	}
		
}

//-------------------------------------------------------------------
SDL_Rect& CreaturePlayer::getAttack()
{
	switch (currentAttack)
	{
	case SDL_SCANCODE_COMMA: //Punch
		if (creature_state == JUMPING)
			return jump;
		if (isButting)
		{
			HeadButt();
			return head_butt;
		}
		return punch.GetCurrentFrame();
		break;

	case SDL_SCANCODE_PERIOD: //Kick
		if (creature_state == JUMPING)
			return kick_jump;
		return kick.GetCurrentFrame();
		break;
	}
}

//--------------------------------------------------------------------
bool CreaturePlayer::isJumping()
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || creature_state == JUMPING)
	{
		creature_state = JUMPING;
		Jump();
		return true;
	}
	return false;
}
//--------------------------------------------------------------------
void CreaturePlayer::Jump()
{
	static int y_ini = -1;
	float aceleration = 0.3f;
	static float jump_speed = 5;
	static bool jumpDirection[] = {false, false}; //right, left

	if (y_ini == -1)// Init
	{
		y_ini = position.y;
		position.y -= 1;
		jump_speed = 5;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			jumpDirection[0] = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			jumpDirection[1] = true;
		}
	}
	else if (y_ini <= position.y) 
	{
		position.y = y_ini;
		y_ini = -1;
		creature_state = IDLE;
		jumpDirection[0] = false;
		jumpDirection[1] = false;
		currentAttack = 0;
	}
	else
	{
		jump_speed -= aceleration;
		position.y -= jump_speed;

		if (jumpDirection[0])
			position.x += 2;
		else if(jumpDirection[1])
			position.x -= 2;
	}
}
/*****************************************/
void CreaturePlayer::HeadButt()
{
	//static bool isButting = false;
	static int y_ini = -1;
	float aceleration = 0.5f;
	static float jump_speed = 5;
	static bool jumpDirection[] = { false, false }; //right, left
	running = false;
	if (y_ini == -1)
	{
		y_ini = position.y;
		--position.y;
		jump_speed = 5;
		running = false;
		isButting = true;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			jumpDirection[0] = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			jumpDirection[1] = true;
		}

	}else if (y_ini <= position.y)
	{
		creature_state = IDLE;
		isButting = false;
		running = false;
		jumpDirection[0] = false;
		jumpDirection[1] = false;
		currentAttack = 0;
		position.y = y_ini;
		y_ini = -1;
	}
	else
	{
		jump_speed -= aceleration;
		position.y -= jump_speed;

		if (jumpDirection[0])
			position.x += 3;
		else if (jumpDirection[1])
			position.x -= 3;
	}

}

/*****************************************/
int CreaturePlayer::getCollision() //Return the type of collision (enemy attack, a wall,...)
{
	int ret;

	//for
	//creatureCollider

	return ret;
}