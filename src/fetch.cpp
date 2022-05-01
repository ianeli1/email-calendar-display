#include "fetch.h"
#include "WiFiClientSecure.h"

char Fetch::HOST_NAME[] = "www.ianeli1.com";
char Fetch::METHOD[] = "GET";
char Fetch::PATH_NAME[] = "/api/hello";
WiFiClientSecure Fetch::client;
char Fetch::res[1024];
char Fetch::req[128];

struct FetchResponse Fetch::fetch()
{
    client.setInsecure();
    struct FetchResponse returnValue = {-1, NULL, true};
    if (!client.connect(HOST_NAME, 443))
    {
        return returnValue;
    }

    client.print(getRequest());

    if (!waitForBytes(5))
    {
        client.stop();
        return returnValue;
    }

    returnValue.length = getResponse();
    returnValue.res = res;
    returnValue.error = false;
    return returnValue;
}

char *Fetch::getRequest()
{
    snprintf(req, 128, "\
%s %s HTTP/1.1\n\
Host: %s\n\
Connection: close\n\n",
             METHOD, PATH_NAME, HOST_NAME);
    return req;
}

bool Fetch::waitForBytes(int timeout)
{
    int _timeout = millis() + (timeout * 1000);
    while (client.available() == 0)
    {
        if (_timeout - millis() <= 0)
        {
            return false;
        }
    }
    return true;
}

int Fetch::getResponse()
{
    int resCount = 0;

    while (client.available())
    {
        res[resCount] = client.read();
        resCount++;
        if (resCount == 1024)
        {
            break;
        }
    }
    client.stop();

    return resCount;
}