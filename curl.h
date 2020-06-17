#ifndef CURL_H
#define CURL_H
#include <curl/curl.h>

CURLcode post_to_webhook(char* dest_url);

#endif
