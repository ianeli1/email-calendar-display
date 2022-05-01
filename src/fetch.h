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

    static struct FetchResponse fetch();

private:
    static char *getRequest();

    static int getResponse();

    static bool waitForBytes(int timeout);
};