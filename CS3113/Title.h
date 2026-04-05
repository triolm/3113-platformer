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
    bool evilgoots;
    bool flygoots;
    const char* filepath;

    Entity* goose;
    

public:
    static constexpr float TILE_DIMENSION       = 75.0f, 
                        ACCELERATION_OF_GRAVITY = 900.0f, 
                        END_GAME_THRESHOLD      = 800.0f;

    // Title();
    Title(Vector2 origin, const char *bgHexCode, const char* filepath): Scene { origin, bgHexCode }, filepath(filepath){}

    void initialise() override{
        mGameState.bg = new Entity{
            {0, 0}, // position
            {1000, 600},             // scale
            filepath,                   // texture file address
            NONE   
        };
    }

    void update(float deltaTime) override{
        // printf("DSFSDFD\n");
    }
    void render() override{
        ClearBackground(ColorFromHex(mBGColourHexCode));
        // printf("dsfsd\n");
        mGameState.bg->render();
    }
    void shutdown() override{
        printf("%i\n\n", mGameState.bg);
        if(mGameState.bg) delete mGameState.bg;
        mGameState.bg = nullptr;
    }
};

#endif
