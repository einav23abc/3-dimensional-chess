
App3D::App3D( Window* parentWindow , SDL_Renderer* renderer ){
    drawTextInit( renderer );

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

    this->updatePieceAmounts();
    return;
};

void App3D::printBoard( piece_t* board ){

    SDL_Rect rect;
    SDL_Rect srcRect;
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


    // board GUI

    // captured pieces
    int advantage = (player0queens*10+player0rooks*5+player0knights*3+player0bishops*3+player0pawns)-(player1queens*10+player1rooks*5+player1knights*3+player1bishops*3+player1pawns);
    // captured white pieces by black
    rect.y -= rect.h*2.125;
    rect.x += rect.w*(8*8+7-15.25) + rect.w*0.125;
    srcRect.x = rect.x-rect.w*0.125;
    srcRect.y = rect.y-rect.h*0.125;
    srcRect.w = rect.w*15.25;
    srcRect.h = rect.h*1.25;
    SDL_SetRenderDrawColor( this->renderer , 38 , 43 , 68 , 255 );
    SDL_RenderFillRect( this->renderer, &srcRect );
    srcRect.x = rect.x;
    srcRect.y = rect.y;
    srcRect.w = rect.w*15;
    srcRect.h = rect.h*1;
    SDL_SetRenderDrawColor( this->renderer , 90 , 105 , 136 , 255 );
    SDL_RenderFillRect( this->renderer, &srcRect );
    rect.x += rect.w*14;
    for ( int i = this->player0queens; i < 2; i++ ){
        this->piecesSprite->copyFrame( &srcRect, 4 );
        SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &rect );
        rect.x -= rect.w;
    };
    for ( int i = this->player0rooks; i < 4; i++ ){
        this->piecesSprite->copyFrame( &srcRect, 3 );
        SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &rect );
        rect.x -= rect.w;
    };
    for ( int i = this->player0knights; i < 4; i++ ){
        this->piecesSprite->copyFrame( &srcRect, 2 );
        SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &rect );
        rect.x -= rect.w;
    };
    for ( int i = this->player0bishops; i < 4; i++ ){
        this->piecesSprite->copyFrame( &srcRect, 1 );
        SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &rect );
        rect.x -= rect.w;
    };
    for ( int i = this->player0pawns; i < 15; i++ ){
        this->piecesSprite->copyFrame( &srcRect, 0 );
        SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &rect );
        rect.x -= rect.w;
    };
    if ( advantage < 0 ){
        char text[4];
        text[0] = '+';
        if ( advantage%10 == advantage ){
            text[1] = '0'-advantage;
            text[2] = '\0';
        }else{
            text[1] = '0'-floor(advantage/10);
            text[2] = '0'-(advantage%10);
            text[3] = '\0';
        };
        drawText( text , this->renderer , rect.x+rect.w , rect.y , rect.w/8 , true , 10 );
    };
    // captured black pieces by white
    this->copyDrawOrigin( &rect , this->parentWindow );
    rect.y += rect.h*9.125;
    rect.x += rect.w*(8*8+7-15.25) + rect.w*0.125;
    srcRect.x = rect.x-rect.w*0.125;
    srcRect.y = rect.y-rect.h*0.125;
    srcRect.w = rect.w*15.25;
    srcRect.h = rect.h*1.25;
    SDL_SetRenderDrawColor( this->renderer , 38 , 43 , 68 , 255 );
    SDL_RenderFillRect( this->renderer, &srcRect );
    srcRect.x = rect.x;
    srcRect.y = rect.y;
    srcRect.w = rect.w*15;
    srcRect.h = rect.h*1;
    SDL_SetRenderDrawColor( this->renderer , 90 , 105 , 136 , 255 );
    SDL_RenderFillRect( this->renderer, &srcRect );
    rect.x += rect.w*14;
    for ( int i = this->player1queens; i < 2; i++ ){
        this->piecesSprite->copyFrame( &srcRect, 10 );
        SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &rect );
        rect.x -= rect.w;
    };
    for ( int i = this->player1rooks; i < 4; i++ ){
        this->piecesSprite->copyFrame( &srcRect, 9 );
        SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &rect );
        rect.x -= rect.w;
    };
    for ( int i = this->player1knights; i < 4; i++ ){
        this->piecesSprite->copyFrame( &srcRect, 8 );
        SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &rect );
        rect.x -= rect.w;
    };
    for ( int i = this->player1bishops; i < 4; i++ ){
        this->piecesSprite->copyFrame( &srcRect, 7 );
        SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &rect );
        rect.x -= rect.w;
    };
    for ( int i = this->player1pawns; i < 15; i++ ){
        this->piecesSprite->copyFrame( &srcRect, 6 );
        SDL_RenderCopy( this->renderer , this->piecesSprite->getTexture() , &srcRect , &rect );
        rect.x -= rect.w;
    };
    if ( advantage > 0 ){
        char text[4];
        text[0] = '+';
        if ( advantage%10 == advantage ){
            text[1] = '0'+advantage;
            text[2] = '\0';
        }else{
            text[1] = '0'+floor(advantage/10);
            text[2] = '0'+(advantage%10);
            text[3] = '\0';
        };
        drawText( text , this->renderer , rect.x+rect.w , rect.y , rect.w/8 , true , 10 );
    };
    return;
};

void App3D::updatePieceAmounts(){
    this->player0pawns = 0;
    this->player0bishops = 0;
    this->player0knights = 0;
    this->player0rooks = 0;
    this->player0queens = 0;
    this->player1pawns = 0;
    this->player1bishops = 0;
    this->player1knights = 0;
    this->player1rooks = 0;
    this->player1queens = 0;
    for ( int i = 0; i < 8*8*8; i++ ){
        switch( board[i].piece ){
            case 0:
                break;
            case 1: // pawn
                if ( this->board[i].team == 1 ){ this->player1pawns++; }else{ this->player0pawns++; };
                break;
            case 2: // bishop
                if ( this->board[i].team == 1 ){ this->player1bishops++; }else{ this->player0bishops++; };
                break;
            case 3: // knight
                if ( this->board[i].team == 1 ){ this->player1knights++; }else{ this->player0knights++; };
                break;
            case 4: // rook
                if ( this->board[i].team == 1 ){ this->player1rooks++; }else{ this->player0rooks++; };
                break;
            case 5: // queen
                if ( this->board[i].team == 1 ){ this->player1queens++; }else{ this->player0queens++; };
                break;
        };
    };
    return;
};