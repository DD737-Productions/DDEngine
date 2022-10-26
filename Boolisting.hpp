#ifndef BOOLISTING_HPP

#define BOOLISTING_HPP

#include "Definitions.h"

void Init(Game* game)
{

    ObjectDefinitions::Define();

    InitManager::Exec(game);

}

#endif // !BOOLISTING_HPP