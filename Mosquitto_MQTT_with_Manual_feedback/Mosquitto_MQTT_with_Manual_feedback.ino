/*

  This is the code for controlling 4 Relays attached to ESP32/ESP8266 boards via MQTT.
  For Demo purpose I have used mosquitto MQTT broker on my Raspberry Pi Board.

  Code Written by - Sachin Soni
  for techiesms YouTube channel

  Visit techiesms on YouTube for deatiled project tutorials
  https://www.youtube.com/techiesms


*/

#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else
#error "Board not found"
#endif

#define DEBUG_SW 1

#include <PubSubClient.h>

//Relays for switching appliances
#define Relay1            15
#define Relay2            2
#define Relay3            4
#define Relay4            22

//Switches for manually control appliances
#define S1               32
#define S2               35
#define S3               34
#define S4               39


int switch_ON_Flag1_previous_I = 0;
int switch_ON_Flag2_previous_I = 0;
int switch_ON_Flag3_previous_I = 0;
int switch_ON_Flag4_previous_I = 0;

// Update these with values suitable for your network.

const char* ssid = "SSID";
const char* password = "PASS";
const char* mqtt_server = "XXX.XXX.XXX.XXX"; // Local IP address of Raspberry Pi

const char* username = "USERNAME";
const char* pass = "PASSWORD";


// Subscribed Topics

#define sub1 "device1/relay1"
#define sub2 "device1/relay2"
#define sub3 "device1/relay3"
#define sub4 "device1/relay4"

#define pub1 "device1/relay1/pub"
#define pub2 "device1/relay2/pub"
#define pub3 "device1/relay3/pub"
#define pub4 "device1/relay4/pub"

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


// Connecting to WiFi Router

void setup_wifi()
{

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

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");


  if (strstr(topic, sub1))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1')
    {
      digitalWrite(Relay1, LOW);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
    } else {
      digitalWrite(Relay1, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  }

  else if ( strstr(topic, sub2))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(Relay2, LOW);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
    } else {
      digitalWrite(Relay2, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  }
  else if ( strstr(topic, sub3))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(Relay3, LOW);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
    } else {
      digitalWrite(Relay3, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  }
  else if ( strstr(topic, sub4))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(Relay4, LOW);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
    } else {
      digitalWrite(Relay4, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  }

  else
  {
    Serial.println("unsubscribed topic");
  }

}


// Connecting to MQTT broker

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str() , username, pass)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(sub1);
      client.subscribe(sub2);
      client.subscribe(sub3);
      client.subscribe(sub4);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup()
{

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
  pinMode(S4, INPUT_PULLUP);

  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  manual_switches();
}

void manual_switches()
{

  if (digitalRead(S1) == LOW)
  {
    if (switch_ON_Flag1_previous_I == 0 )
    {
      digitalWrite(Relay1, HIGH);
      if (DEBUG_SW) Serial.println("Relay1- ON");
      client.publish(pub1, "1");
      switch_ON_Flag1_previous_I = 1;
    }


  }
  if (digitalRead(S1) == HIGH )
  {
    if (switch_ON_Flag1_previous_I == 1)
    {
      digitalWrite(Relay1, LOW);
      if (DEBUG_SW) Serial.println("Relay1 OFF");
      client.publish(pub1, "0");
      switch_ON_Flag1_previous_I = 0;
    }

  }


  if (digitalRead(S2) == LOW)
  {
    if (switch_ON_Flag2_previous_I == 0 )
    {
      digitalWrite(Relay2, HIGH);
      if (DEBUG_SW)  Serial.println("Relay2- ON");
      client.publish(pub2, "1");
      switch_ON_Flag2_previous_I = 1;
    }


  }
  if (digitalRead(S2) == HIGH )
  {
    if (switch_ON_Flag2_previous_I == 1)
    {
      digitalWrite(Relay2, LOW);
      if (DEBUG_SW) Serial.println("Relay2 OFF");
      client.publish(pub2, "0");
      switch_ON_Flag2_previous_I = 0;
    }

  }

  if (digitalRead(S3) == LOW)
  {
    if (switch_ON_Flag3_previous_I == 0 )
    {
      digitalWrite(Relay3, HIGH);
      if (DEBUG_SW) Serial.println("Relay3- ON");
      client.publish(pub3, "1");
      switch_ON_Flag3_previous_I = 1;
    }


  }
  if (digitalRead(S3) == HIGH )
  {
    if (switch_ON_Flag3_previous_I == 1)
    {
      digitalWrite(Relay3, LOW);
      if (DEBUG_SW) Serial.println("Relay3 OFF");
      client.publish(pub3, "0");
      switch_ON_Flag3_previous_I = 0;
    }


  }

  if (digitalRead(S4) == LOW)
  {
    if (switch_ON_Flag4_previous_I == 0 )
    {
      digitalWrite(Relay4, HIGH);
      if (DEBUG_SW) Serial.println("Relay4- ON");
      client.publish(pub4, "1");
      switch_ON_Flag4_previous_I = 1;
    }


  }
  if (digitalRead(S4) == HIGH )
  {
    if (switch_ON_Flag4_previous_I == 1)
    {
      digitalWrite(Relay4, LOW);
      if (DEBUG_SW) Serial.println("Relay4 OFF");
      client.publish(pub4, "0");
      switch_ON_Flag4_previous_I = 0;
    }

  }



}
