#include <ESP8266WiFi.h>
#include <Encoder.h>

const char* ssid = "bestWiFi";
const char* password = "panzer1235";
const byte ledPin = D7;
const byte soundSensorPin = D5;
const int maxValue = 1023;
boolean isLighted = false;


Encoder myEnc(D1, D2);
long value = 0;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  
  // prepare GPIO13
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void updateEncoderValue(int newValue) {
  if (newValue > 0) {
    value = newValue % (maxValue + 1);
  } else {
    value = 0;
  }
  isLighted = value > 0;
  myEnc.write(value);
  analogWrite(ledPin, value);
}

void loop() {

  long newPosition = myEnc.read();
  
  if (newPosition != value) {
    updateEncoderValue(newPosition);
    return;
  }
  if (!digitalRead(soundSensorPin)) {
    updateEncoderValue(maxValue + isLighted);
    delay(100);
    return;
  }
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  
  client.flush();
  
  // Match the request
  int val;
  int from = req.indexOf("light=");
  if (from != -1) {
    int to = req.indexOf("&");
    if (to == -1) to = req.length() - 1;
    String param = req.substring(from, to);
    param.replace("light=", "");
    val = param.toInt();
    updateEncoderValue(val);
  } else {
    client.stop();
    return;
  }

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += String(val);
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(10);
  
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
