#include "../Components/Player.hpp"

void Object00(GameObject* go)
{

    Player* player = go->addComponent<Player>();

    player->message = "Working fine!";

    RectRender* rect = go->addComponent<RectRender>();

    rect->color = Color(255, 255, 255);

    go->transform.scale = Scale(100, 100);

}

