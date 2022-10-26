#ifndef RECT_COMPONENT_HPP

#define RECT_COMPONENT_HPP

#include "Render.hpp"

namespace DDEngine
{

    class RectRender : public Render
    {

    public:

        Color color;

        void start() override
        {
            RectTarget* rt = new RectTarget(transform);
            rt->color = &color;
            target = rt;
        }

        string name() override { return "RectRender"; }

    };

}

#endif // !RECT_COMPONENT_HPP