

Window::Window( const char* title , int xpos , int ypos , int _width , int _height , int minWidth , int minHeight , bool resizable , bool fullscreen ){
    this->width = _width;
    this->height = _height;
    
    Uint32 flags = 0;
    if ( fullscreen ){
        flags = SDL_WINDOW_FULLSCREEN;
    };
    if ( resizable ){
        flags |= SDL_WINDOW_RESIZABLE;
    };

    if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ){
        this->isRunning = false;
        return;
    };
    
    std::cout << "Window Subsystems Initialised!..." << std::endl;
    this->window = SDL_CreateWindow( title , xpos , ypos , width , height , flags );
    SDL_SetWindowMinimumSize(this->window, minWidth , minHeight);
    
    if ( this->window ){
        std::cout << "Window Window Created!" << std::endl;
    };

    this->renderer = SDL_CreateRenderer( this->window , -1 , 0 );
    if ( this->renderer ){
        std::cout << "Window Renderer Created!" << std::endl;
    };

    textInput[0] = '\0';
    this->app = new App3D(this,this->renderer);

    this->isRunning = true;
    
    return;
};

Window::~Window(){
    return;
};

void Window::handleEvents(){

    SDL_Event event;
    while( SDL_PollEvent(&event) ){
        SDL_GetMouseState(&this->mouseX, &this->mouseY);

        if ( event.type == SDL_QUIT ){
            isRunning = false;
        }else if ( event.type == SDL_WINDOWEVENT ){
            if ( event.window.event == SDL_WINDOWEVENT_RESIZED ){
                SDL_GetWindowSize( window, &width, &height );
            };
        }else if( event.type == SDL_MOUSEBUTTONDOWN ){
            if ( event.button.button == SDL_BUTTON_LEFT ){
                this->mouseLeftDown = true;
                LinkedListNode<voidfuncptr_t>* currentFunc = mouseLeftPressEventListenFuncList;
                LinkedListNode<void*>* currentListner = mouseLeftPressEventListenListnerList;
                while( currentFunc != nullptr && currentListner != nullptr ){
                    voidfuncptr_t func = currentFunc->getValue();
                    func( currentListner->getValue() );
                    currentFunc = currentFunc->getNext();
                    currentListner = currentListner->getNext();
                };
            }else if ( event.button.button == SDL_BUTTON_RIGHT ){
                this->mouseRightDown = true;
                LinkedListNode<voidfuncptr_t>* currentFunc = mouseRightPressEventListenFuncList;
                LinkedListNode<void*>* currentListner = mouseRightPressEventListenListnerList;
                while( currentFunc != nullptr && currentListner != nullptr ){
                    voidfuncptr_t func = currentFunc->getValue();
                    func( currentListner->getValue() );
                    currentFunc = currentFunc->getNext();
                    currentListner = currentListner->getNext();
                };
            };
        }else if( event.type == SDL_MOUSEBUTTONUP ){
            if ( event.button.button == SDL_BUTTON_LEFT ){
                this->mouseLeftDown = false;
            }else if ( event.button.button == SDL_BUTTON_RIGHT ){
                this->mouseRightDown = false;
            };
        }else if ( event.type == SDL_KEYDOWN ){
            //std::cout << SDL_GetKeyName( event.key.keysym.sym ) << std::endl;
            if ( strcmp( SDL_GetKeyName( event.key.keysym.sym ) , "Backspace" ) == 0 ){
                if ( strlen( textInput ) >= 1 ){
                    textInput[strlen( textInput )-1] = '\0';
                }else{
                    textInputBackspaces++;
                };
                std::cout << "\"" << textInput << "\" del:" << textInputBackspaces << std::endl;
            }else if ( strcmp( SDL_GetKeyName( event.key.keysym.sym ) , "Return" ) == 0 ){
                if ( event.key.keysym.mod & KMOD_LSHIFT ){
                    if ( strlen( textInput ) + 1 >= MAX_TEXT_INPUT_LENGTH ){
                        textInput[1] = '\0';
                        textInput[0] = '\3';
                        textInputBackspaces = 0;
                    }else{
                        textInput[strlen(textInput)+1] = '\0';
                        textInput[strlen(textInput)] = '\3';
                    };
                }else{
                    if ( strlen( textInput ) + 1 >= MAX_TEXT_INPUT_LENGTH ){
                        textInput[1] = '\0';
                        textInput[0] = '\n';
                        textInputBackspaces = 0;
                    }else{
                        textInput[strlen(textInput)+1] = '\0';
                        textInput[strlen(textInput)] = '\n';
                    };
                };
                std::cout << "\"" << textInput << "\" del:" << textInputBackspaces << std::endl;
            }else if ( strcmp( SDL_GetKeyName( event.key.keysym.sym ) , "Tab" ) == 0 ){
                if ( strlen( textInput ) + 4 >= MAX_TEXT_INPUT_LENGTH ){
                    strcpy( textInput , "    " );
                    textInputBackspaces = 0;
                }else{
                    strcat( textInput , "    " );
                };
                std::cout << "\"" << textInput << "\" del:" << textInputBackspaces << std::endl;
            };
        }else if ( event.type == SDL_KEYUP ){

        }else if ( event.type == SDL_TEXTINPUT ){
            if ( strlen( textInput ) + strlen(event.text.text) >= MAX_TEXT_INPUT_LENGTH ){
                strcpy( textInput , "" );
                textInputBackspaces = 0;
            };
            strcat(textInput, event.text.text );
            std::cout << "\"" << textInput << "\" del:" << textInputBackspaces << std::endl;
        }else {

        };
    };
    return;
};

void Window::update(){
    ((App3D*)this->app)->Update();
    return;
};

void Window::render(){
    SDL_SetRenderDrawColor( this->renderer , 0 , 0 , 0 , 255 );
    SDL_RenderClear(this->renderer);

    ((App3D*)this->app)->Render();

    SDL_RenderPresent(this->renderer);
    return;
};

void Window::clean(){
    delete ((App3D*)this->app);
    SDL_DestroyWindow(this->window);
    SDL_DestroyRenderer(this->renderer);
    SDL_Quit();
    std::cout << "Window Cleaned" << std::endl;
    return;
};


void Window::listenToMouseLeftPressEvent( void* listner , voidfuncptr_t funcptr ){
    if ( mouseLeftPressEventListenFuncList == nullptr ){
        mouseLeftPressEventListenFuncList = new LinkedListNode<voidfuncptr_t>(funcptr);
        mouseLeftPressEventListenFuncListTail = mouseLeftPressEventListenFuncList;
        mouseLeftPressEventListenListnerList = new LinkedListNode<void*>(listner);
        mouseLeftPressEventListenListnerListTail = mouseLeftPressEventListenListnerList;
        return;
    };
    mouseLeftPressEventListenFuncListTail->setNext( new LinkedListNode<voidfuncptr_t>(funcptr) );
    mouseLeftPressEventListenFuncListTail = mouseLeftPressEventListenFuncListTail->getNext();
    mouseLeftPressEventListenListnerListTail->setNext( new LinkedListNode<void*>(listner) );
    mouseLeftPressEventListenListnerListTail = mouseLeftPressEventListenListnerListTail->getNext();
    return;
};
void Window::listenToMouseRightPressEvent( void* listner , voidfuncptr_t funcptr ){
    if ( mouseRightPressEventListenFuncList == nullptr ){
        mouseRightPressEventListenFuncList = new LinkedListNode<voidfuncptr_t>(funcptr);
        mouseRightPressEventListenFuncListTail = mouseRightPressEventListenFuncList;
        mouseRightPressEventListenListnerList = new LinkedListNode<void*>(listner);
        mouseRightPressEventListenListnerListTail = mouseRightPressEventListenListnerList;
        return;
    };
    mouseRightPressEventListenFuncListTail->setNext( new LinkedListNode<voidfuncptr_t>(funcptr) );
    mouseRightPressEventListenFuncListTail = mouseRightPressEventListenFuncListTail->getNext();
    mouseRightPressEventListenListnerListTail->setNext( new LinkedListNode<void*>(listner) );
    mouseRightPressEventListenListnerListTail = mouseRightPressEventListenListnerListTail->getNext();
    return;
};