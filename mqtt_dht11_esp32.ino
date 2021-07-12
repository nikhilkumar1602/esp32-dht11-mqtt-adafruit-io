#include<WiFi.h>             
#include "Adafruit_MQTT.h"                     
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"                                

#define DHT11PIN 16                               
#define DHTTYPE DHT11                          
                 
DHT dht(DHT11PIN, DHT11);


#define WLAN_SSID       "My Zone"                 
#define WLAN_PASS       "sreevani"             
#define AIO_SERVER      "io.adafruit.com"     
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "Nikhilkumar1602"              
#define AIO_KEY         "aio_iRLe12ZHoeTjeLFQOmRdkpJ1eIwi"  


WiFiClient espclient;
Adafruit_MQTT_Client mqtt(&espclient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature1");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity1");

void MQTT_connect();
void setup() {

  Serial.begin(115200);                  
  delay(10);
  dht.begin();                      
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  Serial.println();
  Serial.println("WiFi connected");
    Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());

}
void loop() {

  MQTT_connect();
  dht.begin();
  float temperature_data = dht.readTemperature();     
  float humidity_data = dht.readHumidity();           
  Serial.print(F("Sending temperature value "));
  Serial.print(temperature_data);
  Serial.print("...");

  if (! temp.publish(temperature_data)) 
  {
    Serial.println(F("Failed to publish the temperature data"));
    } 
    Serial.print(F("Sending Humidity value "));
    Serial.print(humidity_data);
    Serial.print("...");
    if (! humidity.publish(humidity_data)) 
    {
      Serial.println(F("Failed to publish the Humidity data"));
      } 
      delay(10000);
}

 void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0) {    
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying to establish MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);              
       retries--;
       if (retries == 0) {
        
         while (1);
       }
  }
  Serial.println("MQTT Connected!");

}
