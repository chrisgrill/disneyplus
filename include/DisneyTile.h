//
// Created by chris on 5/13/21.
//

#ifndef DISNEYPLUS_DISNEYTILE_H
#define DISNEYPLUS_DISNEYTILE_H
#include <memory>
#include "DisneyImage.h"

class DisneyTile {

public:
    std::shared_ptr<DisneyImage> image;
    const std::string background_url;
    const std::string rating;
    const std::string title;
    DisneyTile(std::shared_ptr<DisneyImage> image, std::string background_url, std::string rating, std::string title):
            image(image), background_url(background_url), rating(rating), title(title){};
};


#endif //DISNEYPLUS_DISNEYTILE_H
