#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "./curl.h"

void post_to_webhook(char* dest_url){

  CURL *curl;
  CURLcode res_code;

  // sets up program environment that libcurl needs
  curl_global_init(CURL_GLOBAL_ALL);

  // obtain curl handle
  curl = curl_easy_init();
  if(curl != NULL) {
    // set destination for curl
    curl_easy_setopt(curl, CURLOPT_URL, dest_url);

    // perform request
    printf("Sending POST request to \"%s\". Response:\n    ", dest_url);
    res_code = curl_easy_perform(curl);
    printf("\n");

    // checks for errors
    if(res_code != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res_code));

    // frees curl handle created by curl_easy_init
    curl_easy_cleanup(curl);
  } else {
    perror("Failed to initliase curl");
    exit(EXIT_FAILURE);
  }
  // frees program environment created by curl_global_init
  curl_global_cleanup();
}
