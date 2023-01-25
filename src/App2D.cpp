
App2D::App2D( Window* parentWindow , SDL_Renderer* renderer ){
    this->renderer = renderer;
    this->parentWindow = parentWindow;

    this->piecesSprite = new Sprite( "./assets/pieces.png" , 8 , 8 , this->renderer );
    this->initBoard();

    this->parentWindow->listenToMouseLeftPressEvent( (void*)this , (voidfuncptr_t)&(this->leftMouseButtonPressed) );
    this->possibleMoves = this->getPossibleMovesWithKingCheck( this->board );

    this->currentPlayer = 0;
    return;
};

App2D::~App2D(){
    for ( int i = 0; i < 8*8; i++ ){
        delete ( this->possibleMoves[i] );
    };
    free(this->possibleMoves);
    return;
};

void App2D::Render(){
    this->printBoard( this->board );
    return;
};

void App2D::Update(){
    return;
};

void App2D::initBoard(){
    if ( this->board != nullptr ){
        free( this->board );
    };
    this->board = (piece_t*)calloc( sizeof(piece_t) , 8*8 );
    if ( this->board == nullptr ){
        std::cout << "could not calloc board at 'void App2D::initBoard()'" << std::endl;
        return;
    };
    
    this->board[ 0+ 0*8].piece = 4; this->board[ 0+ 0*8].team = 1;
    this->board[ 1+ 0*8].piece = 3; this->board[ 1+ 0*8].team = 1;
    this->board[ 2+ 0*8].piece = 2; this->board[ 2+ 0*8].team = 1;
    this->board[ 3+ 0*8].piece = 5; this->board[ 3+ 0*8].team = 1;
    this->board[ 4+ 0*8].piece = 6; this->board[ 4+ 0*8].team = 1;
    this->board[ 5+ 0*8].piece = 2; this->board[ 5+ 0*8].team = 1;
    this->board[ 6+ 0*8].piece = 3; this->board[ 6+ 0*8].team = 1;
    this->board[ 7+ 0*8].piece = 4; this->board[ 7+ 0*8].team = 1;
    
    this->board[ 0+ 1*8].piece = 1; this->board[ 0+ 1*8].team = 1;
    this->board[ 1+ 1*8].piece = 1; this->board[ 1+ 1*8].team = 1;
    this->board[ 2+ 1*8].piece = 1; this->board[ 2+ 1*8].team = 1;
    this->board[ 3+ 1*8].piece = 1; this->board[ 3+ 1*8].team = 1;
    this->board[ 4+ 1*8].piece = 1; this->board[ 4+ 1*8].team = 1;
    this->board[ 5+ 1*8].piece = 1; this->board[ 5+ 1*8].team = 1;
    this->board[ 6+ 1*8].piece = 1; this->board[ 6+ 1*8].team = 1;
    this->board[ 7+ 1*8].piece = 1; this->board[ 7+ 1*8].team = 1;

    
    this->board[ 0+ 6*8].piece = 1;
    this->board[ 1+ 6*8].piece = 1;
    this->board[ 2+ 6*8].piece = 1;
    this->board[ 3+ 6*8].piece = 1;
    this->board[ 4+ 6*8].piece = 1;
    this->board[ 5+ 6*8].piece = 1;
    this->board[ 6+ 6*8].piece = 1;
    this->board[ 7+ 6*8].piece = 1;

    this->board[ 0+ 7*8].piece = 4;
    this->board[ 1+ 7*8].piece = 3;
    this->board[ 2+ 7*8].piece = 2;
    this->board[ 3+ 7*8].piece = 5;
    this->board[ 4+ 7*8].piece = 6;
    this->board[ 5+ 7*8].piece = 2;
    this->board[ 6+ 7*8].piece = 3;
    this->board[ 7+ 7*8].piece = 4;
    return;
};

void App2D::printBoard( piece_t* board ){
    SDL_Rect rect;
    rect.x = ( this->parentWindow->width - std::min( (int)(this->parentWindow->width/8) , (int)(this->parentWindow->height/8) )*8 )/2;
    rect.y = ( this->parentWindow->height - std::min( (int)(this->parentWindow->width/8) , (int)(this->parentWindow->height/8) )*8 )/2;
    rect.w = std::min( (int)(this->parentWindow->width/8) , (int)(this->parentWindow->height/8) );
    rect.h = rect.w;
    // drawing board pattern
    for( int y = 0; y < 8; y++ ){
        for( int x = 0; x < 8; x++ ){
            if ( (x+y)%2 == 0 ){
                SDL_SetRenderDrawColor( this->renderer , 192 , 203 , 220 , 255 );
            }else{
                SDL_SetRenderDrawColor( this->renderer , 90 , 105 , 136 , 255 );
            };
            SDL_RenderFillRect( this->renderer, &rect );

            rect.x += rect.w;
        };
        rect.x -= rect.w*8;
        rect.y += rect.h;
    };
    rect.y -= rect.h*8;


    // drawing mouseHoveredPiece's possible moves
    if (
        this->parentWindow->mouseX >= rect.x &&
        this->parentWindow->mouseX < rect.x+rect.w*8 &&
        this->parentWindow->mouseY >= rect.y &&
        this->parentWindow->mouseY < rect.y+rect.h*8
    ){
        int mouseHoveredPiece = floor( ( this->parentWindow->mouseX - rect.x )/rect.w ) + 8*floor( ( this->parentWindow->mouseY - rect.y )/rect.h );
        // hovered piece
        if ( ( (int)( (mouseHoveredPiece%8)+floor(mouseHoveredPiece/8) ) )%2 == 0 ){
            SDL_SetRenderDrawColor( this->renderer , 99 , 199 , 74 , 255 );
        }else{
            SDL_SetRenderDrawColor( this->renderer , 62 , 137 , 72 , 255 );
        };
        SDL_Rect dstRect;
        dstRect.x = rect.x + rect.w*(mouseHoveredPiece%8);
        dstRect.y = rect.y + rect.w*floor(mouseHoveredPiece/8);
        dstRect.w = rect.w;
        dstRect.h = rect.h;
        SDL_RenderFillRect( this->renderer, &dstRect );
        // possible moves
        LinkedListNode<int>* current = this->possibleMoves[mouseHoveredPiece];
        while( current != nullptr ){
            if ( ((int)((current->getValue()%8)+floor(current->getValue()/8)))%2 == 0 ){
                SDL_SetRenderDrawColor( this->renderer , 99 , 199 , 74 , 255 );
            }else{
                SDL_SetRenderDrawColor( this->renderer , 62 , 137 , 72 , 255 );
            };
            dstRect.x = rect.x + rect.w*(current->getValue()%8);
            dstRect.y = rect.y + rect.h*floor(current->getValue()/8);
            SDL_RenderFillRect( this->renderer, &dstRect );
            current = current->getNext();
        };
    };

    
    // drawing selected piece and selected piece possible moves
    if (
        this->selectedPiece >= 0 &&
        this->selectedPiece < 8*8
    ){
        // selected piece
        if ( ( (int)( (this->selectedPiece%8)+floor(this->selectedPiece/8) ) )%2 == 0 ){
            SDL_SetRenderDrawColor( this->renderer , 254 , 174 , 52 , 255 );
        }else{
            SDL_SetRenderDrawColor( this->renderer , 247 , 118 , 34 , 255 );
        };
        SDL_Rect dstRect;
        dstRect.x = rect.x + rect.w*(this->selectedPiece%8);
        dstRect.y = rect.y + rect.w*floor(this->selectedPiece/8);
        dstRect.w = rect.w;
        dstRect.h = rect.h;
        SDL_RenderFillRect( this->renderer, &dstRect );
        // possible moves
        LinkedListNode<int>* current = this->possibleMoves[this->selectedPiece];
        while( current != nullptr ){
            if ( ((int)((current->getValue()%8)+floor(current->getValue()/8)))%2 == 0 ){
                SDL_SetRenderDrawColor( this->renderer , 254 , 174 , 52 , 255 );
            }else{
                SDL_SetRenderDrawColor( this->renderer , 247 , 118 , 34 , 255 );
            };
            dstRect.x = rect.x + rect.w*(current->getValue()%8);
            dstRect.y = rect.y + rect.h*floor(current->getValue()/8);
            SDL_RenderFillRect( this->renderer, &dstRect );
            current = current->getNext();
        };
    };


    // drawing pieces
    for( int y = 0; y < 8; y++ ){
        for( int x = 0; x < 8; x++ ){
            SDL_Rect srcRect;
            if ( board[x+y*8].piece > 0 ){
                this->piecesSprite->copyFrame( &srcRect, board[x+y*8].piece-1+board[x+y*8].team*6 );
                SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &rect );
            };

            rect.x += rect.w;
        };
        rect.x -= rect.w*8;
        rect.y += rect.h;
    };
    rect.y -= rect.h*8;

    /*
    // drawing selectedPiece's possible moves
    if ( this->selectedPiece >= 0 && this->selectedPiece < 8*8 ){
        LinkedListNode<Pmove*>* current = this->possibleMoves[this->selectedPiece];
        while( current != nullptr ){
            SDL_Rect srcRect, dstRect;
            dstRect.x = rect.x + rect.w*current->getValue()->move.tox;
            dstRect.y = rect.y + rect.w*current->getValue()->move.toy;
            dstRect.w = rect.w;
            dstRect.h = rect.h;
            this->piecesSprite->copyFrame( &srcRect, 12+6*((current->getValue()->move.tox+current->getValue()->move.toy)%2) );
            SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &dstRect );
            current = current->getNext();
        };
    };
    */
    return;
};
