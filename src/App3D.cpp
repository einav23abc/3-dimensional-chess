
App3D::App3D( Window* parentWindow , SDL_Renderer* renderer ){
    this->renderer = renderer;
    this->parentWindow = parentWindow;

    this->piecesSprite = new Sprite( "./assets/pieces.png" , 8 , 8 , this->renderer );
    this->initBoard();
    this->possibleMoves = this->getPossibleMovesWithKingCheck( this->board );

    this->parentWindow->listenToMouseLeftPressEvent( (void*)this , (voidfuncptr_t)&(this->leftMouseButtonPressed) );
    this->parentWindow->listenToMouseRightPressEvent( (void*)this , (voidfuncptr_t)&(this->rightMouseButtonPressed) );

    this->currentPlayer = 0;
    this->axisSwaped = false;
    return;
};

App3D::~App3D(){
    for ( int i = 0; i < 8*8*8; i++ ){
        delete ( this->possibleMoves[i] );
    };
    free(this->possibleMoves);
    return;
};

void App3D::Render(){
    this->printBoard( this->board );
    return;
};

void App3D::Update(){
    return;
};

void App3D::initBoard(){
    if ( this->board != nullptr ){
        free( this->board );
    };
    this->board = (piece_t*)calloc( sizeof(piece_t) , 8*8*8 );
    if ( this->board == nullptr ){
        std::cout << "could not calloc board at 'void App3D::initBoard()'" << std::endl;
        return;
    };

    this->board[ 4+ 0*8+ 0*8*8].piece = 4; this->board[ 4+ 0*8+ 0*8*8].team = 1;
    this->board[ 4+ 0*8+ 1*8*8].piece = 3; this->board[ 4+ 0*8+ 1*8*8].team = 1;
    this->board[ 4+ 0*8+ 2*8*8].piece = 2; this->board[ 4+ 0*8+ 2*8*8].team = 1;
    this->board[ 4+ 0*8+ 3*8*8].piece = 5; this->board[ 4+ 0*8+ 3*8*8].team = 1;
    this->board[ 4+ 0*8+ 5*8*8].piece = 2; this->board[ 4+ 0*8+ 5*8*8].team = 1;
    this->board[ 4+ 0*8+ 6*8*8].piece = 3; this->board[ 4+ 0*8+ 6*8*8].team = 1;
    this->board[ 4+ 0*8+ 7*8*8].piece = 4; this->board[ 4+ 0*8+ 7*8*8].team = 1;

    this->board[ 4+ 1*8+ 0*8*8].piece = 1; this->board[ 4+ 1*8+ 0*8*8].team = 1;
    this->board[ 4+ 1*8+ 1*8*8].piece = 1; this->board[ 4+ 1*8+ 1*8*8].team = 1;
    this->board[ 4+ 1*8+ 2*8*8].piece = 1; this->board[ 4+ 1*8+ 2*8*8].team = 1;
    this->board[ 4+ 1*8+ 3*8*8].piece = 1; this->board[ 4+ 1*8+ 3*8*8].team = 1;
    this->board[ 4+ 1*8+ 5*8*8].piece = 1; this->board[ 4+ 1*8+ 5*8*8].team = 1;
    this->board[ 4+ 1*8+ 6*8*8].piece = 1; this->board[ 4+ 1*8+ 6*8*8].team = 1;
    this->board[ 4+ 1*8+ 7*8*8].piece = 1; this->board[ 4+ 1*8+ 7*8*8].team = 1;


    this->board[ 4+ 7*8+ 0*8*8].piece = 4;
    this->board[ 4+ 7*8+ 1*8*8].piece = 3;
    this->board[ 4+ 7*8+ 2*8*8].piece = 2;
    this->board[ 4+ 7*8+ 3*8*8].piece = 5;
    this->board[ 4+ 7*8+ 5*8*8].piece = 2;
    this->board[ 4+ 7*8+ 6*8*8].piece = 3;
    this->board[ 4+ 7*8+ 7*8*8].piece = 4;

    this->board[ 4+ 6*8+ 0*8*8].piece = 1;
    this->board[ 4+ 6*8+ 1*8*8].piece = 1;
    this->board[ 4+ 6*8+ 2*8*8].piece = 1;
    this->board[ 4+ 6*8+ 3*8*8].piece = 1;
    this->board[ 4+ 6*8+ 5*8*8].piece = 1;
    this->board[ 4+ 6*8+ 6*8*8].piece = 1;
    this->board[ 4+ 6*8+ 7*8*8].piece = 1;

    
    this->board[ 0+ 0*8+ 4*8*8].piece = 4; this->board[ 0+ 0*8+ 4*8*8].team = 1;
    this->board[ 1+ 0*8+ 4*8*8].piece = 3; this->board[ 1+ 0*8+ 4*8*8].team = 1;
    this->board[ 2+ 0*8+ 4*8*8].piece = 2; this->board[ 2+ 0*8+ 4*8*8].team = 1;
    this->board[ 3+ 0*8+ 4*8*8].piece = 5; this->board[ 3+ 0*8+ 4*8*8].team = 1;
    this->board[ 4+ 0*8+ 4*8*8].piece = 6; this->board[ 4+ 0*8+ 4*8*8].team = 1;
    this->board[ 5+ 0*8+ 4*8*8].piece = 2; this->board[ 5+ 0*8+ 4*8*8].team = 1;
    this->board[ 6+ 0*8+ 4*8*8].piece = 3; this->board[ 6+ 0*8+ 4*8*8].team = 1;
    this->board[ 7+ 0*8+ 4*8*8].piece = 4; this->board[ 7+ 0*8+ 4*8*8].team = 1;
    
    this->board[ 0+ 1*8+ 4*8*8].piece = 1; this->board[ 0+ 1*8+ 4*8*8].team = 1;
    this->board[ 1+ 1*8+ 4*8*8].piece = 1; this->board[ 1+ 1*8+ 4*8*8].team = 1;
    this->board[ 2+ 1*8+ 4*8*8].piece = 1; this->board[ 2+ 1*8+ 4*8*8].team = 1;
    this->board[ 3+ 1*8+ 4*8*8].piece = 1; this->board[ 3+ 1*8+ 4*8*8].team = 1;
    this->board[ 4+ 1*8+ 4*8*8].piece = 1; this->board[ 4+ 1*8+ 4*8*8].team = 1;
    this->board[ 5+ 1*8+ 4*8*8].piece = 1; this->board[ 5+ 1*8+ 4*8*8].team = 1;
    this->board[ 6+ 1*8+ 4*8*8].piece = 1; this->board[ 6+ 1*8+ 4*8*8].team = 1;
    this->board[ 7+ 1*8+ 4*8*8].piece = 1; this->board[ 7+ 1*8+ 4*8*8].team = 1;

    
    this->board[ 0+ 6*8+ 4*8*8].piece = 1;
    this->board[ 1+ 6*8+ 4*8*8].piece = 1;
    this->board[ 2+ 6*8+ 4*8*8].piece = 1;
    this->board[ 3+ 6*8+ 4*8*8].piece = 1;
    this->board[ 4+ 6*8+ 4*8*8].piece = 1;
    this->board[ 5+ 6*8+ 4*8*8].piece = 1;
    this->board[ 6+ 6*8+ 4*8*8].piece = 1;
    this->board[ 7+ 6*8+ 4*8*8].piece = 1;

    this->board[ 0+ 7*8+ 4*8*8].piece = 4;
    this->board[ 1+ 7*8+ 4*8*8].piece = 3;
    this->board[ 2+ 7*8+ 4*8*8].piece = 2;
    this->board[ 3+ 7*8+ 4*8*8].piece = 5;
    this->board[ 4+ 7*8+ 4*8*8].piece = 6;
    this->board[ 5+ 7*8+ 4*8*8].piece = 2;
    this->board[ 6+ 7*8+ 4*8*8].piece = 3;
    this->board[ 7+ 7*8+ 4*8*8].piece = 4;
    return;
};

void App3D::printBoard( piece_t* board ){
    SDL_Rect rect;
    this->copyDrawOrigin( &rect , this->parentWindow );
    // drawing board pattern
    for ( int z = 0; z < 8; z++ ){
        for( int y = 0; y < 8; y++ ){
            for( int x = 0; x < 8; x++ ){
                if ( (x+y+z)%2 == 0 ){
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
        rect.x += rect.w*9;
    };
    rect.x -= rect.w*(8*9);

    // drawing mouseHoveredPiece's possible moves
    if (
        this->parentWindow->mouseX >= rect.x &&
        this->parentWindow->mouseX < rect.x+rect.w*(8*8+7) &&
        this->parentWindow->mouseY >= rect.y &&
        this->parentWindow->mouseY < rect.y+rect.h*8
    ){
        int mouseHoveredPieceX = floor( ( ( this->parentWindow->mouseX - rect.x )/rect.w )%9 );
        int mouseHoveredPieceY = floor( ( this->parentWindow->mouseY - rect.y )/rect.w );
        int mouseHoveredPieceZ = floor( ( ( this->parentWindow->mouseX - rect.x )/(9*rect.w) )%64 );
        if ( this->axisSwaped == true ){
            int tmp = mouseHoveredPieceX;
            mouseHoveredPieceX = mouseHoveredPieceZ;
            mouseHoveredPieceZ = tmp;
        };
        if (
            mouseHoveredPieceX >= 0 &&
            mouseHoveredPieceX < 8 &&
            mouseHoveredPieceY >= 0 &&
            mouseHoveredPieceY < 8 &&
            mouseHoveredPieceZ >= 0 &&
            mouseHoveredPieceZ < 8
        ){
            int mouseHoveredPiece = mouseHoveredPieceX + 8*mouseHoveredPieceY + 8*8*mouseHoveredPieceZ;
            // hovered piece
            if ( ( mouseHoveredPieceX+mouseHoveredPieceY+mouseHoveredPieceZ )%2 == 0 ){
                SDL_SetRenderDrawColor( this->renderer , 99 , 199 , 74 , 255 );
            }else{
                SDL_SetRenderDrawColor( this->renderer , 62 , 137 , 72 , 255 );
            };
            SDL_Rect dstRect;
            dstRect.x = rect.x + rect.w*mouseHoveredPieceX + rect.w*9*mouseHoveredPieceZ;
            if ( this->axisSwaped == true ){
                dstRect.x = rect.x + rect.w*9*mouseHoveredPieceX + rect.w*mouseHoveredPieceZ;
            };
            dstRect.y = rect.y + rect.h*mouseHoveredPieceY;
            dstRect.w = rect.w;
            dstRect.h = rect.h;
            SDL_RenderFillRect( this->renderer, &dstRect );
            
            // possible moves
            LinkedListNode<int>* current = this->possibleMoves[mouseHoveredPiece];
            while( current != nullptr ){
                if ( ((int)((current->getValue()%8)+floor((current->getValue()%(8*8))/8)+floor(current->getValue()/(8*8))))%2 == 0 ){
                    SDL_SetRenderDrawColor( this->renderer , 99 , 199 , 74 , 255 );
                }else{
                    SDL_SetRenderDrawColor( this->renderer , 62 , 137 , 72 , 255 );
                };
                dstRect.x = rect.x + rect.w*(current->getValue()%8) + rect.w*9*floor(current->getValue()/(8*8));
                if ( this->axisSwaped == true ){
                    dstRect.x = rect.x + rect.w*9*(current->getValue()%8) + rect.w*floor(current->getValue()/(8*8));
                };
                dstRect.y = rect.y + rect.h*floor((current->getValue()%(8*8))/8);
                SDL_RenderFillRect( this->renderer, &dstRect );
                current = current->getNext();
            };
            
        };
    };

    
    // drawing selected piece and selected piece possible moves
    if (
        this->selectedPiece >= 0 &&
        this->selectedPiece < 8*8*8
    ){
        // selected piece
        if ( ( (int)( (this->selectedPiece%8)+floor((this->selectedPiece%(8*8))/8)+floor(this->selectedPiece/(8*8)) ) )%2 == 0 ){
            SDL_SetRenderDrawColor( this->renderer , 254 , 174 , 52 , 255 );
        }else{
            SDL_SetRenderDrawColor( this->renderer , 247 , 118 , 34 , 255 );
        };
        SDL_Rect dstRect;
        dstRect.x = rect.x + rect.w*(this->selectedPiece%8) + rect.w*9*floor(this->selectedPiece/(8*8));
        if ( this->axisSwaped == true ){
            dstRect.x = rect.x + rect.w*9*(this->selectedPiece%8) + rect.w*floor(this->selectedPiece/(8*8));
        };
        dstRect.y = rect.y + rect.w*floor((this->selectedPiece%(8*8))/8);
        dstRect.w = rect.w;
        dstRect.h = rect.h;
        SDL_RenderFillRect( this->renderer, &dstRect );
        
        // possible moves
        LinkedListNode<int>* current = this->possibleMoves[this->selectedPiece];
        while( current != nullptr ){
            if ( ((int)((current->getValue()%8)+floor((current->getValue()%(8*8))/8)+floor(current->getValue()/(8*8))))%2 == 0 ){
                SDL_SetRenderDrawColor( this->renderer , 254 , 174 , 52 , 255 );
            }else{
                SDL_SetRenderDrawColor( this->renderer , 247 , 118 , 34 , 255 );
            };
            dstRect.x = rect.x + rect.w*(current->getValue()%8) + rect.w*9*floor(current->getValue()/(8*8));
            if ( this->axisSwaped == true ){
                dstRect.x = rect.x + rect.w*9*(current->getValue()%8) + rect.w*floor(current->getValue()/(8*8));
            };
            dstRect.y = rect.y + rect.h*floor((current->getValue()%(8*8))/8);
            SDL_RenderFillRect( this->renderer, &dstRect );
            current = current->getNext();
        };
    };

    // drawing pieces
    for( int z = 0; z < 8; z++ ){
        for( int y = 0; y < 8; y++ ){
            for( int x = 0; x < 8; x++ ){
                SDL_Rect srcRect;
                int cord = x+y*8+z*8*8;
                if ( this->axisSwaped == true ){
                    cord = z+y*8+x*8*8;
                };
                if ( board[cord].piece > 0 ){
                    this->piecesSprite->copyFrame( &srcRect, board[cord].piece-1+board[cord].team*6 );
                    SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &rect );
                };

                rect.x += rect.w;
            };
            rect.x -= rect.w*8;
            rect.y += rect.h;
        };
        rect.y -= rect.h*8;
        rect.x += rect.w*9;
    };
    rect.x -= rect.w*(8*9);

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
