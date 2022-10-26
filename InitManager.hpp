#ifndef INIT_MANAGER_HPP

#define INIT_MANAGER_HPP

#include "DDEngine.hpp"

using namespace DDEngine;

using InitLambda = void*(*) (GameObject*);

namespace InitManager
{

    vector<InitLambda> lambdas;

    void requestGameObject(void(*lambda)(GameObject*))
    {
        lambdas.emplace_back(move((InitLambda)lambda));
    }

    void Exec(Game* game)
    {
        for(int i = 0; i < lambdas.size(); i++)
            (lambdas[i])(game->requestGameObject());
    }

};

#endif // !INIT_MANAGER_HPP