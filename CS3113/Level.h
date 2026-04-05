#include "Scene.h"

#ifndef Level_H
#define Level_H

// constexpr int LEVEL_WIDTH  = 75, 
// LEVEL_HEIGHT = 19;

class Level : public Scene {
private:
    int levelWidth;
    int levelHeight;
    unsigned int* mLevelData;
    int* lives;
    int babies;
    int nextIndex;
    bool evilgoots;
    bool flygoots;
    bool plat;

    

public:
    static constexpr float TILE_DIMENSION       = 75.0f, 
                        ACCELERATION_OF_GRAVITY = 900.0f, 
                        END_GAME_THRESHOLD      = 800.0f;

    Level();
    Level(Vector2 origin,  const char *bgHexCode,  int* lives, unsigned int* levelData, int levelWidth, int levelHeight, int nextIndex, bool evilgoots = false, bool flygoots = false, bool plat = false);
    virtual ~Level();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
