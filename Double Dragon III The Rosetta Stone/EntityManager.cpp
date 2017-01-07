#include "EntityManager.h"
#include "ModuleEntity.h"
#include "CreaturePlayer.h"
#include "CreatureEnemy.h"
#include <vector>
#include "Point.h"
#include "EntityExit.h"
#include "EntityCreature.h"

EntityManager::EntityManager()
{}

EntityManager::~EntityManager()
{}

ModuleEntity* EntityManager::CreateEntity(Types type, creature_type creaturetype, float x, float y, int w, int h)
{
	//static_assert(ModuleEntity::Types::un)
	ModuleEntity* ret = nullptr;
	switch (type)
	{
	case player: ret = (ModuleEntity*)new CreaturePlayer(); break;
	case enemy: ret = (ModuleEntity*)new CreatureEnemy(creaturetype, x, y); break;
	case exits:
		SDL_Rect rect = {x, y, w, h};
		ret = (ModuleEntity*)new EntityExit(rect); 
		break;
	}

	if (ret != nullptr)
	{
		ret->type = type;
		entities.push_back(ret);
	}

	return ret;
}

/**************************************************************/
void EntityManager::RadixSortList(std::list<ModuleEntity*>& v, int length, int numMax)
{
	vector <ModuleEntity*> v2;
	vector <vector<ModuleEntity*>> c(10);
	int d;

	for (int i = 0; i <= log10(numMax); ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			v2.clear();
			c[j].clear();
		}

		for (auto& k : v)
		{
			d = Digit(i, k->profundity);
			c[d].push_back(k);
		}

		for (int l = 0; l < c.size(); ++l)
			for (int m = 0; m < c[l].size(); ++m)
				v2.push_back(c[l][m]);

		v.clear();

		for (auto& l : v2)
			v.push_back(l);
	}
}

/**************************************************************/
int EntityManager::Digit(int index, int num)
{
	int aux = 1;
	for (int i = index; i > 0; --i)
		aux *= 10;

	num = num / aux;
	num = num % 10;

	return num;
}

/***********************************************************/
void EntityManager::Wave(int numEnemy1, int numEnemy2, float posX[], float posY[])
{
	for (int i = 0; i < numEnemy1; ++i)
	{
		ModuleEntity* e = CreateEntity(Types::enemy, creature_type::ENEMY1, posX[i], posY[i]);

		e->Start();
		e->Enable();
	}
	
	for (int j = numEnemy1; j < numEnemy1 + numEnemy2; ++j)
	{
		ModuleEntity* e2 = CreateEntity(Types::enemy, creature_type::ENEMY2, posX[j], posY[j]);

		e2->Start();
		e2->Enable();
	}
}