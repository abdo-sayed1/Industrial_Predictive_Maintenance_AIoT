#include "wifi.h"
const char* ssid = SSID;
const char* pass = PASSWORD;
#if SERVERCLIENTMODE == SERVER_M
WiFiServer server(80);
#endif
void wifiSetup()
{
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) 
    {
    delay(500);
    Serial.print(".");
    }
#if SERVERCLIENTMODE == SERVER_M
    Serial.println("WiFi connected (Server Mode)");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
#else
    Serial.println("WiFi connected (Client Mode)");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
#endif
}
String wifi_receive()
{
    String msg = "";
    int i=0;
    #if SERVERCLIENTMODE == SERVER_M
    WiFiClient client = server.available();
    if (client) 
    {
        Serial.println("New Client.");
        String currentLine = "";
        while (client.connected()) 
        {
            if (client.available()) 
            {
                char c = client.read();
                msg+=c;
                Serial.write(c);
                if (c == '\n') 
                {
                    if (currentLine.length() == 0) 
                    {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();
                        client.println("<!DOCTYPE HTML><html><body><h1>Hello from ESP32 Server</h1></body></html>");
                        client.println();
                        break;
                    } 
                    else 
                    {
                        currentLine = "";
                    }
                } 
                else if (c != '\r') 
                {
                    currentLine += c;
                }
            }
            i++;
        }
        client.stop();
        Serial.println("Client Disconnected.");
    }
    #else 
    if (WiFi.status() == WL_CONNECTED) 
    {
        WiFiClient client;
        const char* host = "example.com";
        if (client.connect(host, 80)) 
        {
            client.println("GET / HTTP/1.1");
            client.println("Host: example.com");
            client.println("Connection: close");
            client.println();
            while (client.connected() || client.available()) 
            {
                if (client.available()) 
                {
                    char c = client.read();
                    msg+=c;
                    Serial.write(c);
                }
                i++;
            }
            client.stop();
            Serial.println("Disconnected from server.");
        } 
        else 
        {
            Serial.println("Connection to server failed.");
        }
    }
    #endif
    //Receive data from client or server and store in msg
    return msg;
}
void wifi_transmit(char* str)
{    
    #if SERVERCLIENTMODE == SERVER_M
        // In Server Mode, we wait for a client to connect to send data
        WiFiClient client = server.available();
        if (client) {
            if (client.connected()) {
                client.print((char*)str);
                Serial.println("Data transmitted to client.");
            }
            client.stop();
        } else {
            Serial.println("No client connected to transmit to.");
        }
    #else 
        // In Client Mode, we connect to the server and send the data
        if (WiFi.status() == WL_CONNECTED) {
            WiFiClient client;
            const char* host = "example.com"; // Ensure this matches your wifireceive host
            if (client.connect(host, 80)) {
                client.print((char*)str); 
                // Adding a small delay or a println to ensure the buffer sends
                client.println(); 
                Serial.println("Data transmitted to server.");
                client.stop();
            } else {
                Serial.println("Transmission failed: Could not connect to host.");
            }
        }
    #endif
}