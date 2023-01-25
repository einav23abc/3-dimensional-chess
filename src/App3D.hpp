#ifndef App3D_HPP
#define App3D_HPP
#include <cmath>
#include "drawText.cpp"
#include "Sprite.hpp"
class App3D;
#include "Window.hpp"

/*
typedef struct {
    unsigned int team : 1;
    unsigned int piece : 3;
    unsigned int moved : 1;
    unsigned int enpas : 1;
} piece_t;
*/

// number = piece
// 0 = no piece
// 1 = pawn
// 2 = bishop
// 3 = knight
// 4 = rook
// 5 = queen
// 6 = king


#define APP3D_WINDOW_1X_WIDTH  568 // 8*(8*8+7) = 568
#define APP3D_WINDOW_1X_HEIGHT 64  // 8*8 = 64


class App3D {
    public:
        App3D( Window* parentWindow , SDL_Renderer* renderer );
        ~App3D();
        void Render();
        void Update();
    private:
        Window* parentWindow;
        SDL_Renderer* renderer;
        Sprite* piecesSprite = nullptr;
        
        piece_t* board = nullptr;
        bool axisSwaped = false;
        int currentPlayer = 0;
        int selectedPiece = -1;
        LinkedListNode<int>** possibleMoves = nullptr;

        int player0pawns;
        int player0bishops;
        int player0knights;
        int player0rooks;
        int player0queens;
        int player1pawns;
        int player1bishops;
        int player1knights;
        int player1rooks;
        int player1queens;

        void initBoard();
        void printBoard( piece_t* board );
        void updatePieceAmounts();
        static LinkedListNode<int>** getPossibleMoves( piece_t* board ){
            LinkedListNode<int>** res = (LinkedListNode<int>**)calloc( sizeof( LinkedListNode<int>* ) , 8*8*8 );
            if ( res == nullptr ){
                std::cout << "calloc failed at App3D::getPossibleMoves" << std::endl;
                return nullptr;
            };
            for ( int i = 0; i < 8*8*8; i++ ){
                res[i] = nullptr;
                int ptd = board[i].team*2-1; // ptd = pieceTeamDirection
                switch ( board[i].piece ){
                    case 0: // no piece
                        break;
                    case 1: // pawn
                        // single forward
                        if (
                            i+8*ptd >= 0 && i+8*ptd < 8*8*8 &&
                            board[i+8*ptd].piece == 0
                        ){
                            LinkedListNode<int>::appendNode(
                                &(res[i]),
                                new LinkedListNode<int>( i+8*ptd )
                            );
                            // double forward
                            if (
                                floor((i%(8*8))/8) == 3.5-2.5*ptd &&
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
                            i+8*ptd+1 >= 0 && i+8*ptd+1 < 8*8*8 &&
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
                            i+8*ptd-1 >= 0 && i+8*ptd-1 < 8*8*8 &&
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
                        // take up (with enpas)
                        if (
                            floor(i/(8*8)) < 7 &&
                            i+8*ptd+8*8 >= 0 && i+8*ptd+8*8 < 8*8*8 &&
                            (
                                (
                                    board[i+8*ptd+8*8].piece != 0 &&
                                    board[i+8*ptd+8*8].team != board[i].team
                                ) ||
                                (
                                    board[i+8*ptd+8*8].piece == 0 &&
                                    board[i+8*8].enpas == 1 &&
                                    board[i+8*8].piece == 1 &&
                                    board[i+8*8].team != board[i].team
                                )
                            )
                        ){
                            LinkedListNode<int>::appendNode(
                                &(res[i]),
                                new LinkedListNode<int>( i+8*ptd+8*8 )
                            );
                        };
                        // take down (with enpas)
                        if (
                            floor(i/(8*8)) > 0 &&
                            i+8*ptd-8*8 >= 0 && i+8*ptd-8*8 < 8*8*8 &&
                            (
                                (
                                    board[i+8*ptd-8*8].piece != 0 &&
                                    board[i+8*ptd-8*8].team != board[i].team
                                ) ||
                                (
                                    board[i+8*ptd-8*8].piece == 0 &&
                                    board[i-8*8].enpas == 1 &&
                                    board[i-8*8].piece == 1 &&
                                    board[i-8*8].team != board[i].team
                                )
                            )
                        ){
                            LinkedListNode<int>::appendNode(
                                &(res[i]),
                                new LinkedListNode<int>( i+8*ptd-8*8 )
                            );
                        };
                        break;
                    case 2: // bishop
                        for ( int dz = -1; dz <= 1; dz += 1 ){
                            for ( int dx = -1; dx <= 1; dx += 1 ){
                                for ( int dy = -1; dy <= 1; dy += 1 ){
                                    if ( abs(dx) + abs(dy) + abs(dz) == 2 ){
                                        for ( int x = i%8+dx , y = floor((i%(8*8))/8)+dy , z = floor(i/(8*8))+dz; ( x >= 0 && x < 8 && y >= 0 && y < 8 && z >= 0 && z < 8 ); x+=dx, y+=dy, z+=dz ){
                                            if ( board[x+8*y+z*8*8].piece == 0 ){
                                                LinkedListNode<int>::appendNode(
                                                    &(res[i]),
                                                    new LinkedListNode<int>( x+y*8+z*8*8 )
                                                );
                                            }else{
                                                if ( board[x+8*y+z*8*8].team != board[i].team ){
                                                    LinkedListNode<int>::appendNode(
                                                        &(res[i]),
                                                        new LinkedListNode<int>( x+8*y+z*8*8 )
                                                    );
                                                };
                                                // exists x,y for loop
                                                x += 8*dx; y += 8*dy; z += 8*dz;
                                            };
                                        };
                                    };
                                };
                            };
                        };
                        break;
                    case 3: // knight
                        for ( int dz = -2; dz <= 2; dz += 1 ){
                            for ( int dx = -2; dx <= 2; dx += 1 ){
                                for ( int dy = -2; dy <= 2; dy += 1 ){
                                    if ( abs(dx) + abs(dy) + abs(dz) == 3 && ( dz == 0 || dy == 0 || dx == 0 ) ){
                                        int x = i%8+dx;
                                        int y = floor((i%(8*8))/8)+dy;
                                        int z = floor(i/(8*8))+dz;
                                        if ( x >= 0 && x < 8 && y >= 0 && y < 8 && z >= 0 && z < 8 ){
                                            if ( board[x+8*y+z*8*8].piece == 0 ){
                                                LinkedListNode<int>::appendNode(
                                                    &(res[i]),
                                                    new LinkedListNode<int>( x+y*8+z*8*8 )
                                                );
                                            }else if ( board[x+8*y+z*8*8].team != board[i].team ){
                                                LinkedListNode<int>::appendNode(
                                                    &(res[i]),
                                                    new LinkedListNode<int>( x+8*y+z*8*8 )
                                                );
                                            };
                                        };
                                    };
                                };
                            };
                        };
                        break;
                    case 4: // rook
                        for ( int dz = -1; dz <= 1; dz += 1 ){
                            for ( int dx = -1; dx <= 1; dx += 1 ){
                                for ( int dy = -1; dy <= 1; dy += 1 ){
                                    if ( abs(dx) + abs(dy) + abs(dz) == 1 ){
                                        for ( int x = i%8+dx , y = floor((i%(8*8))/8)+dy , z = floor(i/(8*8))+dz; ( x >= 0 && x < 8 && y >= 0 && y < 8 && z >= 0 && z < 8 ); x+=dx, y+=dy, z+=dz ){
                                            if ( board[x+8*y+z*8*8].piece == 0 ){
                                                LinkedListNode<int>::appendNode(
                                                    &(res[i]),
                                                    new LinkedListNode<int>( x+y*8+z*8*8 )
                                                );
                                            }else{
                                                if ( board[x+8*y+z*8*8].team != board[i].team ){
                                                    LinkedListNode<int>::appendNode(
                                                        &(res[i]),
                                                        new LinkedListNode<int>( x+8*y+z*8*8 )
                                                    );
                                                };
                                                // exists x,y for loop
                                                x += 8*dx; y += 8*dy; z += 8*dz;
                                            };
                                        };
                                    };
                                };
                            };
                        };
                        break;
                    case 5: // queen
                        for ( int dz = -1; dz <= 1; dz += 1 ){
                            for ( int dx = -1; dx <= 1; dx += 1 ){
                                for ( int dy = -1; dy <= 1; dy += 1 ){
                                    if ( abs(dx) + abs(dy) + abs(dz) == 2 || abs(dx) + abs(dy) + abs(dz) == 1 ){
                                        for ( int x = i%8+dx , y = floor((i%(8*8))/8)+dy , z = floor(i/(8*8))+dz; ( x >= 0 && x < 8 && y >= 0 && y < 8 && z >= 0 && z < 8 ); x+=dx, y+=dy, z+=dz ){
                                            if ( board[x+8*y+z*8*8].piece == 0 ){
                                                LinkedListNode<int>::appendNode(
                                                    &(res[i]),
                                                    new LinkedListNode<int>( x+y*8+z*8*8 )
                                                );
                                            }else{
                                                if ( board[x+8*y+z*8*8].team != board[i].team ){
                                                    LinkedListNode<int>::appendNode(
                                                        &(res[i]),
                                                        new LinkedListNode<int>( x+8*y+z*8*8 )
                                                    );
                                                };
                                                // exists x,y for loop
                                                x += 8*dx; y += 8*dy; z += 8*dz;
                                            };
                                        };
                                    };
                                };
                            };
                        };
                        break;
                    case 6: // king
                        for ( int dz = -1; dz <= 1; dz += 1 ){
                            for ( int dx = -1; dx <= 1; dx += 1 ){
                                for ( int dy = -1; dy <= 1; dy += 1 ){
                                    int x = i%8+dx;
                                    int y = floor((i%(8*8))/8)+dy;
                                    int z = floor(i/(8*8))+dz;
                                    if ( x >= 0 && x < 8 && y >= 0 && y < 8 && z >= 0 && z < 8 ){
                                        if ( board[x+8*y+z*8*8].piece == 0 ){
                                            LinkedListNode<int>::appendNode(
                                                &(res[i]),
                                                new LinkedListNode<int>( x+y*8+z*8*8 )
                                            );
                                        }else if ( board[x+8*y+z*8*8].team != board[i].team ){
                                            LinkedListNode<int>::appendNode(
                                                &(res[i]),
                                                new LinkedListNode<int>( x+8*y+z*8*8 )
                                            );
                                        };
                                    };
                                };
                            };
                        };
                        // castle short x
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
                        // castle long x
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
                        // castle short z
                        if (
                            board[i].moved == 0 &&
                            board[i+3*8*8].piece == 4 && board[i+3*8*8].team == board[i].team && board[i+3*8*8].moved == 0 &&
                            board[i+1*8*8].piece == 0 && board[i+2*8*8].piece == 0
                        ){
                            LinkedListNode<int>::appendNode(
                                &(res[i]),
                                new LinkedListNode<int>( i+2*8*8 )
                            );
                        };
                        // castle long z
                        if (
                            board[i].moved == 0 &&
                            board[i-4*8*8].piece == 4 && board[i-4*8*8].team == board[i].team && board[i-4*8*8].moved == 0 &&
                            board[i-1*8*8].piece == 0 && board[i-2*8*8].piece == 0 && board[i-3*8*8].piece == 0
                        ){
                            LinkedListNode<int>::appendNode(
                                &(res[i]),
                                new LinkedListNode<int>( i-2*8*8 )
                            );
                        };
                        break;
                };
            };
            return res;
        };
        static LinkedListNode<int>** getPossibleMovesWithKingCheck( piece_t* board ){
            LinkedListNode<int>** res = (LinkedListNode<int>**)calloc( sizeof( LinkedListNode<int>* ) , 8*8*8 );
            if ( res == nullptr ){
                std::cout << "calloc failed at App3D::getPossibleMoves" << std::endl;
                return nullptr;
            };
            for ( int i = 0; i < 8*8*8; i++ ){
                res[i] = nullptr;
                int ptd = board[i].team*2-1; // ptd = pieceTeamDirection
                switch ( board[i].piece ){
                    case 0: // no piece
                        break;
                    case 1: // pawn
                        // single forward
                        if (
                            i+8*ptd >= 0 && i+8*ptd < 8*8*8 &&
                            board[i+8*ptd].piece == 0
                        ){
                            App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+8*ptd );
                            // double forward
                            if (
                                floor((i%(8*8))/8) == 3.5-2.5*ptd &&
                                board[i+2*8*ptd].piece == 0
                            ){
                                App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+2*8*ptd );
                            };
                        };
                        // take right (and enpas)
                        if (
                            i%8 < 7 &&
                            i+8*ptd+1 >= 0 && i+8*ptd+1 < 8*8*8 &&
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
                            App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+8*ptd+1 );
                        };
                        // take left (with enpas)
                        if (
                            i%8 > 0 &&
                            i+8*ptd-1 >= 0 && i+8*ptd-1 < 8*8*8 &&
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
                            App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+8*ptd-1 );
                        };
                        // take up (with enpas)
                        if (
                            floor(i/(8*8)) < 7 &&
                            i+8*ptd+8*8 >= 0 && i+8*ptd+8*8 < 8*8*8 &&
                            (
                                (
                                    board[i+8*ptd+8*8].piece != 0 &&
                                    board[i+8*ptd+8*8].team != board[i].team
                                ) ||
                                (
                                    board[i+8*ptd+8*8].piece == 0 &&
                                    board[i+8*8].enpas == 1 &&
                                    board[i+8*8].piece == 1 &&
                                    board[i+8*8].team != board[i].team
                                )
                            )
                        ){
                            App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+8*ptd+8*8 );
                        };
                        // take down (with enpas)
                        if (
                            floor(i/(8*8)) > 0 &&
                            i+8*ptd-8*8 >= 0 && i+8*ptd-8*8 < 8*8*8 &&
                            (
                                (
                                    board[i+8*ptd-8*8].piece != 0 &&
                                    board[i+8*ptd-8*8].team != board[i].team
                                ) ||
                                (
                                    board[i+8*ptd-8*8].piece == 0 &&
                                    board[i-8*8].enpas == 1 &&
                                    board[i-8*8].piece == 1 &&
                                    board[i-8*8].team != board[i].team
                                )
                            )
                        ){
                            App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+8*ptd-8*8 );
                        };
                        break;
                    case 2: // bishop
                        for ( int dz = -1; dz <= 1; dz += 1 ){
                            for ( int dx = -1; dx <= 1; dx += 1 ){
                                for ( int dy = -1; dy <= 1; dy += 1 ){
                                    if ( abs(dx) + abs(dy) + abs(dz) == 2 ){
                                        for ( int x = i%8+dx , y = floor((i%(8*8))/8)+dy , z = floor(i/(8*8))+dz; ( x >= 0 && x < 8 && y >= 0 && y < 8 && z >= 0 && z < 8 ); x+=dx, y+=dy, z+=dz ){
                                            if ( board[x+8*y+z*8*8].piece == 0 ){
                                                App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8+z*8*8 );
                                            }else{
                                                if ( board[x+8*y+z*8*8].team != board[i].team ){
                                                    App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+8*y+z*8*8 );
                                                };
                                                // exists x,y for loop
                                                x += 8*dx; y += 8*dy; z += 8*dz;
                                            };
                                        };
                                    };
                                };
                            };
                        };
                        break;
                    case 3: // knight
                        for ( int dz = -2; dz <= 2; dz += 1 ){
                            for ( int dx = -2; dx <= 2; dx += 1 ){
                                for ( int dy = -2; dy <= 2; dy += 1 ){
                                    if ( abs(dx) + abs(dy) + abs(dz) == 3 && ( dz == 0 || dy == 0 || dx == 0 ) ){
                                        int x = i%8+dx;
                                        int y = floor((i%(8*8))/8)+dy;
                                        int z = floor(i/(8*8))+dz;
                                        if ( x >= 0 && x < 8 && y >= 0 && y < 8 && z >= 0 && z < 8 ){
                                            if ( board[x+8*y+z*8*8].piece == 0 ){
                                                App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8+z*8*8 );
                                            }else if ( board[x+8*y+z*8*8].team != board[i].team ){
                                                App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+8*y+z*8*8 );
                                            };
                                        };
                                    };
                                };
                            };
                        };
                        break;
                    case 4: // rook
                        for ( int dz = -1; dz <= 1; dz += 1 ){
                            for ( int dx = -1; dx <= 1; dx += 1 ){
                                for ( int dy = -1; dy <= 1; dy += 1 ){
                                    if ( abs(dx) + abs(dy) + abs(dz) == 1 ){
                                        for ( int x = i%8+dx , y = floor((i%(8*8))/8)+dy , z = floor(i/(8*8))+dz; ( x >= 0 && x < 8 && y >= 0 && y < 8 && z >= 0 && z < 8 ); x+=dx, y+=dy, z+=dz ){
                                            if ( board[x+8*y+z*8*8].piece == 0 ){
                                                App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8+z*8*8 );
                                            }else{
                                                if ( board[x+8*y+z*8*8].team != board[i].team ){
                                                    App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+8*y+z*8*8 );
                                                };
                                                // exists x,y for loop
                                                x += 8*dx; y += 8*dy; z += 8*dz;
                                            };
                                        };
                                    };
                                };
                            };
                        };
                        break;
                    case 5: // queen
                        for ( int dz = -1; dz <= 1; dz += 1 ){
                            for ( int dx = -1; dx <= 1; dx += 1 ){
                                for ( int dy = -1; dy <= 1; dy += 1 ){
                                    if ( abs(dx) + abs(dy) + abs(dz) == 2 || abs(dx) + abs(dy) + abs(dz) == 1 ){
                                        for ( int x = i%8+dx , y = floor((i%(8*8))/8)+dy , z = floor(i/(8*8))+dz; ( x >= 0 && x < 8 && y >= 0 && y < 8 && z >= 0 && z < 8 ); x+=dx, y+=dy, z+=dz ){
                                            if ( board[x+8*y+z*8*8].piece == 0 ){
                                                App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8+z*8*8 );
                                            }else{
                                                if ( board[x+8*y+z*8*8].team != board[i].team ){
                                                    App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+8*y+z*8*8 );
                                                };
                                                // exists x,y for loop
                                                x += 8*dx; y += 8*dy; z += 8*dz;
                                            };
                                        };
                                    };
                                };
                            };
                        };
                        break;
                    case 6: // king
                        for ( int dz = -1; dz <= 1; dz += 1 ){
                            for ( int dx = -1; dx <= 1; dx += 1 ){
                                for ( int dy = -1; dy <= 1; dy += 1 ){
                                    int x = i%8+dx;
                                    int y = floor((i%(8*8))/8)+dy;
                                    int z = floor(i/(8*8))+dz;
                                    if ( x >= 0 && x < 8 && y >= 0 && y < 8 && z >= 0 && z < 8 ){
                                        if ( board[x+8*y+z*8*8].piece == 0 ){
                                            App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+y*8+z*8*8 );
                                        }else if ( board[x+8*y+z*8*8].team != board[i].team ){
                                            App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , x+8*y+z*8*8 );
                                        };
                                    };
                                };
                            };
                        };
                        // castle short x
                        if (
                            board[i].moved == 0 &&
                            board[i+3].piece == 4 && board[i+3].team == board[i].team && board[i+3].moved == 0 &&
                            board[i+1].piece == 0 && board[i+2].piece == 0
                        ){
                            App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+2 );
                        };
                        // castle long x
                        if (
                            board[i].moved == 0 &&
                            board[i-4].piece == 4 && board[i-4].team == board[i].team && board[i-4].moved == 0 &&
                            board[i-1].piece == 0 && board[i-2].piece == 0 && board[i-3].piece == 0
                        ){
                            App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i-2 );
                        };
                        // castle short z
                        if (
                            board[i].moved == 0 &&
                            board[i+3*8*8].piece == 4 && board[i+3*8*8].team == board[i].team && board[i+3*8*8].moved == 0 &&
                            board[i+1*8*8].piece == 0 && board[i+2*8*8].piece == 0
                        ){
                            App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i+2*8*8 );
                        };
                        // castle long z
                        if (
                            board[i].moved == 0 &&
                            board[i-4*8*8].piece == 4 && board[i-4*8*8].team == board[i].team && board[i-4*8*8].moved == 0 &&
                            board[i-1*8*8].piece == 0 && board[i-2*8*8].piece == 0 && board[i-3*8*8].piece == 0
                        ){
                            App3D::appendPossibleMoveAndCheckKing( board , &(res[i]) , i , i-2*8*8 );
                        };
                        break;
                };
            };
            return res;
        };
        static void appendPossibleMoveAndCheckKing( piece_t* board , LinkedListNode<int>** fromPieceLL , int from , int to ){
            piece_t* boardPostMove = App3D::playPossibleMove( board , from , to );
            if ( App3D::isKingThreteaned( boardPostMove , board[from].team ) == false ){
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
            for ( int i = 0; i < 8*8*8; i++ ){ board[i].enpas = 0; };

            // pawn special rules
            if ( board[from].piece == 1 ){
                // enpas
                if ( ( from%8 != to%8 || floor(from/(8*8)) != floor(to/(8*8)) ) && board[to].piece == 0 ){
                    board[(int)(floor(from%(8*8)/8)*8+(to%8)+floor(to/(8*8))*(8*8))].piece = 0;
                    board[(int)(floor(from%(8*8)/8)*8+(to%8)+floor(to/(8*8))*(8*8))].team = 0;
                    board[(int)(floor(from%(8*8)/8)*8+(to%8)+floor(to/(8*8))*(8*8))].enpas = 0;
                    board[(int)(floor(from%(8*8)/8)*8+(to%8)+floor(to/(8*8))*(8*8))].moved = 0;
                }else
                // double forward
                if ( abs( (int)( floor((from%(8*8))/8) - floor((to%(8*8))/8) ) ) == 2 ){
                    board[from].enpas = 1;
                };
            };

            board[to] = board[from];
            board[from].piece = 0; board[from].team = 0; board[from].enpas = 0; board[from].moved = 0;
            board[to].moved = 1;
            
            // castle long x
            if ( board[to].piece == 6 && from%8 - to%8 == 2 ){
                board[to+1] = board[to-2];
                board[to-2].piece = 0; board[to-2].team = 0; board[to-2].enpas = 0; board[to-2].moved = 0;
            }else
            // castle short x
            if( board[to].piece == 6 && from%8 - to%8 == -2 ){
                board[to-1] = board[to+1];
                board[to+1].piece = 0; board[to+1].team = 0; board[to+1].enpas = 0; board[to+1].moved = 0;
            }else
            // castle long z
            if ( board[to].piece == 6 && floor(from/(8*8)) - floor(to/(8*8)) == 2 ){
                board[to+1*8*8] = board[to-2*8*8];
                board[to-2*8*8].piece = 0; board[to-2*8*8].team = 0; board[to-2*8*8].enpas = 0; board[to-2*8*8].moved = 0;
            }else
            // castle short z
            if( board[to].piece == 6 && floor(from/(8*8)) - floor(to/(8*8)) == -2 ){
                board[to-1*8*8] = board[to+1*8*8];
                board[to+1*8*8].piece = 0; board[to+1*8*8].team = 0; board[to+1*8*8].enpas = 0; board[to+1*8*8].moved = 0;
            };
            return;
        };
        static piece_t* playPossibleMove( piece_t* board , int from , int to ){
            piece_t* newBoard = (piece_t*)calloc( sizeof(piece_t) , 8*8*8 );
            if ( newBoard == nullptr ){
                std::cout << "calloc failed at App3D::playPossibleMove" << std::endl;
                return newBoard;
            };
            for ( int i = 0; i < 8*8*8; i++ ){
                newBoard[i] = board[i];
            };
            playPossibleMoveOnBoard( newBoard , from , to );
            return newBoard;
        };
        static bool isPieceThretened( piece_t* board , int piece ){
            LinkedListNode<int>** possibleMoves = App3D::getPossibleMoves( board );
            for ( int i = 0; i < 8*8*8; i++ ){
                if ( i != piece && board[i].piece != 0 && board[i].team != board[piece].team ){
                    LinkedListNode<int>* current = possibleMoves[i];
                    while( current != nullptr ){
                        if ( current->getValue() == piece ){
                            for ( int i = 0; i < 8*8*8; i++ ){
                                delete ( possibleMoves[i] );
                            };
                            free(possibleMoves);
                            return true;
                        };
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
            for ( int i = 0; i < 8*8*8; i++ ){
                if ( board[i].piece == 6 && board[i].team == team ){
                    return App3D::isPieceThretened( board , i );
                };
            };
            std::cout << "no king was found at App3D::isKingThreteaned(" << board << "," << team << ")\n";
            for ( int y = 0; y < 8; y++ ){
                for ( int x = 0; x < 8; x++ ){
                    std::cout << board[x+y*8].piece <<".";
                };
                std::cout << "\n";
            };
            return false;
        };
        static void leftMouseButtonPressed( void* This ){
            SDL_Rect rect;
            App3D::copyDrawOrigin( &rect , ((App3D*)This)->parentWindow );

            if (
                ((App3D*)This)->parentWindow->mouseX < rect.x ||
                ((App3D*)This)->parentWindow->mouseX >= rect.x+rect.w*(8*8+7) ||
                ((App3D*)This)->parentWindow->mouseY < rect.y ||
                ((App3D*)This)->parentWindow->mouseY >= rect.y+rect.w*8
            ){
                ((App3D*)This)->selectedPiece = -1;
                return;
            };
            int newSelectedPieceX = floor( ( ( ((App3D*)This)->parentWindow->mouseX - rect.x )/rect.w )%9 );
            int newSelectedPieceY = floor( ( ((App3D*)This)->parentWindow->mouseY - rect.y )/rect.w );
            int newSelectedPieceZ = floor( ( ( ((App3D*)This)->parentWindow->mouseX - rect.x )/(9*rect.w) )%64 );
            if ( ((App3D*)This)->axisSwaped == true ){
                int tmp = newSelectedPieceX;
                newSelectedPieceX = newSelectedPieceZ;
                newSelectedPieceZ = tmp;                
            };
            if (
                newSelectedPieceX < 0 ||
                newSelectedPieceX >= 8 ||
                newSelectedPieceY < 0 ||
                newSelectedPieceY >= 8 ||
                newSelectedPieceZ < 0 ||
                newSelectedPieceZ >= 8
            ){
                ((App3D*)This)->selectedPiece = -1;
                return;
            };
            int newSelectedPiece = newSelectedPieceX + 8*newSelectedPieceY + 8*8*newSelectedPieceZ;
            if ( ((App3D*)This)->selectedPiece >= 0 && ((App3D*)This)->selectedPiece < 8*8*8 ){
                
                LinkedListNode<int>* current = ((App3D*)This)->possibleMoves[((App3D*)This)->selectedPiece];
                while( current != nullptr ){
                    if ( current->getValue() == newSelectedPiece ){
                        if ( ((App3D*)This)->board[((App3D*)This)->selectedPiece].team != ((App3D*)This)->currentPlayer ){
                            // wrong team - cant play move
                            ((App3D*)This)->selectedPiece = newSelectedPiece;
                            return;
                        };
                        playPossibleMoveOnBoard( ((App3D*)This)->board , ((App3D*)This)->selectedPiece , newSelectedPiece );
                        // freeing ((App3D*)This)->possibleMoves
                        for ( int i = 0; i < 8*8; i++ ){
                            delete ( ((App3D*)This)->possibleMoves[i] );
                        };
                        free( ((App3D*)This)->possibleMoves );

                        ((App3D*)This)->possibleMoves = getPossibleMovesWithKingCheck( ((App3D*)This)->board );
                        ((App3D*)This)->selectedPiece = -1;

                        ((App3D*)This)->currentPlayer = 1-((App3D*)This)->currentPlayer;

                        ((App3D*)This)->updatePieceAmounts();
                        return;
                    };
                    
                    current = current->getNext();
                };
                ((App3D*)This)->selectedPiece = newSelectedPiece;
                return;
                
            };
            ((App3D*)This)->selectedPiece = newSelectedPiece;
            return;
        };
        static void rightMouseButtonPressed( void* This ){
            ((App3D*)This)->axisSwaped = !((App3D*)This)->axisSwaped;
            return;
        };
        static void copyDrawOrigin( SDL_Rect* _rect , Window* _parentWindow ){
            _rect->w = std::min( (int)((floor(_parentWindow->width/APP3D_WINDOW_1X_WIDTH)*APP3D_WINDOW_1X_WIDTH)/(8*8+7)) , (int)((floor(_parentWindow->height/APP3D_WINDOW_1X_HEIGHT)*APP3D_WINDOW_1X_HEIGHT)/8) );
            _rect->h = _rect->w;
            _rect->x = ( _parentWindow->width - _rect->w*(APP3D_WINDOW_1X_WIDTH/8) )/2;
            _rect->y = ( _parentWindow->height - _rect->w*(APP3D_WINDOW_1X_HEIGHT/8) )/2;
            return;
        };
        
};

#include "App3D.cpp"
#endif