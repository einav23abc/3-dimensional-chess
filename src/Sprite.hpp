#ifndef Sprite_HPP
#define Sprite_HPP


class Sprite {
    public:
        Sprite( const char* textureFileName , int frameWidth , int frameHeight , SDL_Renderer* renderer );
        ~Sprite();
        void copyFrame( SDL_Rect* srcRect , int frameIndex );
        SDL_Texture* getTexture(){ return this->texture; };
        int getFrameWidth(){ return this->frameWidth; };
        int getFrameHeight(){ return this->frameHeight; };
    private:
        SDL_Texture* texture;
        int textureWidth;
        int textureheight;
        int frameWidth;
        int frameHeight;
        int framesPerRow;
        int frameRows;
        int frames;
};

#include "Sprite.cpp"
#endif