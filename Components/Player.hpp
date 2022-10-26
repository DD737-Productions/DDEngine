#ifndef PLAYER_COMPONENT_HPP

#define PLAYER_COMPONENT_HPP

#include "../DDEngine.hpp"

using namespace DDEngine;

class Player : public Component
{

public:

    string message = "hihihi";

    void start() override
    {
        debug.log(message);
    }

    void update() override
    {

        bool w, a, s, d, e, q;
        w = inputManager->isdown(SDLK_w);
        a = inputManager->isdown(SDLK_a);
        s = inputManager->isdown(SDLK_s);
        d = inputManager->isdown(SDLK_d);
        e = inputManager->isdown(SDLK_e);
        q = inputManager->isdown(SDLK_q);

        int speed = 10;
        int rspeed = 5;

        if(e && q) {}
        else if(e)
            transform->rotation += rspeed;
        else if(q)
            transform->rotation -= rspeed;

        if(w && s) {}
        else if(w)
            transform->position.y -= speed;
        else if(s)
            transform->position.y += speed;

        if(a && d) {}
        else if(a)
            transform->position.x -= speed;
        else if(d)
            transform->position.x += speed;

    }

    string name() override { return "Player.hpp"; }

};

#endif // !PLAYER_COMPONENT_HPP