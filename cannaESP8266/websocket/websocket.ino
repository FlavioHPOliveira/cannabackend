/*
  Esp8266 Websockets Client
  This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server
        3. Sends the websockets server a message ("Hello Server")
        4. Prints all incoming messages while the connection is open
  Hardware:
        For this sketch you only need an ESP8266 board.
  Created 15/02/2019
  By Gil Maimon
  https://github.com/gilmaimon/ArduinoWebsockets
*/

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <string>


/* Wifi connection */
const char* ssid = "2G_Netvirtua80"; //Enter SSID
const char* password = "34424595"; //Enter Password

/*Web Socket*/
const char* websockets_server_host = "192.168.0.213"; //Enter server adress
const uint16_t websockets_server_port = 3000; // Enter server port
using namespace websockets;
WebsocketsClient client;

/*Sensors*/
#define DHTPIN 5             // Digital pin connected to the DHT sensor. Code = 5, ESP8266 D1.
#define DHTTYPE DHT22        // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);
int soilPIN  = A0;           //Connect the soilMoisture output to analogue pin 1.
//DEFINE the VARIABLES for AIR and water to calibrate the soil mositure sensor
const int aire = 786;
const int agua = 377;       

void setup() {

    // Initialize the LED_BUILTIN pin as an output
    pinMode(LED_BUILTIN, OUTPUT);     
  
    Serial.begin(115200);
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
    }

    Serial.println("Connected to Wifi, Connecting to server.");
    // try to connect to Websockets server
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
    //bool connected = client.connect(websockets_connection_string);
    if(connected) {
        Serial.println("Connecetd!");
        client.send("Hello Server");
    } else {
        Serial.println("Not Connected!");
    }
    
    // run callback when messages are received
    client.onMessage([&](WebsocketsMessage message) {
        Serial.print("Got Message: ");
        Serial.println(message.data());

      // Ligamos/Desligamos o led de acordo com o comando
        if(message.data().equalsIgnoreCase("ON")){
            //digitalWrite(led, HIGH);
            // but actually the LED is on; this is because
            // it is active low on the ESP-01)
            digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
            Serial.println("Turned on!");
        }
            
        if(message.data().equalsIgnoreCase("OFF")){
            //digitalWrite(led, LOW);
            digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
            Serial.println("Turned Off!");
        }
        
    });

    /*Initialize temperature and humidity sensor.*/
    dht.begin();
}

void loop() {
    // let the websockets client check for incoming messages
    if(client.available()) {
        client.poll();
        String dht_temperature = String(dht.readTemperature());
        client.send(dht_temperature);
    }else {
        Serial.println("Client not available: ");
    }
    delay(500);
}
