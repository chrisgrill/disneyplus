//
// Created by chris on 5/10/21.
//

#ifndef DISNEYPLUS_DISNEYCURL_H
#define DISNEYPLUS_DISNEYCURL_H
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <curl/curl.h>
#include "DisneyImage.h"

struct MemoryStruct {
    char *memory;
    size_t size;
};

class DisneyCurl{
public:
    static std::string GetJson(std::string url);
    static std::shared_ptr<DisneyImage> GetImage(std::string url);
private:
    static MemoryStruct Request(std::string url);

};
#endif //DISNEYPLUS_DISNEYCURL_H
