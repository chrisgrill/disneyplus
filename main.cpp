#include <SDL.h>
#include <curl/curl.h>
#include "DisneyCurl.h"
#include "json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

int main(int argc, char ** argv)
{

    bool quit = false;
    SDL_Event event;
    int columns = 4;
    DisneyImage di = DisneyCurl::GetImage("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/0A9ABA4D36CEDF49052535C075C6D05D089CAC7B7C497D5FEC6E53E015506484/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
                                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, di.surf);

    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
        }
        SDL_Rect dstrect = { 5, 5, 250, 109 };
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        //SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    /*------------------------------------------------------------------------------------------------------------*/



    std::string home_json = DisneyCurl::GetJson("https://cd-static.bamgrid.com/dp-117731241344/home.json");
    json j = json::parse(home_json);
    curl_global_cleanup();
    return 0;
}
