#ifndef App2D_HPP
#define App2D_HPP
#include <cmath>
#include "Sprite.hpp"
class App2D;
#include "Window.hpp"

typedef struct {
    unsigned int team : 1;
    unsigned int piece : 3;
    unsigned int moved : 1;
    unsigned int enpas : 1;
} piece_t;

// number = piece
// 0 = no piece
// 1 = pawn
// 2 = bishop
// 3 = knight
// 4 = rook
// 5 = queen
// 6 = king

class App2D {
    public:
        App2D( Window* parentWindow , SDL_Renderer* renderer );
        ~App2D();
        void Render();
        void Update();
    private:
        Window* parentWindow;
        SDL_Renderer* renderer;
        Sprite* piecesSprite = nullptr;
        
        piece_t* board = nullptr;
        int currentPlayer = 0;
        int selectedPiece = -1;
        LinkedListNode<int>** possibleMoves = nullptr;

        void initBoard();
        void printBoard( piece_t* board );
        static LinkedListNode<int>** getPossibleMoves( piece_t* board ){
            LinkedListNode<int>** res = (LinkedListNode<int>**)calloc( sizeof( LinkedListNode<int>* ) , 8*8 );
            if ( res == nullptr ){
                std::cout << "calloc failed at App2D::getPossibleMoves" << std::endl;
                return nullptr;
            };
            for ( int i = 0; i < 8*8; i++ ){
                res[i] = nullptr;
                int ptd = board[i].team*2-1; // ptd = pieceTeamDirection
                switch ( board[i].piece ){
                    case 0: // no piece
                        break;
                    case 1: // pawn
                        // single forward
                        if (
                            i+8*ptd >= 0 && i+8*ptd < 8*8 &&
                            board[i+8*ptd].piece == 0
                        ){
                            LinkedListNode<int>::appendNode(
                                &(res[i]),
                                new LinkedListNode<int>( i+8*ptd )
                            );
                            // double forward
                            if (
                                floor(i/8) == 3.5-2.5*ptd &&
                                board[i+2*8*ptd].piece == 0
                            ){
                                LinkedListNode<int>::appendNode(
                                    &(res[i]),
                                    new LinkedListNode<int>( i+2*8*ptd )
                                );
                            };
                        };
                        // take right (and enpas)
                        if (
                            i%8 < 7 &&
                            i+8*ptd+1 >= 0 && i+8*ptd+1 < 8*8 &&
                            (
                                (
                                    board[i+8*ptd+1].piece != 0 &&
                                    board[i+8*ptd+1].team != board[i].team
                                ) ||
                                (
                                    board[i+8*ptd+1].piece == 0 &&
                                    board[i+1].enpas == 1 &&
                                    board[i+1].piece == 1 &&
                                    board[i+1].team != board[i].team
                                )
                            )
                        ){
                            LinkedListNode<int>::appendNode(
                                &(res[i]),
                                new LinkedListNode<int>( i+8*ptd+1 )
                            );
                        };
                        // take left (with enpas)
                        if (
                            i%8 > 0 &&
                            i+8*ptd-1 >= 0 && i+8*ptd-1 < 8*8 &&
                            (
                                (
                                    board[i+8*ptd-1].piece != 0 &&
                                    board[i+8*ptd-1].team != board[i].team
                                ) ||
                                (
                                    board[i+8*ptd-1].piece == 0 &&
                                    board[i-1].enpas == 1 &&
                                    board[i-1].piece == 1 &&
                                    board[i-1].team != board[i].team
                                )
                            )
                        ){
                            LinkedListNode<int>::appendNode(
                                &(res[i]),
                                new LinkedListNode<int>( i+8*ptd-1 )
                            );
                        };
                        break;
                    case 2: // bishop
                        for ( int dx = -1; dx <= 1; dx += 2 ){
                            for ( int dy = -1; dy <= 1; dy += 2 ){
                                for ( int x = i%8+dx , y = floor(i/8)+dy; ( x >= 0 && x < 8 && y >= 0 && y < 8 ); x+=dx, y+=dy ){
                                    if ( board[x+8*y].piece == 0 ){
                                        LinkedListNode<int>::appendNode(
                                            &(res[i]),
                                            new LinkedListNode<int>( x+y*8 )
                                        );
                                    }else{
                                        if ( board[x+8*y].team != board[i].team ){
                                            LinkedListNode<int>::appendNode(
                                                &(res[i]),
                                                new LinkedListNode<int>( x+8*y )
                                            );
                                        };
                                        // exists x,y for loop
                                        x += 8*dx; y += 8*dy;
                                    };
                                };
                            };
                        };
                        break;
                    case 3: // knight
                        for ( int dx = -2; dx <= 2; dx++ ){
                            for ( int dy = -2; dy <= 2; dy++ ){
                                int x = i%8+dx , y = floor(i/8)+dy;
                                if (
                                    x >= 0 && x < 8 && y >= 0 && y < 8 &&
                                    dx != 0 && dy != 0 &&
                                    abs(dx) != abs(dy)
                                ){
                                    if (
                                        board[x+8*y].piece == 0 ||
                                        board[x+8*y].team != board[i].team
                                    ){
                                        LinkedListNode<int>::appendNode(
                                            &(res[i]),
                                            new LinkedListNode<int>( x+8*y )
                                        );
                                    };
                                };
                            };
                        };
                        break;
                    case 4: // rook
                        for ( int dx = -1; dx <= 1; dx++ ){
                            for ( int dy = -1; dy <= 1; dy++ ){
                                if ( abs(dx) != abs(dy) ){
                                    for ( int x = i%8+dx , y = floor(i/8)+dy; ( x >= 0 && x < 8 && y >= 0 && y < 8 ); x+=dx, y+=dy ){
                                        if ( board[x+8*y].piece == 0 ){
                                            LinkedListNode<int>::appendNode(
                                                &(res[i]),
                                                new LinkedListNode<int>( x+8*y )
                                            );
                                        }else{
                                            if ( board[x+8*y].team != board[i].team ){
                                                LinkedListNode<int>::appendNode(
                                                    &(res[i]),
                                                    new LinkedListNode<int>( x+8*y )
                                                );
                                            };
                                            // exists x,y for loop
                                            x += 8*dx; y += 8*dy;
                                        };
                                    };
                                };
                            };
                        };
                        break;
                    case 5: // queen
                        for ( int dx = -1; dx <= 1; dx++ ){
                            for ( int dy = -1; dy <= 1; dy++ ){
                                if ( dx != 0 || dy != 0 ){
                                    for ( int x = i%8+dx , y = floor(i/8)+dy; ( x >= 0 && x < 8 && y >= 0 && y < 8 ); x+=dx, y+=dy ){
                                        if ( board[x+8*y].piece == 0 ){
                                            LinkedListNode<int>::appendNode(
                                                &(res[i]),
                                                new LinkedListNode<int>( x+8*y )
                                            );
                                        }else{
                                            if ( board[x+8*y].team != board[i].team ){
                                                LinkedListNode<int>::appendNode(
                                                    &(res[i]),
                                                    new LinkedListNode<int>( x+8*y )
                                                );
                                            };
                                            // exists x,y for loop
                                            x += 8*dx; y += 8*dy;
                                        };
                                    };
                                };
                            };
                        };
                        break;
                    case 6: // king
                        // normal 8 way
                        for ( int dx = -1; dx <= 1; dx++ ){
                            for ( int dy = -1; dy <= 1; dy++ ){
                                int x = i%8+dx , y = floor(i/8)+dy;
                                if (
                                    x >= 0 && x < 8 && y >= 0 && y < 8 &&
                                    ( dx != 0 || dy != 0 )
                                ){
                                    if (
                                        board[x+8*y].piece == 0 ||
                                        board[x+8*y].team != board[i].team
                                    ){
                                        LinkedListNode<int>::appendNode(
                                            &(res[i]),
                                            new LinkedListNode<int>( x+8*y )
                                        );
                                    };
                                };
                            };
                        };
                        // castle short
                        if (
                            board[i].moved == 0 &&
                            board[i+3].piece == 4 && board[i+3].team == board[i].team && board[i+3].moved == 0 &&
                            board[i+1].piece == 0 && board[i+2].piece == 0
                        ){
                            LinkedListNode<int>::appendNode(
                                &(res[i]),
                                new LinkedListNode<int>( i+2 )
                            );
                        };
                        // castle long
                        if (
                            board[i].moved == 0 &&
                            board[i-4].piece == 4 && board[i-4].team == board[i].team && board[i-4].moved == 0 &&
                            board[i-1].piece == 0 && board[i-2].piece == 0 && board[i-3].piece == 0
                        ){
                            LinkedListNode<int>::appendNode(
                                &(res[i]),
                                new LinkedListNode<int>( i-2 )
                            );
                        };
                        break;
                };
            };
            return res;
        };
        static LinkedListNode<int>** getPossibleMovesWithKingCheck( piece_t* board ){
            LinkedListNode<int>** res = (LinkedListNode<int>**)calloc( sizeof( LinkedListNode<int>* ) , 8*8 );
            if ( res == nullptr ){
                std::cout << "calloc failed at App2D::getPossibleMoves" << std::endl;
                return nullptr;
            };
            for ( int i = 0; i < 8*8; i++ ){
                res[i] = nullptr;
                int ptd = board[i].team*2-1; // ptd = pieceTeamDirection
                switch ( board[i].piece ){
                    case 0: // no piece
                        break;
                    case 1: // pawn
                        // single forward
                        if (
                            i+8*ptd >= 0 && i+8*ptd < 8*8 &&
                            board[i+8*ptd].piece == 0
                        ){
                            App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+8*ptd );
                            // double forward
                            if (
                                floor(i/8) == 3.5-2.5*ptd &&
                                board[i+2*8*ptd].piece == 0
                            ){
                                App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+2*8*ptd );
                            };
                        };
                        // take right (and enpas)
                        if (
                            i%8 < 7 &&
                            i+8*ptd+1 >= 0 && i+8*ptd+1 < 8*8 &&
                            (
                                (
                                    board[i+8*ptd+1].piece != 0 &&
                                    board[i+8*ptd+1].team != board[i].team
                                ) ||
                                (
                                    board[i+8*ptd+1].piece == 0 &&
                                    board[i+1].enpas == 1 &&
                                    board[i+1].piece == 1 &&
                                    board[i+1].team != board[i].team
                                )
                            )
                        ){
                            App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+8*ptd+1 );
                        };
                        // take left (with enpas)
                        if (
                            i%8 > 0 &&
                            i+8*ptd-1 >= 0 && i+8*ptd-1 < 8*8 &&
                            (
                                (
                                    board[i+8*ptd-1].piece != 0 &&
                                    board[i+8*ptd-1].team != board[i].team
                                ) ||
                                (
                                    board[i+8*ptd-1].piece == 0 &&
                                    board[i-1].enpas == 1 &&
                                    board[i-1].piece == 1 &&
                                    board[i-1].team != board[i].team
                                )
                            )
                        ){
                            App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+8*ptd-1 );
                        };
                        break;
                    case 2: // bishop
                        for ( int dx = -1; dx <= 1; dx += 2 ){
                            for ( int dy = -1; dy <= 1; dy += 2 ){
                                for ( int x = i%8+dx , y = floor(i/8)+dy; ( x >= 0 && x < 8 && y >= 0 && y < 8 ); x+=dx, y+=dy ){
                                    if ( board[x+8*y].piece == 0 ){
                                        App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8 );
                                    }else{
                                        if ( board[x+8*y].team != board[i].team ){
                                            App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8 );
                                        };
                                        // exists x,y for loop
                                        x += 8*dx; y += 8*dy;
                                    };
                                };
                            };
                        };
                        break;
                    case 3: // knight
                        for ( int dx = -2; dx <= 2; dx++ ){
                            for ( int dy = -2; dy <= 2; dy++ ){
                                int x = i%8+dx , y = floor(i/8)+dy;
                                if (
                                    x >= 0 && x < 8 && y >= 0 && y < 8 &&
                                    dx != 0 && dy != 0 &&
                                    abs(dx) != abs(dy)
                                ){
                                    if (
                                        board[x+8*y].piece == 0 ||
                                        board[x+8*y].team != board[i].team
                                    ){
                                        App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8 );
                                    };
                                };
                            };
                        };
                        break;
                    case 4: // rook
                        for ( int dx = -1; dx <= 1; dx++ ){
                            for ( int dy = -1; dy <= 1; dy++ ){
                                if ( abs(dx) != abs(dy) ){
                                    for ( int x = i%8+dx , y = floor(i/8)+dy; ( x >= 0 && x < 8 && y >= 0 && y < 8 ); x+=dx, y+=dy ){
                                        if ( board[x+8*y].piece == 0 ){
                                            App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8 );
                                        }else{
                                            if ( board[x+8*y].team != board[i].team ){
                                                App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8 );
                                            };
                                            // exists x,y for loop
                                            x += 8*dx; y += 8*dy;
                                        };
                                    };
                                };
                            };
                        };
                        break;
                    case 5: // queen
                        for ( int dx = -1; dx <= 1; dx++ ){
                            for ( int dy = -1; dy <= 1; dy++ ){
                                if ( dx != 0 || dy != 0 ){
                                    for ( int x = i%8+dx , y = floor(i/8)+dy; ( x >= 0 && x < 8 && y >= 0 && y < 8 ); x+=dx, y+=dy ){
                                        if ( board[x+8*y].piece == 0 ){
                                            App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8 );
                                        }else{
                                            if ( board[x+8*y].team != board[i].team ){
                                                App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8 );
                                            };
                                            // exists x,y for loop
                                            x += 8*dx; y += 8*dy;
                                        };
                                    };
                                };
                            };
                        };
                        break;
                    case 6: // king
                        // normal 8 way
                        for ( int dx = -1; dx <= 1; dx++ ){
                            for ( int dy = -1; dy <= 1; dy++ ){
                                int x = i%8+dx , y = floor(i/8)+dy;
                                if (
                                    x >= 0 && x < 8 && y >= 0 && y < 8 &&
                                    ( dx != 0 || dy != 0 )
                                ){
                                    if (
                                        board[x+8*y].piece == 0 ||
                                        board[x+8*y].team != board[i].team
                                    ){
                                        App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8 );
                                    };
                                };
                            };
                        };
                        // castle short
                        if (
                            board[i].moved == 0 &&
                            board[i+3].piece == 4 && board[i+3].team == board[i].team && board[i+3].moved == 0 &&
                            board[i+1].piece == 0 && board[i+2].piece == 0
                        ){
                            App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+2 );
                        };
                        // castle long
                        if (
                            board[i].moved == 0 &&
                            board[i-4].piece == 4 && board[i-4].team == board[i].team && board[i-4].moved == 0 &&
                            board[i-1].piece == 0 && board[i-2].piece == 0 && board[i-3].piece == 0
                        ){
                            App2D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i-2 );
                        };
                        break;
                };
            };
            return res;
        };
        static void appendPossibleMoveAndCheckKing( piece_t* board , LinkedListNode<int>** fromPieceLL , int from , int to ){
            piece_t* boardPostMove = App2D::playPossibleMove( board , from , to );
            if ( App2D::isKingThreteaned( boardPostMove , board[from].team ) == false ){
                LinkedListNode<int>::appendNode(
                    fromPieceLL,
                    new LinkedListNode<int>( to )
                );
            };
            free( boardPostMove );
            return;
        };
        static void playPossibleMoveOnBoard( piece_t* board , int from , int to ){
            // remove enpas data
            for ( int i = 0; i < 8*8; i++ ){ board[i].enpas = 0; };

            // pawn special rules
            if ( board[from].piece == 1 ){
                // enpas
                if ( from%8 != to%8 && board[to].piece == 0 ){
                    board[from-(from%8)+to%8].piece = 0; board[from-(from%8)+to%8].team = 0; board[from-(from%8)+to%8].enpas = 0; board[from-(from%8)+to%8].moved = 0;
                }else
                // double forward
                if ( abs( (int)( floor(from/8) - floor(to/8) ) ) == 2 ){
                    board[from].enpas = 1;
                };
            };

            board[to] = board[from];
            board[from].piece = 0; board[from].team = 0; board[from].enpas = 0; board[from].moved = 0;
            board[to].moved = 1;
            
            // castle long
            if ( board[to].piece == 6 && from%8 - to%8 == 2 ){
                board[to+1] = board[to-2];
                board[to-2].piece = 0; board[to-2].team = 0; board[to-2].enpas = 0; board[to-2].moved = 0;
            }else
            // castle short
            if( board[to].piece == 6 && from%8 - to%8 == -2 ){
                board[to-1] = board[to+1];
                board[to+1].piece = 0; board[to+1].team = 0; board[to+1].enpas = 0; board[to+1].moved = 0;
            };
            return;
        };
        static piece_t* playPossibleMove( piece_t* board , int from , int to ){
            piece_t* newBoard = (piece_t*)calloc( sizeof(piece_t) , 8*8 );
            if ( newBoard == nullptr ){
                std::cout << "calloc failed at App2D::playPossibleMove" << std::endl;
                return newBoard;
            };
            for ( int i = 0; i < 8*8; i++ ){
                newBoard[i] = board[i];
            };
            playPossibleMoveOnBoard( newBoard , from , to );
            return newBoard;
        };
        static bool isPieceThretened( piece_t* board , int piece ){
            LinkedListNode<int>** possibleMoves = App2D::getPossibleMoves( board );
            for ( int i = 0; i < 8*8; i++ ){
                if ( i != piece && board[i].piece != 0 && board[i].team != board[piece].team ){
                    LinkedListNode<int>* current = possibleMoves[i];
                    while( current != nullptr ){
                        for ( int i = 0; i < 8*8; i++ ){
                            delete ( possibleMoves[i] );
                        };
                        free(possibleMoves);
                        current = current->getNext();
                    };
                };
            };
            for ( int i = 0; i < 8*8*8; i++ ){
                delete ( possibleMoves[i] );
            };
            free(possibleMoves);
            return false;
        };
        static bool isKingThreteaned( piece_t* board , int team ){
            for ( int i = 0; i < 8*8; i++ ){
                if ( board[i].piece == 6 && board[i].team == team ){
                    return App2D::isPieceThretened( board , i );
                };
            };
            std::cout << "no king was found at App2D::isKingThreteaned(" << board << "," << team << ")\n";
            for ( int y = 0; y < 8; y++ ){
                for ( int x = 0; x < 8; x++ ){
                    std::cout << board[x+y*8].piece <<".";
                };
                std::cout << "\n";
            };
            return false;
        };
        static void leftMouseButtonPressed( void* This ){
            int x = ( ((App2D*)This)->parentWindow->width - std::min( (int)(((App2D*)This)->parentWindow->width/8) , (int)(((App2D*)This)->parentWindow->height/8) )*8 )/2;
            int y = ( ((App2D*)This)->parentWindow->height - std::min( (int)(((App2D*)This)->parentWindow->width/8) , (int)(((App2D*)This)->parentWindow->height/8) )*8 )/2;
            int size = std::min( (int)(((App2D*)This)->parentWindow->width/8) , (int)(((App2D*)This)->parentWindow->height/8) );

            if (
                ((App2D*)This)->parentWindow->mouseX < x ||
                ((App2D*)This)->parentWindow->mouseX >= x+size*8 ||
                ((App2D*)This)->parentWindow->mouseY < y ||
                ((App2D*)This)->parentWindow->mouseY >= y+size*8
            ){
                ((App2D*)This)->selectedPiece = -1;
                return;
            };
            int newSelectedPiece = floor( ( ((App2D*)This)->parentWindow->mouseX - x )/size ) + 8*floor( ( ((App2D*)This)->parentWindow->mouseY - y )/size );
            if ( ((App2D*)This)->selectedPiece >= 0 && ((App2D*)This)->selectedPiece < 8*8 ){
                LinkedListNode<int>* current = ((App2D*)This)->possibleMoves[((App2D*)This)->selectedPiece];
                while( current != nullptr ){
                    if ( current->getValue() == newSelectedPiece ){
                        if ( ((App2D*)This)->board[((App2D*)This)->selectedPiece].team != ((App2D*)This)->currentPlayer ){
                            // wrong team - cant play move
                            ((App2D*)This)->selectedPiece = newSelectedPiece;
                            return;
                        };
                        playPossibleMoveOnBoard( ((App2D*)This)->board , ((App2D*)This)->selectedPiece , newSelectedPiece );
                        // freeing ((App2D*)This)->possibleMoves
                        for ( int i = 0; i < 8*8; i++ ){
                            delete ( ((App2D*)This)->possibleMoves[i] );
                        };
                        free( ((App2D*)This)->possibleMoves );

                        ((App2D*)This)->possibleMoves = getPossibleMovesWithKingCheck( ((App2D*)This)->board );
                        ((App2D*)This)->selectedPiece = -1;

                        ((App2D*)This)->currentPlayer = 1-((App2D*)This)->currentPlayer;
                        return;
                    };
                    
                    current = current->getNext();
                };
                ((App2D*)This)->selectedPiece = newSelectedPiece;
                return;
            };
            ((App2D*)This)->selectedPiece = newSelectedPiece;
            return;
        };
        
};

#include "App2D.cpp"
#endif