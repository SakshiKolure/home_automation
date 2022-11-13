#include<ESP8266WiFi.h>
#include"Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"

/***** pin defination *****/
//relay for switch appliances
#define relay1  D0
#define relay2  D1

//DHT11 for reading temperature and humidity value

//buzzer to know the status of MQTT connections and can be used for any other purpose according to your project need.

//selection pins for multiplexer module to switch between different sensors and give data on a single analog pin.

//analog pin to read the incoming analog value for different sensors.
#DHTPIN D2
/*****   wifi acess point    *****/
#define WLAN_SSID  "lenovo"
#define WLAN_PASS   "12345678"

/*******   Adafruit.io setup   *****/

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT "8883"      //use 8883 for SSL
#define AIO SERVER "sakshi17"
#define AIO_KEY "df5af5dd653f47948671c50552bec432"

/******  global state *****/

//create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
//or... use Wificlientsecure for SSL
//WiFiclient secure client;

//setup the MQTT client by passing in the wifi client and MQTT server and login details.
adafruit_MQTT_Client mqtt(&client, AIO_SERVER,AIO_SERVERPORT, AIO_USERNAME,AIO_KEY);

/***** FEED*****/

//Notice MQTT path for AIO follow the form:<username>/feeds/<feedname>
Adafruit_MQTT_Publish_humidity=Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"feeds/temperature");

//setup a feed called "on off" for subscribing for changes.
Adafruit_MQTT_Subscribe food=Adafruit_MQTT_Subscribe(&mqtt,AIO_USERNAME"feeds/fan");
Adafruit_MQTT_Subscribe food=Adafruit_MQTT_Subscribe(&mqtt,AIO_USERNAME"feeds/lamp");

/*****   necessary declaration for DHT11    *****/
/SKETCH CODE
//Bug work around for arduino 1.6.6,it seems to need a function declaration 
//for some reason(only affects ESP8266,likely an arduino-builder bug).

void MQTT_connects();
void setup() {
    Serial.begin(115200);
    delay(10);

    pinMode(Relay1, OUTPUT);
    pinMode(Relay2, OUTPUT);
    pinMode(D4, INPUT);

    Serial.println(F(" Adafruit MQTT demo"));

    //Connect to WIfi acces point.
    Serial.println();Serial.println();
    Serial.println("Cennecting to ");
    Serial.println(WLAN_SSID);

    Wifi.begin(WLAN_SSID, WLAN_PASS);
    while (wifi.status()!= WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.println("wifi connected");
    Serial.println("IP address:")Serial.println(WiFi.localIP());

    //Setting up DHT sensor
    dht.begin();

    //Setup MQTT subscription for onoff feed.
    mqtt.subscribe(&food);
    mqtt.subscribe(&water);
}
uint32_t x =0;

void loop() {
    //ensure the connection to the MQTT server is alive (this will make the first 
    // connection and automatically reconnect when disconnected.) See the MQTT_connect 
    //function  defination further below.
    MQTT_connect();
    //this is our 'wait for incoming subscription packets' busy subloop 
    //try to spend your time here 

    Adafruit_MQTT_Subscribe*subscription;

    while((subscription = mqtt.readSubscription(20000))) {
        if (subscription  == &food ){
            Serial.print(F("Got: "));
            Serial.println(( char *) food.lastread);
            digitalwrite( Relay1, food_State);
        }

    }
    // ping the server to keep th mqtt connection alive 
    //NOT requried if you are publishimg once every KEEPALIVE seconds
    /*if ( !mqtt.ping()) {
        mqtt.disconnect();
     }
     */
} 
//Function to connect and reconnect as neccessary to th MQTT server.
//Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
    int8_t ret;

    //stop if already connected.
    if(mqtt.connected()) {
        return;
    }

    Serial.print(" connecting to MQTT... ");
    uint8_t retries = 3;
    while((ret = mqtt.connect()) != 0) { // connect will return 0 for connected Serial.println(mqtt.connectErrorString( ret));
    Serial.println(" Retrying MQTT connection in 5 seconds..");
    mqtt.disconnect();
    delay(5000); //wait 5 seconds
    retries--;
    if ( retries == 0){
    //basically die and wait for WDT to reset me while (1);
    }
}
Serial.println("MQTT Connected!");
}


}