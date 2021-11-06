/*
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

//const char* ssid = "Dialog 4G 799";
//const char* password = "2728e7D3";
const char* ssid = "Samsung Galaxy J2";
const char* password = "zurz415912";
int Elephant=0;
float longt=1833.62409;
float lat=00.00;
int PIR=8;
int LED=9;
int trigPin=10;
int echoPin1=11;
int echoPin2=12;
int distance;
int duration1;
int duration2;
//int Elephant=0;
char Str4[] = "lat=00.00 longt=00.00";

const char* mqtt_server = "test.mosquitto.org";
const char* outTopic = "Project/Pass1/Status";
const char* inTopic = "Project/Pass1/Call";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "hello world");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // Initialize the BUILTIN_LED pin as an output
  pinMode(BUILTIN_LED, OUTPUT); 
  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(trigPin,OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1,INPUT);// Sets the echoPin as an Input
  pinMode(echoPin2,INPUT);
  //Serial.begin(9600);    
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  long data= digitalRead(PIR);
  if(data==HIGH){
    digitalWrite(LED,HIGH);
    Serial.println(data);
    
            

    
   
  }
  else{
    digitalWrite(LED,LOW);
    Serial.println(data);
    
    
  }
  digitalWrite(trigPin, LOW); // Clears the trigPin
    delayMicroseconds(2); // Sets the trigPin on HIGH state for 10 micro seconds
 
    digitalWrite(trigPin, HIGH); // write the Trigger for MCU on Module 
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW); // Reads the echoPin, returns the sound wave travel time in microseconds
    duration1 = pulseIn(echoPin1, HIGH); // Using the Formula
  duration2 = pulseIn(echoPin2, HIGH); // Using the Formula
  if ((duration1<19200)&&(duration2<19200)){
    
    Elephant=1;
    distance= duration1*0.034/2;
    
    Serial.print("Distance: ");
    Serial.println(duration1);
  }
  else{
    Serial.println("Elephant Not DETECTED");
    Elephant=0;
  }
    








  

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "%f %f %d", lat,longt,Elephant);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(outTopic, msg);
  }
}
