#pragma once
#include "WiFiClientSecure.h"

struct FetchResponse
{
    int length;
    char *res;
    bool error;
};

class Fetch
{
public:
    static char HOST_NAME[];
    static char METHOD[];
    static char PATH_NAME[];
    static WiFiClientSecure client;
    static char res[1024];
    static char req[128];
    static char body[512];

    static struct FetchResponse fetch();
    static char *getBody();

private:
    static int response_length;
    static char *getRequest();

    static int getResponse();

    static bool waitForBytes(int timeout);
};