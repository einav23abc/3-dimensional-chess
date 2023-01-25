#include "Window.hpp"
#include <Windows.h>

#define FPS 60
#define FRAME_DELAY (1000/FPS)

Window* window = nullptr;

int main ( int argc , char *argv[] ) {
    
    //FreeConsole();

    Uint32 frameStart;
    int frameTime;

    window = new Window( "3D chess" , 100 , 100 , (8*(8*8+7))*2 , 8*8*2 , (8*(8*8+7)) , 8*8 , true, false );


    while ( window->running() ){
        frameStart = SDL_GetTicks();

        window->handleEvents();
        window->update();
        window->render();

        frameTime = SDL_GetTicks() - frameStart;
        if ( FRAME_DELAY > frameTime ){
            SDL_Delay( FRAME_DELAY - frameTime );
        };
    };

    window->clean();

    return 0;
};