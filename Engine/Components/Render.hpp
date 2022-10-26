#ifndef RENDER_COMPONENT_HPP

#define RENDER_COMPONENT_HPP

#include "../DDEngine.hpp"

namespace DDEngine
{

    class Render : public Component
    {

    public:

        class RenderTarget
        {

        public:

            RenderTarget(Transform* t)
            {
                transform = t;
            }

            virtual void render(Renderer* renderer) {}

        protected:

            Transform* transform;

        };

        class ImageTarget : public RenderTarget
        {

        public:

            ImageTarget(Transform* t) : RenderTarget(t) {}

            SDL_Texture* texture;

            void render(Renderer* renderer) override
            {
                if(texture)
                    renderer->DrawTexture(texture, transform->ToRect(), transform->rotation);
            }

        };

        class RectTarget : public RenderTarget
        {

        public:

            RectTarget(Transform* t) : RenderTarget(t) {}

            Color* color;

            void render(Renderer* renderer) override
            {
                if(color && colorcopy != *color || firsttime)
                {
                    SDL_Surface* sur = SDL_CreateRGBSurface(0, transform->scale.w, transform->scale.h, 32, 0, 0, 0, 0);
                    SDL_FillRect(sur, transform->ToRect(), SDL_MapRGBA(sur->format, color->r, color->g, color->b, color->a));
                    tex = SDL_CreateTextureFromSurface(renderer->GetInternal(), sur);
                    SDL_FreeSurface(sur);
                    firsttime = false;
                }
                colorcopy = *color;
                renderer->DrawTexture(tex, transform->ToRect(), transform->rotation);
            }

        private:

            Color colorcopy;
            bool firsttime = true;
            SDL_Texture* tex;

        };

        RenderTarget* target = nullptr;

        void render(Renderer* renderer) override
        {
            if(target)
                target->render(renderer);
        }

        string name() override { return "Render"; }

    };

}

#endif // !RENDER_COMPONENT_HPP