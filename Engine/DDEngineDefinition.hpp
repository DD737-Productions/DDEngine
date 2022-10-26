#ifndef DDENGINE_DEFINITION_HPP

#define DDENGINE_DEFINITION_HPP

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "DDEngineInternal.hpp"

using namespace std;

namespace DDEngine
{

    class Color
    {

    public:

        Color() : Color(0, 0, 0) {}
        Color(Uint8 r, Uint8 g, Uint8 b) : Color(r, g, b, 255) {}
        Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        Uint8 r, g, b, a;

        bool operator==(Color o)
        {
            return r == o.r && g == o.g && b == o.b && a == o.a;
        }
        bool operator!=(Color o) { return !((*this) == o); }

    };

    using KeyCode = SDL_Keycode;
    
    class Renderer
    {

    public:

        Renderer(SDL_Renderer* renderer)
        {
            this->renderer = renderer;
        }

        SDL_Renderer* GetInternal() { return renderer; }

        void Clear()
        {
            SDL_RenderClear(renderer);
        }
        void Present()
        {
            SDL_RenderPresent(renderer);
        }

        void SetColor(Color c)
        {
            SetColor(c.r, c.g, c.b, c.a);
        }
        void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
        {
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
        }

        void FillRect(int x, int y, int w, int h)
        {
            SDL_Rect rect;
            rect.x = x;
            rect.y = y;
            rect.w = w;
            rect.h = h;
            FillRect(&rect);
        }
        void FillRect(SDL_Rect* rect)
        {
            SDL_RenderFillRect(renderer, rect);
        }

        void DrawTexture(SDL_Texture* texture, SDL_Rect* dimensions)
        {
            SDL_RenderCopy(renderer, texture, nullptr, dimensions);
        }
        void DrawTexture(SDL_Texture* texture, SDL_Rect* dimensions, double rotation)
        {
            SDL_RenderCopyEx(renderer, texture, nullptr, dimensions, rotation, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
        }

    private:

        SDL_Renderer* renderer;

    };

    class GameObject;
    class InputManager;
    class AppInfo;

    class DebugManager
    {

    public:

        virtual void log(string message)
        {
            write("LOG: ", message);
        }
        virtual void logWarning(string warning)
        {
            write("WARNING: ", warning);
        }
        virtual void logError(string error)
        {
            write("ERROR: ", error);
        }
        virtual void print(string text)
        {
            cout << text << endl;
        }

    private:

        void write(string prefix, string message)
        {
            print(prefix + message);
        }

    };

    class Vector
    {

    public:

        Vector() : Vector(0, 0) {}
        Vector(int x, int y)
        {
            this->x = x;
            this->y = y;
        }

        int x, y;

    };

    class Scale
    {

    public:

        Scale() : Scale(0, 0) {}
        Scale(int width, int height)
        {
            this->w = width;
            this->h = height;
        }

        int w, h;

    };

    class Transform
    {

    public:

        Vector position;
        Scale scale;
        double rotation = 0;

        void Translate(Vector vec)
        {
            Translate(vec.x, vec.y);
        }
        void Translate(int x, int y)
        {
            position.x += x;
            position.y += y;
        }

        SDL_Rect* ToRect() 
        {
            rect.x = position.x;
            rect.y = position.y;
            rect.w = scale.w;
            rect.h = scale.h;
            return &rect;
        }

    private:

        SDL_Rect rect;

    };

    class Component
    {

    public:

        virtual void start() {}
        virtual void update() {}
        virtual void render(Renderer* renderer) {}
        virtual void onDestroy() {}

        virtual string name() { return "[UNNAMED COMPONENT]"; }

        template<typename T>
        T* addComponent();
        template<typename T>
        bool hasComponent();
        template<typename T>
        T* getComponent();

        class Debug
        {

        private:

            DebugManager* internal;
            Component* component;

        public:

            Debug() : Debug(nullptr, nullptr) {}
            Debug(DebugManager* from, Component* c) : internal(from), component(c) {}

            void log(string message)
            {
                internal->print(component->name() + ": LOG: " + message);
            }
            void logWarning(string warning)
            {
                internal->print(component->name() + ": WARNING: " + warning);
            }
            void logError(string error)
            {
                internal->print(component->name() + ": ERROR: " + error);
            }

        };

        GameObject* gameObject;
        InputManager* inputManager;
        Transform* transform;
        Debug debug;
        const AppInfo* appInfo;

    };

    class GameObject
    {

    public:

        GameObject()
        {
            for(int i = 0; i < DDEngineInternal::MAX_COMPONENT_COUNT; i++)
                components[i] = nullptr;
        }
        ~GameObject()
        {
            for(int i = 0; i < DDEngineInternal::MAX_COMPONENT_COUNT; i++)
                if(components[i])
                    delete components[i];
        }

        InputManager* inputManager;
        DebugManager* debug;
        Transform transform;
        const AppInfo* appInfo;

        template<typename T>
        T* addComponent()
        {
            DDEngineInternal::CID id = DDEngineInternal::getCID<T>();
            T* c = new T();
            c->gameObject = this;
            c->inputManager = inputManager;
            c->transform = &transform;
            c->appInfo = appInfo;
            c->debug = Component::Debug(debug, c);
            components[id] = c;
            return c;
        }
        template<typename T>
        bool hasComponent()
        {
            return components[DDEngineInternal::getCID<T>()] != nullptr;
        }
        template <typename T>
        T* getComponent()
        {
            Component* ptr = components[DDEngineInternal::getCID<T>()];
            return static_cast<T*>(ptr);
        }

        void start()
        {
            for(int i = 0; i < DDEngineInternal::MAX_COMPONENT_COUNT; i++)
                if(components[i])
                    components[i]->start();
        }
        void update()
        {
            for(int i = 0; i < DDEngineInternal::MAX_COMPONENT_COUNT; i++)
                if(components[i])
                    components[i]->update();
        }
        void render(Renderer* renderer)
        {
            for(int i = 0; i < DDEngineInternal::MAX_COMPONENT_COUNT; i++)
                if(components[i])
                    components[i]->render(renderer);
        }

        void destroy()
        {
            for(int i = 0; i < DDEngineInternal::MAX_COMPONENT_COUNT; i++)
                if(components[i])
                    components[i]->onDestroy();
            alive = false;
        }

        bool isAlive() const { return alive; }

        void setActive(bool state) { active = state; }
        bool isActive() { return active; }

    private:

        bool active = true;
        bool alive = true;

        Component* components[DDEngineInternal::MAX_COMPONENT_COUNT];

    };

#pragma region ext shit

    template<typename T>
    T* Component::addComponent()
    {
        return gameObject->getComponent<T>();
    }
    template<typename T>
    bool Component::hasComponent()
    {
        return gameObject->hasComponent<T>();
    }
    template<typename T>
    T* Component::getComponent()
    {
        return gameObject->getComponent<T>();
    }

#pragma endregion



    class InputManager
    {

    private:

        class State
        {

        public:

            State() : State((KeyCode)0, false) {}
            State(KeyCode c, bool d)
            {
                this->code = c;
                this->down = d;
            }

            KeyCode code = (KeyCode)0;
            bool down = false;

        };

        vector<State> states;

        State* FindState(KeyCode code)
        {
            for(size_t i = 0; i < states.size(); i++)
                if(states[i].code == code)
                    return &states[i];
            states.emplace_back(move(State(code, false)));
            return &states[states.size() - 1];
        }

    public:

        ~InputManager()
        {

            states.clear();

        }

        void events(SDL_Event* event)
        {
            if(event->type == SDL_KEYUP)
                FindState(event->key.keysym.sym)->down = false;
            else if(event->type == SDL_KEYDOWN)
                FindState(event->key.keysym.sym)->down = true;
        }

        bool isdown(KeyCode code)
        {
            return FindState(code)->down;
        }
        bool isup(KeyCode code)
        {
            return !isdown(code);
        }

        KeyCode anydown()
        {
            for(size_t i = 0; i < states.size(); i++)
                if(states[i].down)
                    return states[i].code;
            return(KeyCode)0;
        }

    };

    class AppInfo
    {

    public:

        int WindowWidth, WindowHeight;
        string WindowTitle;
        Color Background;

    };

    class Game
    {

    public:

        void Init(string title, int width, int height, Color bg, bool fullScreen)
        {

            info.WindowWidth = width;
            info.WindowHeight = height;
            info.WindowTitle = title;
            info.Background = bg;
            this->bg = bg;

            if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
                throw runtime_error("NOPE INIT");

            int flags = SDL_WINDOW_SHOWN;
            if(fullScreen)
                flags = SDL_WINDOW_FULLSCREEN;

            window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
            if(!window)
                throw runtime_error("NOPE WINDOW");

            SDL_Renderer* r = SDL_CreateRenderer(window, -1, 0);
            if(!r)
                throw runtime_error("NOPE RENDERER");

            renderer = new Renderer(r);

        }

        GameObject* requestGameObject()
        {
            GameObject* go = new GameObject();
            GameObjectWrapper* gow = new GameObjectWrapper();
            gow->go = go;
            go->inputManager = &inputManager;
            go->debug = &debug;
            go->appInfo = &info;
            gameObjects.emplace_back(move(gow));
            return go;
        }

        void Run()
        {

            const int FPS = 60;
            const int frameDelay = 1000 / FPS;
            Uint32 frameStart;
            int frameTime;
            
            for(int i = 0; i < gameObjects.size(); i++)
                if(gameObjects[i]->go->isActive())
                    gameObjects[i]->go->start();

            do
            {

                frameStart = SDL_GetTicks();

                checkGameObjects();
                events();

                int translatex, translatey;
                translatex = translatey = 0;

                update();

                render();

                cleanGameObjects();

                frameTime = SDL_GetTicks() - frameStart;
                if(frameDelay > frameTime)
                    SDL_Delay(frameDelay - frameTime);

            }
            while(event.type != SDL_QUIT);

        }

        void Clean()
        {

            for(int i = 0; i < gameObjects.size(); i++)
                delete gameObjects[i];

            SDL_DestroyRenderer(renderer->GetInternal());
            SDL_DestroyWindow(window);

            delete renderer;

            SDL_Quit();

        }

    private:

        SDL_Window* window;
        Renderer* renderer;
        SDL_Event event;
        InputManager inputManager;
        DebugManager debug;

        AppInfo info;
        Color bg;

        class GameObjectWrapper
        {

        public:

            GameObject* go = nullptr;
            bool active = true;

        };

        vector<GameObjectWrapper*> gameObjects;

        bool running = true;

        void checkGameObjects()
        {
            for(int i = 0; i < gameObjects.size(); i++)
                if(gameObjects[i]->active != gameObjects[i]->go->isActive())
                {
                    if(gameObjects[i]->go->isActive())
                        gameObjects[i]->go->start();
                    gameObjects[i]->active = gameObjects[i]->go->isActive();
                }
        }
        void events()
        {

            SDL_PollEvent(&event);

            if(event.type == SDL_QUIT)
                running = false;

            inputManager.events(&event);

        }
        void update()
        {
            
            for(int i = 0; i < gameObjects.size(); i++)
                if(gameObjects[i]->go->isActive())
                    gameObjects[i]->go->update();

        }
        void render()
        {

            renderer->SetColor(bg);

            renderer->Clear();

            for(int i = 0; i < gameObjects.size(); i++)
                if(gameObjects[i]->go->isActive())
                    gameObjects[i]->go->render(renderer);

            renderer->Present();
            
        }

        void cleanGameObjects()
        {
            gameObjects.erase(
                remove_if(
                    begin(gameObjects),
                    end(gameObjects),
                    [](const GameObjectWrapper* go)
                    {
                        return !go->go->isAlive();
                    }
                ),
                end(gameObjects)
            );
        }

    };

    #include "components/Render.hpp"

}

#endif