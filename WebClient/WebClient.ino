/*
 WiFiEsp example: WebClient

 This sketch connects to google website using an ESP8266 module to
 perform a simple web search.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
//SoftwareSerial Serial1(2, 3); // RX, TX
//SoftwareSerial Serial1(50, 52); // RX, TX
#endif

char ssid[] = "AndroidAP";            // your network SSID (name)
char pass[] = "22222222";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int method;///send or receive data
String data;///data to send to the database
char server[] = "192.168.43.132";///aparche server pc ip

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
   Serial1.begin(9600);///for uno
 // Serial1.begin(115200);////changed for the arduino mega
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  printWifiStatus();

  Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    Serial.println("press 1 to send data press 2 to get data");
    while(Serial.available()==0){}////wait for input
    method=Serial.parseInt();

    if(method==1){
    // Make a HTTP request to send data
    Serial.println("Enter the data to send");
    while(Serial.available()==0){}
    data=Serial.readString();///read the input
     ////to send the all data for loop goes here
      
    client.print("GET /info.php?");
    client.print("request=");
    client.print(data);
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.43.132");
    client.println("Connection: close");
    client.println();
    }else if(method==2){
     //Make a HTTP request to get data 
     client.println("GET /tryjson.php HTTP/1.1");
     client.println("Host: 192.168.43.132");
     client.println("Connection: close");
     client.println();
    }
  }
}

void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()) {
    char c = client.read();
   Serial.write(c);
   
   
  }

  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();

    // do nothing forevermore
    while (true);
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
