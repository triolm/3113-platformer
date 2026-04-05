#include "Scene.h"

#ifndef Title_h
#define Title_h

// constexpr int LEVEL_WIDTH  = 75, 
// LEVEL_HEIGHT = 19;

class Title : public Scene {
private:
    int levelWidth;
    int levelHeight;
    unsigned int* mLevelData;
    int* lives;
    int babies;
    int nextIndex;
    // bool evilgoots;
    // bool flygoots;
    const char* filepath;
    bool title;

    Entity* goose = nullptr;
    Entity* geese[5];
    

public:
    static constexpr float TILE_DIMENSION       = 75.0f, 
                        ACCELERATION_OF_GRAVITY = 900.0f, 
                        END_GAME_THRESHOLD      = 800.0f;

    // Title();
    Title(Vector2 origin, const char *bgHexCode, const char* filepath, bool title = false): Scene { origin, bgHexCode }, filepath(filepath), title(title){}

    void initialise() override{
        mGameState.bg = new Entity{
            {0, 0}, // position
            {1000, 600},             // scale
            filepath,                   // texture file address
            NONE   
        };

        
        float sizeRatio  = 48.0f / 64.0f;
        
        if(title){
            std::map<Direction, std::vector<int>> gootsAnimationAtlas = {
                {UPRIGHT,  {  14,15,16,17,18,19,20,21,22,23,24,25 }},
                {RIGHT,  {  14,15,16,17,18,19,20,21,22,23,24,25 }},
                {UPLEFT,    { 42,43,44,45,46,47,48,49,50,51,52,53 }},
                {LEFT, { 42,43,44,45,46,47,48,49,50,51,52,53 }},
            };
            goose = new Entity(
                {-mOrigin.x, mOrigin.y - 170}, // position
                {150.0f * 1.5, 100.0f * 1.5},             // scale
                "assets/goots.png",                   // texture file address
                ATLAS,                                    // single image or atlas?
                { 8, 7 },                                 // atlas dimensions
                gootsAnimationAtlas,                    // actual atlas
                PLAYER                                    // entity type
            );
            goose->moveRight();
            goose->setSpeed(60);
            goose->setFrameSpeed(10);

            for(int i = 0; i < 5; ++i){
                geese[i] = new Entity(
                    {-mOrigin.x - 120 - i * 45, mOrigin.y - 128}, // position
                    {50.0f * 1.5, 40.0f* 1.5},             // scale
                    "assets/babies.png",                   // texture file address
                    ATLAS,                                    // single image or atlas?
                    { 8, 7 },                                 // atlas dimensions
                    gootsAnimationAtlas,                    // actual atlas
                    PLAYER                                    // entity type
                );
                geese[i]->moveRight();
                geese[i]->setSpeed(60);
                geese[i]->setFrameSpeed(10);
            }
        }
    }

    void update(float deltaTime) override{
        if(title) {
            goose->update(
                deltaTime,      // delta time / fixed timestep
                nullptr,        // player
                nullptr, // map
                nullptr,        // collidable entities
                0               // col. entity count
            );
            if(goose->getPosition().x > mOrigin.x + 50){
                goose->setPosition({-mOrigin.x - 50, goose->getPosition().y});
            }
            for(int i = 0; i < 5; ++i){
                geese[i]->update(
                    deltaTime,      // delta time / fixed timestep
                    nullptr,        // player
                    nullptr, // map
                    nullptr,        // collidable entities
                    0               // col. entity count
                );
                if(geese[i]->getPosition().x > mOrigin.x + 50){
                    geese[i]->setPosition({goose->getPosition().x - 120 - i * 45, geese[i]->getPosition().y});
                }
            }
        }
    }
    void render() override{
        ClearBackground(ColorFromHex(mBGColourHexCode));
        // printf("dsfsd\n");
        mGameState.bg->render();
        if(title){
            goose->render();
            for(int i = 0; i < 5; ++i){
                geese[i]->render();
            }
        }

    }
    void shutdown() override{
        // printf("%i\n\n", mGameState.bg);
        if(mGameState.bg) delete mGameState.bg;
        mGameState.bg = nullptr;
        
        if(goose) {
            for(int i = 0; i < 5; ++i) delete geese[i];
            delete goose;
            goose = nullptr;
        }
    }
};

#endif
