#include "fetch.h"
#include "WiFiClientSecure.h"
#include "ArduinoJson.h"
#include "setupWifi.h"

char Fetch::HOST_NAME[] = "email-calendar-display.vercel.app";
char Fetch::METHOD[] = "GET";
char Fetch::PATH_NAME[] = "/api/getData";
WiFiClientSecure Fetch::client;
char Fetch::res[1024];
char Fetch::req[128];
char Fetch::body[512];

struct FetchResponse Fetch::fetch()
{
    client.setInsecure();
    client.setHandshakeTimeout(30);
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
    jsonDeserialized = false;
    return returnValue;
}

char *Fetch::getBody()
{
    int counter = response_length - 1;
    while (counter > 0 && res[counter] != '\n')
    {
        counter--;
    }
    for (int i = counter; i < response_length; i++)
    {
        body[i - counter] = res[i];
    }
    body[response_length - counter - 1] = 0;
    return body;
}

struct ApiResponse Fetch::getJson()
{
    struct ApiResponse res = {0, NULL, NULL, NULL, NULL, true};
    if (!jsonDeserialized)
    {

        DeserializationError error = deserializeJson(json, getBody());
        if (error)
        {
            return res;
        }
    }

    res.unread = json["unread"].as<int>();
    res.subject = json["subject"];
    res.from = json["from"];
    res.date = json["date"];
    res.time = json["time"];
    res.error = false;
    jsonDeserialized = true;
    return res;
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
    response_length = 0;

    while (client.available())
    {
        res[response_length] = client.read();
        response_length++;
        if (response_length == 1024)
        {
            break;
        }
    }
    client.stop();

    return response_length;
}

int Fetch::response_length = 0;
StaticJsonDocument<256> Fetch::json;
bool Fetch::jsonDeserialized = false;