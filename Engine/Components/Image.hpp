#ifndef IMAGE_COMPONENT_HPP

#define IMAGE_COMPONENT_HPP

#include "Render.hpp"

namespace DDEngine
{

    class Image : public Render
    {

    public:

        SDL_Texture* texture;

        void start() override
        {
            ImageTarget* it = new ImageTarget(transform);
            it->texture = texture;
            target = it;
        }

        string name() override { return "Image"; }

    };

}

#endif // !IMAGE_COMPONENT_HPP