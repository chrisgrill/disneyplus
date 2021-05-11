//
// Created by chris on 5/10/21.
//
#include <string>
#include "DisneyCurl.h"

// Taken from libcurl example
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = static_cast<char *>(realloc(mem->memory, mem->size + realsize + 1));
    if(!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

MemoryStruct DisneyCurl::Request(std::string url) {
    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;

    chunk.memory = static_cast<char *>(malloc(1));  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    //curl_global_init(CURL_GLOBAL_ALL);
    curl_global_init(CURL_GLOBAL_DEFAULT);
    /* init the curl session */
    curl_handle = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    curl_easy_setopt(curl_handle, CURLOPT_BUFFERSIZE, 102400L);
    //curl_easy_setopt(curl_handle, CURLOPT_URL, "https://cd-static.bamgrid.com/dp-117731241344/home.json");
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "curl_handle/7.58.0");
    curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl_handle, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(curl_handle, CURLOPT_FTP_SKIP_PASV_IP, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }
    else {
        /*
         * Now, our chunk.memory points to a memory block that is chunk.size
         * bytes big and contains the remote file.
         *
         * Do something nice with it!
         */

        printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();
    return chunk;
}

std::string DisneyCurl::GetJson(std::string url){
    struct MemoryStruct chunk = Request(url);
    std::string str(chunk.memory);
    free(chunk.memory);
    return str;
}

DisneyImage DisneyCurl::GetImage(std::string url){
    struct MemoryStruct chunk = Request(url);
    DisneyImage di((unsigned char*)chunk.memory, chunk.size);
    free(chunk.memory);
    return di;
}

