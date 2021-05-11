//
// Created by chris on 5/10/21.
//
#include "DisneyImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
DisneyImage::DisneyImage(std::string filename) {
    int req_format = STBI_rgb_alpha;
    int width, height, orig_format;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &orig_format, req_format);
    int depth, pitch;
    Uint32 pixel_format;
    if (req_format == STBI_rgb) {
        depth = 24;
        pitch = 3*width; // 3 bytes per pixel * pixels per row
        pixel_format = SDL_PIXELFORMAT_RGB24;
    } else { // STBI_rgb_alpha (RGBA)
        depth = 32;
        pitch = 4*width;
        pixel_format = SDL_PIXELFORMAT_RGBA32;
    }

    surf = SDL_CreateRGBSurfaceWithFormatFrom((void*)data, width, height,
                                                           depth, pitch, pixel_format);
}

DisneyImage::DisneyImage(unsigned char* data, int len) {
    int req_format = STBI_rgb_alpha;
    int width, height, orig_format;
    //unsigned char* data = stbi_load(filename.c_str(), &width, &height, &orig_format, req_format);
    unsigned char* image = stbi_load_from_memory(data, len, &width, &height, &orig_format, req_format);
    int depth, pitch;
    Uint32 pixel_format;
    if (req_format == STBI_rgb) {
        depth = 24;
        pitch = 3*width; // 3 bytes per pixel * pixels per row
        pixel_format = SDL_PIXELFORMAT_RGB24;
    } else { // STBI_rgb_alpha (RGBA)
        depth = 32;
        pitch = 4*width;
        pixel_format = SDL_PIXELFORMAT_RGBA32;
    }

    surf = SDL_CreateRGBSurfaceWithFormatFrom((void*)image, width, height,
                                              depth, pitch, pixel_format);
}

DisneyImage::~DisneyImage() {
    SDL_FreeSurface(surf);
//    SDL_DestroyTexture(texture);
//    SDL_DestroyRenderer(renderer);
}