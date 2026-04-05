#include "Level.h"

Level::Level()                                      : Scene { {0.0f}, nullptr   } {}
Level::Level(Vector2 origin, const char *bgHexCode, int* lives, unsigned int* levelData, int levelWidth, int levelHeight, int nextIndex, bool evilgoots, bool flygoots, bool plat) 
: Scene { origin, bgHexCode }, lives(lives), mLevelData(levelData), levelWidth(levelWidth), levelHeight(levelHeight), nextIndex(nextIndex), evilgoots(evilgoots), flygoots(flygoots), plat(plat) {}

Level::~Level() { shutdown(); }

void Level::initialise()
{
   mGameState.nextSceneID = 0;


   mGameState.jumpSound = LoadSound("assets/honk-sound.mp3");
   mGameState.dieSound = LoadSound("assets/goosedie.mp3");

   /*
      ----------- MAP -----------
   */
   mGameState.map = new Map(
      levelWidth, levelHeight,   // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/tileset.png",   // texture filepath
      TILE_DIMENSION/2,              // tile size
      8, 8,                        // texture cols & rows
      mOrigin                      // in-game origin
   );

   /*
      ----------- PROTAGONIST -----------
   */
   std::map<Direction, std::vector<int>> gootsAnimationAtlas = {
      {UPRIGHT,  {  0,1,2,3,4,5,6,7,8,9 }},
      {RIGHT,  {  14,15,16,17,18,19,20,21,22,23,24,25 }},
      {UPLEFT,    { 28,29,30,31,32,33,34,35,36,37 }},
      {LEFT, { 42,43,44,45,46,47,48,49,50,51,52,53 }},
   };

  

   float sizeRatio  = 48.0f / 64.0f;

   mGameState.goots = new Entity(
      {mOrigin.x - 400, mOrigin.y - 150}, // position
      {150.0f, 100.0f},             // scale
      "assets/goots.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      { 8, 7 },                                 // atlas dimensions
      gootsAnimationAtlas,                    // actual atlas
      PLAYER                                    // entity type
   );


 
   babies = *lives;
   mGameState.babies = new Entity*[babies];

   for(int i = 0; i < babies; ++i){
    mGameState.babies[i] = new Entity(
        {mOrigin.x - 400 - 80 - i * 30, mOrigin.y - 150}, // position
        {50.0f, 40.0f},             // scale
        "assets/babies.png",                   // texture file address
        ATLAS,                                    // single image or atlas?
        { 8, 7 },                                 // atlas dimensions
        gootsAnimationAtlas,                    // actual atlas
        NPC                                    // entity type
    );
    mGameState.babies[i]->setFollowDist(80 + i * 30);
    mGameState.babies[i]->setAIType(FOLLOWER);
   mGameState.babies[i]->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
     mGameState.babies[i]->setColliderDimensions({
      mGameState.babies[i]->getScale().x/ 3.0f,
      mGameState.babies[i]->getScale().y / 1.0f
   });
  }
   
  if(evilgoots){
     mGameState.evilgoots = new Entity(
        {mOrigin.x + 350, mOrigin.y}, // position
        {50.0f, 40.0f},             // scale
        "assets/evilgoot.png",                   // texture file address
        ATLAS,                                    // single image or atlas?
        { 8, 7 },                                 // atlas dimensions
        gootsAnimationAtlas,                    // actual atlas
        NPC                                    // entity type
    );
    mGameState.evilgoots->setAIType(WANDERER);
    mGameState.evilgoots->moveLeft();
    mGameState.evilgoots->setSpeed(20);
    mGameState.evilgoots->setFrameSpeed(7);
    mGameState.evilgoots->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
     mGameState.evilgoots->setColliderDimensions({
      mGameState.evilgoots->getScale().x/ 3.0f,
      mGameState.evilgoots->getScale().y / 1.0f
   });

  }
  if(flygoots){
     mGameState.flygoots = new Entity(
        {mOrigin.x + 500, mOrigin.y-145}, // position
        {50.0f, 40.0f},             // scale
        "assets/evilgoot.png",                   // texture file address
        ATLAS,                                    // single image or atlas?
        { 8, 7 },                                 // atlas dimensions
        gootsAnimationAtlas,                    // actual atlas
        NPC                                    // entity type
    );
    mGameState.flygoots->setAIType(FLYER);
    mGameState.flygoots->moveLeft();
    mGameState.flygoots->setSpeed(50);
    mGameState.flygoots->setFrameSpeed(7);
    mGameState.flygoots->setColliderDimensions({
        mGameState.flygoots->getScale().x/ 3.0f,
        mGameState.flygoots->getScale().y / 1.0f
    });
  }

  if(plat){
     mGameState.movingPlat = new Entity(
        {mOrigin.x + 10, mOrigin.y}, // position
        {376.0f*1.5/2.5f, 93.0f/2.5f},             // scale
        "assets/platform.png",                   // texture file address
        PLATFORM                                    // entity type
    );
    mGameState.movingPlat->setAIType(FLYER);
    mGameState.movingPlat->moveLeft();
    mGameState.movingPlat->setSpeed(50);
    mGameState.movingPlat->setFrameSpeed(7);
  } else mGameState.movingPlat = nullptr;


   mGameState.knife = new Entity(
      {mOrigin.x + 600, mOrigin.y + 50}, // position
      {100.0f, 100.0f},             // scale
      "assets/knife.png",                   // texture file address
       BLOCK                                    // entity type
   );
    mGameState.knife->setColliderDimensions({
      mGameState.knife->getScale().x/ 2.0f,
      mGameState.knife->getScale().y / 2.0f
   });



   mGameState.goots->setJumpingPower(400.0f);
   mGameState.goots->setColliderDimensions({
      mGameState.goots->getScale().x/ 3.0f,
      mGameState.goots->getScale().y / 1.05f
   });
   mGameState.goots->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
}

void Level::update(float deltaTime)
{
//    UpdateMusicStream(mGameState.bgm);

   mGameState.goots->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      mGameState.movingPlat,        // collidable entities
      mGameState.movingPlat ? 1: 0                // col. entity count
   );

   if(evilgoots){
        mGameState.evilgoots->update(
        deltaTime,      // delta time / fixed timestep
        mGameState.goots,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );
    }
   if(flygoots){
        mGameState.flygoots->update(
        deltaTime,      // delta time / fixed timestep
        mGameState.goots,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );
    }
   if(plat){
        mGameState.movingPlat->update(
        deltaTime,      // delta time / fixed timestep
        mGameState.goots,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );
    }

   if(mGameState.goots->onDeadlyTile()) {
        --*lives;
        shutdown();
        initialise();
        PlaySound(mGameState.dieSound);
        return;
    }

   for(int i = 0; i < babies; ++i){
    if (mGameState.babies[i]->onDeadlyTile()){
        --*lives;
        shutdown();
        initialise();
        PlaySound(mGameState.dieSound);
        return;
    }
    mGameState.babies[i]->update(
       deltaTime,      // delta time / fixed timestep
       mGameState.goots,        // player
       mGameState.map, // map
       mGameState.movingPlat,        // collidable entities
       mGameState.movingPlat ? 1: 0                // col. entity count
    );
}
   

   mGameState.knife->update(
      deltaTime,      // delta time / fixed timestep
      mGameState.goots,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );


   if (!mGameState.knife->isActive()) {
    // printf("sdfjdsk\n");
    mGameState.nextSceneID = nextIndex;
   }
   if (evilgoots && !mGameState.evilgoots->isActive()){
        --*lives;
        shutdown();
        initialise();
        PlaySound(mGameState.dieSound);
        return;
   }
   if (flygoots && !mGameState.flygoots->isActive()){
        --*lives;
        shutdown();
        initialise();
        PlaySound(mGameState.dieSound);
        return;
   }
}

void Level::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.map->render();
   mGameState.goots->render();
   mGameState.knife->render();
   for(int i = 0; i < babies; ++i){
    mGameState.babies[i]->render();
   }

   if(plat) mGameState.movingPlat->render();
   if(evilgoots) mGameState.evilgoots->render();
   if(flygoots) mGameState.flygoots->render();
}

void Level::shutdown()
{
   if (mGameState.goots) delete mGameState.goots;
   mGameState.goots = nullptr;

   for(int i = 0; mGameState.babies && i < babies; ++i) {
      delete mGameState.babies[i];
   }
   delete [] mGameState.babies;
   mGameState.babies = nullptr;

   if(mGameState.knife) delete mGameState.knife;
   mGameState.knife = nullptr;

   if(mGameState.map) delete mGameState.map;
   mGameState.map = nullptr;

   if(evilgoots) delete mGameState.evilgoots;
   mGameState.evilgoots=nullptr;

   if(flygoots) delete mGameState.flygoots;
   mGameState.flygoots=nullptr;

//    UnloadMusicStream(mGameState.bgm);
   // if(IsSoundValid(mGameState.jumpSound)) UnloadSound(mGameState.jumpSound);
}