/**
* Author: Nicolas Ollivier
* Assignment: Simple 2D Scene
* Date due: 2025-09-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

// Meets all 3 requirements and extra credit.
//Requirement 1: At Least Three Objects (25%) - duck, lemonade stand, baby duck, cat
//Requirement 2: Relative Position (10%) - baby duck follows duck
//Requirement 3: Movement (65%) - all move with trig functions, one rotates, one scales
//Extra Credit: Swaps between black and white bgcolor

#include "CS3113/cs3113.h"
#include <math.h>

// Global Constants
constexpr int   SCREEN_WIDTH  = 1600 / 2,
                SCREEN_HEIGHT = 900 / 2,
                FPS           = 60,
                SIZE          = 200,
                FRAME_LIMIT   = 100,
                DUCK_OFFSET   = 10;
constexpr float MAX_AMP       = 100.0f,
                ROTATION_AMP  = 100.0f;

constexpr char    BG_COLOUR[]        = "#000000";
          char    SECONDARY_COLOUR[] = "#ffffff"; 
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE   = { (float) SIZE, (float) SIZE };

constexpr char DUCK[] = "assets/duck.png";
constexpr char LEMONADE[] = "assets/lemonadestand.png";
constexpr char CAT[] = "assets/ooeeaaee.png";

// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gScaleFactor   = SIZE,
          gAngle         = 0.0f,
          gPulseTime     = 0.0f,
          gPreviousSwap  = 0.0f,
          gPreviousTick  = 0.0f,
          gFrameCounter  = 0.0f;
Vector2   gPosition      = ORIGIN,
          gDuckPosition  = ORIGIN,
          gCatPosition   = ORIGIN,
          gBabyDuckPos   = ORIGIN;
Vector2   gScale         = BASE_SIZE;
Texture2D gCat,
          gDuck,
          gLemonade;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

    gDuck     = LoadTexture(DUCK);
    gCat      = LoadTexture(CAT);
    gLemonade = LoadTexture(LEMONADE); 

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    float time = (float) GetTime();
    float deltaTime = time - gPreviousTick;
    gPreviousTick = time;

    gPulseTime += 1.0f * deltaTime;

    gScale = {
        BASE_SIZE.x + MAX_AMP * cos(gPulseTime),
        BASE_SIZE.y + MAX_AMP * cos(gPulseTime)
    };

    gPosition.x = -cos(gPulseTime)*ROTATION_AMP + ORIGIN.x;
    gPosition.y = -sin(gPulseTime)*ROTATION_AMP + ORIGIN.y;
    
    gDuckPosition.x = cos(gPulseTime)*ROTATION_AMP + ORIGIN.x;
    gDuckPosition.y = cos(gPulseTime)*ROTATION_AMP + ORIGIN.y;

    gCatPosition.x = sin(gPulseTime)*ROTATION_AMP + ORIGIN.x;
    gCatPosition.y = tan(gPulseTime)*ROTATION_AMP + ORIGIN.y;

    gBabyDuckPos.x = gDuckPosition.x - DUCK_OFFSET;
    gBabyDuckPos.y = gDuckPosition.y;
}

void render()
{
    BeginDrawing();
    if (int(gPulseTime) & 1 == 1){
        ClearBackground(ColorFromHex(BG_COLOUR));
    }
    else{
        ClearBackground(ColorFromHex(SECONDARY_COLOUR));
    }

    Rectangle duckTextureArea = {
        0,0,

        static_cast<float>(gDuck.width),
        static_cast<float>(gDuck.height)
    };

    Rectangle catTextureArea = {
        0,0,

        static_cast<float>(gCat.width),
        static_cast<float>(gCat.height)
    };

    Rectangle lemonadeTextureArea = {
        0,0,

        static_cast<float>(gLemonade.width),
        static_cast<float>(gLemonade.height)
    };

    Rectangle babyDuckTexture = {
        0,0,

        static_cast<float>(gDuck.width),
        static_cast<float>(gDuck.height)
    };

    // Destination rectangle – centred on gPosition
    Rectangle catDestinationArea = {
        gCatPosition.x,
        gCatPosition.y,
        static_cast<float>(gScale.x),
        static_cast<float>(gScale.y)
    };

    Rectangle duckDestinationArea = {
        gDuckPosition.x,
        gDuckPosition.y,
        static_cast<float>(BASE_SIZE.x),
        static_cast<float>(BASE_SIZE.y)
    };

    Rectangle babyDuckDestination = {
        gBabyDuckPos.x,
        gBabyDuckPos.y,
        static_cast<float>(BASE_SIZE.x/2),
        static_cast<float>(BASE_SIZE.y/2)
    };

    Rectangle lemonadeDestinationArea = {
        gPosition.x+100,
        gPosition.y,
        static_cast<float>(BASE_SIZE.x),
        static_cast<float>(BASE_SIZE.y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 objectOrigin = {
        static_cast<float>(BASE_SIZE.x) / 2.0f,
        static_cast<float>(BASE_SIZE.y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        gCat, 
        catTextureArea, 
        catDestinationArea, 
        objectOrigin, 
        gPulseTime*30, 
        WHITE
    );

    DrawTexturePro(
        gDuck, 
        duckTextureArea, 
        duckDestinationArea, 
        objectOrigin, 
        0, 
        WHITE
    );

    DrawTexturePro(
        gDuck,
        babyDuckTexture,
        babyDuckDestination, 
        objectOrigin, 
        0, 
        WHITE
    );

    DrawTexturePro(
        gLemonade, 
        lemonadeTextureArea, 
        lemonadeDestinationArea, 
        objectOrigin, 
        0, 
        WHITE
    );

    EndDrawing();
}

void shutdown() { CloseWindow(); }

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}