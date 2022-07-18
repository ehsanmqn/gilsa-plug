#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "config.h"

// Set web server and wifi client
WiFiServer server(80);
WiFiClient wifiClient;
WiFiManager wifiManager;
PubSubClient client(wifiClient);

// Definition of public variables
int longPressCounter = 0;
int lightFlashCounter = 0;
int whiteFlashStatus = 0;
int reconnectCounter = CHECK_CONNETCTION_ITERATIONS;
int rolloutCounter = 0;
int leftButtonState, midButtonState, rightButtonState;
bool isMidPressed = false;
bool isRightPressed = false;
bool isLeftPressed = false;
bool isConnected = false;

void setup() {
  // Setup pins
  pinMode(midSwitchRelay, OUTPUT);
  pinMode(linkLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  pinMode(midSwitchInput, INPUT);

  delay(10);

  digitalWrite(linkLed, LOW);
  digitalWrite(redLed, HIGH);
  digitalWrite(midSwitchRelay, LOW);

  // Setup serial
  if (serialEnabled)
  {
    Serial.begin(serialBuadrate);
  }

  Serial.println("[ INFO ] Init WiFi");


  String mac = WiFi.macAddress();
  mac.replace(":","");
  mac += String(ESP.getChipId());
  mac.toCharArray(TOKEN, 20);

  // Add token field to the portal
  WiFiManagerParameter device_token_field("token", "Device credential", TOKEN, 20);

  wifiManager.addParameter(&device_token_field);
  wifiManager.setConfigPortalTimeout(configPortalTimeout);
  wifiManager.setDebugOutput(false);
  // wifiManager.setSaveParamsCallback(saveParamsCallback);

  //automatically connect using saved credentials if they exist
  //If connection fails it starts an access point with the specified name
  if (autoGenerateSSID) {
    // Use this for auto generated name ESP + ChipID
    wifiManager.autoConnect();
  } else {
    wifiManager.autoConnect(DEVICE_NAME);
  }

  // if you get here you have connected to the WiFi
  Serial.println("[ INFO ] Connected to WiFi: ");

  // Read token from EEPROM
  // if(!statictoken) {
  //     EEPROM.get(0, TOKEN);
  // }

  // Connect to the server
  server.begin();
  client.setServer(dmiotServer, mqttPort);
  client.setCallback(on_message);
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("[ INFO ] Entered config mode");
  Serial.println(WiFi.softAPIP());

  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void saveParamsCallback () {
  // Save token into EEPROM
  // if(!statictoken){
  //   strcpy(TOKEN, device_token_field.getValue());
  //   EEPROM.put(0, TOKEN);
  //   EEPROM.commit();
  // }
}

void startConfigPortal() {
  delay(3000);
  wifiManager.erase();
  wifiManager.resetSettings();
  ESP.restart();
}

void loop() {
  // ============================ Middle button =================================
  midButtonState = digitalRead(midSwitchInput);

  if (midButtonState == HIGH)
    isMidPressed = false;
  else {
    // This part of code is for detecting long press
    if ( midButtonState == LOW ) {
      longPressCounter++;
    }
    else {
      longPressCounter = 0;
    }

    if (longPressCounter == LONG_PRESS_ITERATIONS) {
      Serial.println("[ INFO ] Long press detected, starting Config Portal");

      startConfigPortal();

      longPressCounter = 0;
    }
  }

  if (midButtonState == LOW && !isMidPressed) {
    isMidPressed = true;
    if (gpioState[0] == false)
    {
      digitalWrite(midSwitchRelay, HIGH);
      digitalWrite(linkLed, HIGH);
      digitalWrite(redLed, LOW);

      // Update GPIOs state
      gpioState[0] = true;
    }
    else {
      digitalWrite(midSwitchRelay, LOW);
      digitalWrite(linkLed, LOW);
      digitalWrite(redLed, HIGH);

      // Update GPIOs state
      gpioState[0] = false;
    }
    client.publish("v1/devices/me/attributes", get_gpio_status().c_str());
  }

  // // ============================ Blink LED =================================
  // if (lightFlashCounter >= CONNECTION_BLINK_ITERATIONS && !isConnected) {
  //   if (whiteFlashStatus == 0 )
  //   {
  //     // Turn the light flash on
  //     whiteFlashStatus = 1;
  //   }
  //   else
  //   {
  //     // Turn the light flash off
  //     whiteFlashStatus = 0;
  //   }
  //
  //   lightFlashCounter = 0;
  // }

  // Check for connection
  if (reconnectCounter >= CHECK_CONNETCTION_ITERATIONS) {
    if ( !client.connected() )
    {
      reconnect();
    }

    reconnectCounter = 0;
  }

  client.loop();
  reconnectCounter++;
  lightFlashCounter++;

  delay(LOOP_SLEEP_TIME);
}

// The callback for when a PUBLISH message is received from the server.
void on_message(const char* topic, byte* payload, unsigned int length) {

  Serial.println("On message");

  char json[length + 1];
  strncpy (json, (char*)payload, length);
  json[length] = '\0';

  Serial.print("[ INFO ] Topic: ");
  Serial.println(topic);
  Serial.print("[ INFO ] Message: ");
  Serial.println(json);

  // Decode JSON request
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject((char*)json);

  if (!data.success())
  {
    Serial.println("[ EROR ] parseObject() failed");
    return;
  }

  // Check request method
  String methodName = String((const char*)data["method"]);

  if (methodName.equals("getGpioStatus")) {

    // Reply with GPIO status
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
  } else if (methodName.equals("setGpioStatus")) {
    // Update GPIO status and reply
    set_gpio_status(data["params"]["pin"], data["params"]["enabled"]);

    String responseTopic = String(topic);

    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
    client.publish("v1/devices/me/attributes", get_gpio_status().c_str());
  }
}

// Get GPIO RPC
String get_gpio_status() {

  // Prepare gpios JSON payload string
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  data[String(1)] = gpioState[0] ? true : false;
  data[String(2)] = gpioState[1] ? true : false;
  data[String(3)] = gpioState[2] ? true : false;

  char payload[256];
  data.printTo(payload, sizeof(payload));
  String strPayload = String(payload);
  Serial.print("Get gpio status: ");
  Serial.println(strPayload);
  return strPayload;
}

// Set GPIO RPC
void set_gpio_status(int pole, boolean enabled) {
  if (pole == 1) {
    if (enabled)
    {
      digitalWrite(midSwitchRelay, HIGH);
      digitalWrite(linkLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else
    {
      digitalWrite(midSwitchRelay, LOW);
      digitalWrite(linkLed, LOW);
      digitalWrite(redLed, HIGH);
    }

    // Update GPIOs state
    gpioState[0] = enabled;
  }

  // if (pole == 2) {
  //   if (enabled)
  //   {
  //     digitalWrite(leftSwitchRelay, HIGH);
  //   }
  //   else
  //   {
  //     digitalWrite(leftSwitchRelay, LOW);
  //   }
  //
  //   // Update GPIOs state
  //   gpioState[1] = enabled;
  // }

  // if (pole == 3) {
  //   if (enabled)
  //   {
  //     digitalWrite(rightSwitchRelay, HIGH);
  //   }
  //   else
  //   {
  //     digitalWrite(rightSwitchRelay, LOW);
  //   }
  //
  //   // Update GPIOs state
  //   gpioState[2] = enabled;
  // }
}

// Reconnect to the server function
void reconnect() {
  // Connecting to the server
  Serial.println("[ INFO ] Connecting to DMIoT server ...");

  // Attempt to connect (clientId, username, password)
  if ( client.connect(DEVICE_NAME, TOKEN, NULL) ) {
    Serial.println( "[ INFO ] Connected to the server" );

    // digitalWrite(linkLed, LOW);
    isConnected = true;

    // Subscribing to receive RPC requests
    client.subscribe("v1/devices/me/rpc/request/+");

    // Sending current GPIO status
    Serial.println("[ INFO ] Sending current GPIO status ...");
    client.publish("v1/devices/me/attributes", get_gpio_status().c_str());
  } else {
    isConnected = false;
    Serial.print( "[FAILED] [ rc = " );
    Serial.print( client.state() );
    Serial.println( " : retrying in 5 seconds]" );
  }
}
