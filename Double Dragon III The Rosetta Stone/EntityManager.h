#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <list>
#include "EntityCreature.h"

class ModuleEntity;
enum Types;

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	ModuleEntity* CreateEntity(Types type, creature_type creaturetype, float x = 0.0f, float y = 0.0f, int w = 0, int h = 0);
	void RadixSortList(std::list<ModuleEntity*>& l, int length, int numMax = 255);
	int Digit(int index, int num);
	void Wave(int numEnemy1, int numEnemy2, float posX[], float posY[]);

public:
	std::list<ModuleEntity*> entities;
};

#endif