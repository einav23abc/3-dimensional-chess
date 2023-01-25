
Sprite::Sprite( const char* textureFileName , int _frameWidth , int _frameHeight , SDL_Renderer* renderer ){
    
    SDL_Surface* tempSurface = IMG_Load( textureFileName );
    this->texture = SDL_CreateTextureFromSurface( renderer , tempSurface );
    SDL_FreeSurface( tempSurface );

    SDL_QueryTexture(this->texture, NULL, NULL, &this->textureWidth, &this->textureheight);
    this->frameWidth = _frameWidth;
    this->frameHeight = _frameHeight;

    this->framesPerRow = this->textureWidth/this->frameWidth;
    this->frameRows = this->textureheight/this->frameHeight;
    this->frames = this->frameRows*this->framesPerRow;

    return;
};

Sprite::~Sprite(){
    return;
};

void Sprite::copyFrame( SDL_Rect* srcRect , int frameIndex ){
    srcRect->w = this->frameWidth;
    srcRect->h = this->frameHeight;
    srcRect->x = ((frameIndex%this->frames)%this->framesPerRow)*this->frameWidth;
    srcRect->y = (((frameIndex%this->frames)-(frameIndex%this->frames)%this->framesPerRow)/this->framesPerRow)*this->frameHeight;
    return;
};