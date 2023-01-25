#ifndef drawText_CPP
#define drawText_CPP
#include "Sprite.hpp"
#include <string>
#include <string.h>

typedef struct {
    int x;
    int y;
} point_t;

Sprite* fontSprite = nullptr;
/* // font v1:
int fontSpacing[128] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    6,2,4,6,6,6,6,2,3,3,4,4,3,4,6,6,
    4,2,4,4,4,4,4,4,4,4,2,3,4,4,4,4,
    6,5,5,5,5,4,4,5,5,4,4,4,4,6,6,4,
    4,5,4,4,4,4,6,6,4,6,4,3,6,3,4,6,
    3,5,4,4,4,4,4,4,4,2,3,4,3,6,4,4,
    4,5,4,4,4,4,6,6,4,4,4,4,2,4,5,0,
};
*/
// font v2:
int fontSpacing[128] = {
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    4,2,4,6,5,6,6,3, 3,3,4,6,3,4,2,4,
    5,3,5,5,5,5,5,5, 5,5,2,3,4,5,4,5,
    9,5,5,4,5,5,5,5, 5,2,4,5,4,6,5,6,
    5,6,5,5,6,5,6,8, 6,6,5,3,4,3,6,6,
    4,5,5,4,5,5,3,5, 5,2,3,5,2,6,5,5,
    5,5,4,4,4,5,5,6, 6,5,5,4,2,4,5,0,
};

void drawTextInit( SDL_Renderer* renderer ){
    //fontSprite = new Sprite( "assets/fontv1.png" , 5 , 5 , renderer );
    if ( fontSprite == nullptr ){
        fontSprite = new Sprite( "assets/fontv2.png" , 8 , 8 , renderer );
    };
    return;
};

int getTextLineHeight( int size ){
    return (fontSprite->getFrameHeight()+2)*size;
};

point_t drawText( char* text , SDL_Renderer* renderer , int xpos , int ypos , int size , bool rightAligned , int lineBreakAt ){
    SDL_Rect srcRect, destRect;
    point_t currentPos;
    currentPos.x = xpos;
    currentPos.y = ypos;
    if ( rightAligned == true ){
        currentPos.x += fontSprite->getFrameWidth()*size -fontSpacing[(int)(text[strlen(text)-1])]*size;
        for ( int i = strlen(text)-1; i >= 0; i-- ){
            destRect.h = fontSprite->getFrameHeight()*size;
            destRect.w = fontSprite->getFrameWidth()*size;
            destRect.x = currentPos.x-fontSprite->getFrameWidth()*size;
            destRect.y = currentPos.y;
            fontSprite->copyFrame( &srcRect, (int)(text[i]) );
            SDL_RenderCopy( renderer , fontSprite->getTexture() , &srcRect , &destRect );
            if ( i > 0 ){
                currentPos.x -= fontSpacing[(int)(text[i-1])]*size;
            };
            // special cases:
            switch( (int)(text[i]) ){
                case ((int)'\n'):
                    currentPos.x = xpos;
                    currentPos.y += (fontSprite->getFrameHeight()+2)*size;
                    break;
            };
        };
    }else{
        for ( int i = 0; i < strlen(text); i++ ){
            if ( currentPos.x+fontSpacing[(int)(text[i])]*size >= lineBreakAt ){
                currentPos.x = xpos;
                currentPos.y += (fontSprite->getFrameHeight()+2)*size;
            };
            destRect.h = fontSprite->getFrameHeight()*size;
            destRect.w = fontSprite->getFrameWidth()*size;
            destRect.x = currentPos.x;
            destRect.y = currentPos.y;
            fontSprite->copyFrame( &srcRect, (int)(text[i]) );
            SDL_RenderCopy( renderer , fontSprite->getTexture() , &srcRect , &destRect );
            currentPos.x += fontSpacing[(int)(text[i])]*size;
            // special cases:
            switch( (int)(text[i]) ){
                case ((int)'\n'):
                    currentPos.x = xpos;
                    currentPos.y += (fontSprite->getFrameHeight()+2)*size;
                    break;
            };
        };
    };
    
    currentPos.y += (fontSprite->getFrameHeight()+2)*size;
    return currentPos;
};

point_t getTextDrawHiddenString( char* stringDest , char* text , int xpos , int ypos , int size , bool rightAligned , int lineBreakAt ){
    int stringDestIndex = 0;
    stringDest[0] = '\0';

    bool addToString = true;

    point_t currentPos;
    currentPos.x = xpos;
    currentPos.y = ypos;
    if ( rightAligned ){
        for ( int i = strlen(text)-1; i >= 0; i-- ){
            if ( currentPos.y + (fontSprite->getFrameHeight()+2)*size >= 0 ){
                addToString = false;
            };

            if ( addToString == true ){
                stringDest[stringDestIndex+1] = '\0';
                stringDest[stringDestIndex] = text[i];
                stringDestIndex++;
            };
            if ( i > 0 ){
                currentPos.x -= fontSpacing[(int)(text[i-1])]*size;
            };
            // special cases:
            switch( (int)(text[i]) ){
                case ((int)'\n'):
                    currentPos.x = xpos;
                    currentPos.y += (fontSprite->getFrameHeight()+2)*size;
                    break;
            };
        };
    }else{
        for ( int i = 0; i < strlen(text); i++ ){
            if ( currentPos.y + (fontSprite->getFrameHeight()+2)*size >= 0 ){
                addToString = false;
            };

            if ( currentPos.x+fontSpacing[(int)(text[i])]*size >= lineBreakAt ){
                currentPos.x = xpos;
                currentPos.y += (fontSprite->getFrameHeight()+2)*size;
            };

            if ( addToString == true ){
                stringDest[stringDestIndex+1] = '\0';
                stringDest[stringDestIndex] = text[i];
                stringDestIndex++;
            };

            currentPos.x += fontSpacing[(int)(text[i])]*size;
            // special cases:
            switch( (int)(text[i]) ){
                case ((int)'\n'):
                    currentPos.x = xpos;
                    currentPos.y += (fontSprite->getFrameHeight()+2)*size;
                    break;
            };
        };
    };

    currentPos.y += (fontSprite->getFrameHeight()+2)*size;
    return currentPos;
};


#endif