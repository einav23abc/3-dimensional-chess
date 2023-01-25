#ifndef Window_HPP
#define Window_HPP

#include "LinkedList.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string.h>

typedef void (*voidfuncptr_t)(void* This);

class Window {
    public:
        Window(const char* title,int xpos,int ypos,int _width,int _height,int minWidth,int minHeight,bool resizable,bool fullscreen);
        ~Window();
        void handleEvents();
        void update();
        void render();
        void clean();
        bool running(){ return isRunning; };
#define MAX_TEXT_INPUT_LENGTH 12
        char textInput[MAX_TEXT_INPUT_LENGTH];
        int textInputBackspaces = 0;
        int width, height;
        int mouseX, mouseY;
        bool mouseLeftDown;
        bool mouseRightDown;
        void listenToMouseLeftPressEvent( void* listner , voidfuncptr_t funcptr );
        void listenToMouseRightPressEvent( void* listner , voidfuncptr_t funcptr );
    private:
        LinkedListNode<voidfuncptr_t>* mouseLeftPressEventListenFuncList = nullptr;
        LinkedListNode<voidfuncptr_t>* mouseLeftPressEventListenFuncListTail = nullptr;
        LinkedListNode<void*>* mouseLeftPressEventListenListnerList = nullptr;
        LinkedListNode<void*>* mouseLeftPressEventListenListnerListTail = nullptr;
        LinkedListNode<voidfuncptr_t>* mouseRightPressEventListenFuncList = nullptr;
        LinkedListNode<voidfuncptr_t>* mouseRightPressEventListenFuncListTail = nullptr;
        LinkedListNode<void*>* mouseRightPressEventListenListnerList = nullptr;
        LinkedListNode<void*>* mouseRightPressEventListenListnerListTail = nullptr;
        bool isRunning;
        SDL_Window* window;
        SDL_Renderer* renderer;
        void* app; /* supose to be (App3D*) app */
};

#include "App2D.hpp"
#include "App3D.hpp"
#include "Window.cpp"
#endif