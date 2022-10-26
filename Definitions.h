#ifndef DEFINITIONS_HPP

#define DEFINITIONS_HPP

#define TITLE "game"
#define WIDTH 800
#define HEIGHT 600
#define FULLSCREEN false
#define BGR 100
#define BGG 100
#define BGB 100
#define BGA 255

#include "InitManager.hpp"

#include "Objects/Object00.hpp"

namespace ObjectDefinitions
{

    static void Define()
    {

        InitManager::requestGameObject(Object00);

    }

};

#endif // !DEFINITIONS_HPP