#include <cmath>
#include <cstdio>
#include <vector>
#include <SDL2/SDL.h>
#include "camera.hpp"
#include "map.hpp"
#include "vec2.hpp"


const char WINDOW_TITLE[] = "Raycast Prototype 3";
const int WINDOW_X = SDL_WINDOWPOS_UNDEFINED;
const int WINDOW_Y = SDL_WINDOWPOS_UNDEFINED;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const uint32_t WINDOW_FLAGS = SDL_WINDOW_SHOWN;

const double PLAYER_SPEED = 0.03;
const double PLAYER_TURN = M_PI / 180; // 1 degree


bool gQuit = false;
namespace gKbd
{
    bool up = false;
    bool left = false;
    bool down = false;
    bool right = false;
    bool comma = false;
    bool period = false;
}


SDL_Window* gWindow = NULL;


// Moves camera according to input
void moveCamera(Camera& camera);

// Initializes SDL and create window
bool init();

// Destroys window and quits SDL
void close();

// Polls SDL events
void pollEvents();


int main(int argc, char* argv[])
{
    if (!init())
    {
        return 1;
    }
    
    Map world(
    {
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0}
    });
    Camera player(&world, Vec2(4, 0), Vec2(0, 1), M_PI/2, 1, 16);
    
    SDL_Surface* windowSurface = SDL_GetWindowSurface(gWindow);
    
    while(!gQuit)
    {
        pollEvents();
        moveCamera(player);
        player.render(windowSurface);
        SDL_UpdateWindowSurface(gWindow);
    }
    
    close();
    return 0;
}


void moveCamera(Camera& camera)
{
    if (gKbd::up)
    {
        camera.moveZ(PLAYER_SPEED);
    }
    if (gKbd::down)
    {
        camera.moveZ(-PLAYER_SPEED);
    }
    if (gKbd::left)
    {
        camera.rotate(PLAYER_TURN);
    }
    if (gKbd::right)
    {
        camera.rotate(-PLAYER_TURN);
    }
    if (gKbd::comma) // this way of doing strafing allows straferunning
    {
        camera.moveX(-PLAYER_SPEED);
    }
    if (gKbd::period)
    {
        camera.moveX(PLAYER_SPEED);
    }
}


bool init()
{
    bool success = true;
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Create window
        gWindow = SDL_CreateWindow(WINDOW_TITLE,
                                   WINDOW_X,
                                   WINDOW_Y,
                                   WINDOW_WIDTH,
                                   WINDOW_HEIGHT,
                                   WINDOW_FLAGS);
        if (gWindow == NULL)
        {
            printf("Could not create window: %s\n", SDL_GetError());
            success = false;
        }
    }
    
    return success;
}


void close()
{
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}


void pollEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
            case SDL_QUIT:
                gQuit = true;
                break;
                
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                    case SDLK_UP:
                        gKbd::up = true;
                        break;
                    case SDLK_LEFT:
                        gKbd::left = true;
                        break;
                    case SDLK_DOWN:
                        gKbd::down = true;
                        break;
                    case SDLK_RIGHT:
                        gKbd::right = true;
                        break;
                    case SDLK_COMMA:
                        gKbd::comma = true;
                        break;
                    case SDLK_PERIOD:
                        gKbd::period = true;
                        break;
                }
                break;
                
            case SDL_KEYUP:
                switch (e.key.keysym.sym)
                {
                    case SDLK_UP:
                        gKbd::up = false;
                        break;
                    case SDLK_LEFT:
                        gKbd::left = false;
                        break;
                    case SDLK_DOWN:
                        gKbd::down = false;
                        break;
                    case SDLK_RIGHT:
                        gKbd::right = false;
                        break;
                    case SDLK_COMMA:
                        gKbd::comma = false;
                        break;
                    case SDLK_PERIOD:
                        gKbd::period = false;
                        break;
                }
                break;
        }
    }
}
