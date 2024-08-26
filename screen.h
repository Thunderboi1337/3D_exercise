#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

class Screen
{
private:
    SDL_Event e;
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::vector<SDL_FPoint> points;

    TTF_Font *font;
    SDL_Color textColor;
    SDL_Surface *textSurface;
    SDL_Texture *textTexture;

public:
    Screen();
    ~Screen();

    void pixel(float x, float y);
    void show(void);
    void input();
    void clear();
};

Screen::Screen()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Create window and renderer
    if (SDL_CreateWindowAndRenderer(640 * 2, 480 * 2, 0, &window, &renderer) < 0)
    {
        std::cerr << "Window/Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    SDL_RenderSetScale(renderer, 2, 2);

    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        std::cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    // Load font
    font = TTF_OpenFont("FiraMono-Regular.ttf", 20);
    if (font == nullptr)
    {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    // Render text
    textColor = {255, 255, 255}; // White color
    textSurface = TTF_RenderText_Solid(font, " o ", textColor);
    if (textSurface == nullptr)
    {
        std::cerr << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    // Create texture from surface
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface); // Free the surface after creating texture
    if (textTexture == nullptr)
    {
        std::cerr << "Unable to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

Screen::~Screen()
{
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}

inline void Screen::pixel(float x, float y)
{
    points.emplace_back(x, y);
}

inline void Screen::show(void)
{
    // Get the width and height of the text
    int textWidth = 0, textHeight = 0;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

    SDL_FRect renderQuad;

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    // Render the text texture at each point
    for (const auto &point : points)
    {
        renderQuad = {point.x, point.y, static_cast<float>(textWidth), static_cast<float>(textHeight)};
        SDL_RenderCopyF(renderer, textTexture, nullptr, &renderQuad);
    }

    // Update the screen
    SDL_RenderPresent(renderer);
}

inline void Screen::input()
{
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            SDL_Quit();
            exit(0);
        }
    }
}

inline void Screen::clear()
{
    points.clear();
}