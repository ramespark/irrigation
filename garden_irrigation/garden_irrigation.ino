#include <SPI.h>
#include <ESP8266WiFi.h>



const char* ssid     = "Charm";
const char* password = "samplekey";

// sensor/led pins
#define SENSOR_PIN A0
#define RELAY1 D0


char thingSpeakAddress[] = "api.thingspeak.com";
const char* host = "api.thingspeak.com";
String writeAPIKey = "6CVQ3BZV0DATFKN8";

void setup() {
  Serial.begin(9600);
   pinMode(RELAY1, OUTPUT);   
   
   delay(100);
   
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  //Turn-off the relay so that it does not consume more power.
  digitalWrite(RELAY1,1); 
   //Check once in every 10 minutes. This saves battery
  delay(5000*12*10);
    
  int sensorValue = analogRead(SENSOR_PIN);
  
  Serial.println(sensorValue);
  String tsData = "field1=";
  Serial.println(tsData);
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/update";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
  client.print("Content-Type: application/x-www-form-urlencoded\n");
  client.print("Content-Length: ");
  if(sensorValue<1000)
    client.print(tsData.length()+3);
  else
    client.print(tsData.length()+4);
  client.print("\n\n");
  client.print(tsData);
  client.print(sensorValue);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");

  //Water for arround 15 sec. This dispatches arround 1 ltr of water based on the DC motor used.
  if(sensorValue < 600){
    digitalWrite(RELAY1,0);    
    delay(15000);
    digitalWrite(RELAY1,1); 
  }
}
