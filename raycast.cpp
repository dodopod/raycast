#include <cmath>
#include <cstdio>
#include <vector>
#include <SDL2/SDL.h>


const char WINDOW_TITLE[] = "Raycast Prototype 1";
const int WINDOW_X = SDL_WINDOWPOS_UNDEFINED;
const int WINDOW_Y = SDL_WINDOWPOS_UNDEFINED;
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const uint32_t WINDOW_FLAGS = SDL_WINDOW_SHOWN;

const SDL_Color SKY_COLOR = {0, 0, 0, 255};
const SDL_Color WALL_COLOR = {0, 0, 255, 255};

SDL_Window* gWindow = NULL;

typedef std::vector<bool> MapRow;
typedef std::vector<MapRow> Map;

struct Vec2
{
    double x;
    double y;
};
Vec2 operator-(const Vec2& v)
{
    return {-v.x, -v.y};
}
Vec2 operator+(const Vec2& u, const Vec2& v)
{
    return {u.x + v.x, u.y + v.y};
}
Vec2 operator-(const Vec2& u, const Vec2& v)
{
    return {u.x - v.x, u.y - v.y};
}
Vec2 operator*(double a, const Vec2& v)
{
    return {a * v.x, a * v.y};
}
inline double dot(const Vec2& u, const Vec2& v)
{
    return u.x * v.x + u.y * v.y;
}
inline double magnitude(const Vec2& v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

struct Ray
{
    Vec2 origin;
    Vec2 direction;
};

struct Camera
{
    Vec2 viewpoint; // position of viewpoint
    Vec2 direction; // direction of view
    Vec2 imageSize; // half the image width (in world coordinates)
};


// Initializes SDL and create window
bool init();

// Destroys window and quits SDL
void close();

// Renders the world to the given surface
void render(SDL_Surface* surface, const Camera& camera, const Map& map);

// Returns position of pixel in world coordinates
Vec2 findPixel(const Camera& camera, int w, int x);

// Returns first intersection of ray with wall
Vec2 cast(const Ray& ray, const Map& map);

// Move a point along a ray, until one of its coordinates is a whole number
void step(Vec2& position, const Vec2& direction);

// Returns true if given point touches a wall
bool intersects(const Vec2& point, const Map& map);

// Returns true if point is on map
bool isOnMap(const Vec2& point, const Map& map);

// Returns Euclidean distance between 2 points
double distance(const Vec2& p1, const Vec2& p2);

// Returns true if x is a whole number
bool isWhole(double x);


int main(int argc, char* argv[])
{
    if (!init())
    {
        return 1;
    }
    
    Map world =
    {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    Camera player =
    {
        {0, 3}, // viewpoint (middle of western edge)
        {1, 0}, // direction (east)
        {0, 1}  // imageSize (90 degree fov)
    };
    
    SDL_Surface* windowSurface = SDL_GetWindowSurface(gWindow);
    
    SDL_Delay(100); // so I don't render before window finishes initializing
    
    render(windowSurface, player, world);
    
    SDL_Delay(3000);
    
    close();
    return 0;
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


void render(SDL_Surface* surface, const Camera& camera, const Map& map)
{
    const int HORIZON = WINDOW_HEIGHT / 2;
    const int WALL_HEIGHT = HORIZON;
    
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format,
                                           SKY_COLOR.r,
                                           SKY_COLOR.g,
                                           SKY_COLOR.b)); // fill in the background
    
    // For each column of the image
    SDL_Rect column = {0, 0, 1, surface->h};
    for (int x = 0; x < surface->w; x++)
    {
        // Cast a ray
        Ray ray = {camera.viewpoint, findPixel(camera, surface->w, x) - camera.viewpoint};
        Vec2 intersection = cast(ray, map);

        // If the ray intersects a wall, draw it
        if (isOnMap(intersection, map))
        {
            // find visible height of wall
            double z = dot(intersection - camera.viewpoint, camera.direction)
                       / magnitude(camera.direction);
            column.y = HORIZON - WALL_HEIGHT/z;
            column.h = 2 * (HORIZON - column.y);
            
            // shade wall
            int shade = 0;
            if (isWhole(intersection.x))
            {
                shade += 32; // NS walls are shaded
            }
            
            shade += 30 * z; // fog
            
            SDL_Color shaded = WALL_COLOR;
            shaded.r = shaded.r > shade ? shaded.r - shade : 0;
            shaded.g = shaded.g > shade ? shaded.g - shade : 0;
            shaded.b = shaded.b > shade ? shaded.b - shade : 0;
            
            // draw wall
            column.x = x;
            SDL_FillRect(surface, &column, SDL_MapRGB(surface->format,
                                                      shaded.r,
                                                      shaded.g,
                                                      shaded.b));
        }
    }

    SDL_UpdateWindowSurface(gWindow);
}


Vec2 findPixel(const Camera& camera, int w, int x)
{
    // Origin to center of image
    Vec2 center = camera.viewpoint + camera.direction;
    
    // Center point to pixel
    Vec2 cameraX = ((2 * (float)x / (float)w) - 1) * camera.imageSize;

    return center + cameraX;
}


Vec2 cast(const Ray& ray, const Map& map)
{
    Vec2 position = ray.origin + ray.direction; // point on ray that we're checking
    
    // move along ray, until we hit a wall, or go off the map
    while (isOnMap(position, map) && !intersects(position, map))
    {
        step(position, ray.direction);
    }
    
    return position;
}


void step(Vec2& position, const Vec2& direction)
{
    double slope = direction.y / direction.x;
    
    // find position at next whole X
    Vec2 stepX;
    stepX.x = direction.x < 0 ? ceil(position.x - 1) : floor(position.x + 1);
    stepX.y = (stepX.x - position.x) * slope + position.y;
    
    // find position at next whole Y
    Vec2 stepY;
    stepY.y = direction.y < 0 ? ceil(position.y - 1) : floor(position.y + 1);
    stepY.x = (stepY.y - position.y) / slope + position.x;
    
    // move position to closer of the two
    position = distance(position, stepX) < distance(position, stepY) ? stepX : stepY;
}


// NOTE Can I combine this with step?
inline bool intersects(const Vec2& point, const Map& map)
{
    return isWhole(point.x) && (map[point.y][point.x-1] || map[point.y][point.x])
        || isWhole(point.y) && (map[point.y-1][point.x] || map[point.y][point.x]);
}


inline bool isOnMap(const Vec2& point, const Map& map)
{
    return point.x >= 0 && point.x < map[0].size() && point.y >= 0 && point.y < map.size();
}


inline double distance(const Vec2& p1, const Vec2& p2)
{
    return magnitude(p2-p1);
}


inline bool isWhole(double x)
{
    return x == (int)x;
}
