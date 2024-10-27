#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#define DEVICE_STATE_STRING_VPIN 
#define ID_VPIN 
#define NAME_VPIN 
#define STATE_VPIN 

#define NUM_DEVICES 8

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "Device.h"

Device devices[] = {
  Device(0, "", 0, D0),
  Device(1, "", 0, D1),
  Device(2, "", 0, D2),
  Device(3, "", 0, D3),
  Device(4, "", 0, D4),
  Device(5, "", 0, D5),
  Device(6, "", 0, D6),
  Device(7, "", 0, D7)
};

char ssid[] = "";
char pass[] = "";

uint8_t selectedID;
bool isStateVPinSync;

BLYNK_WRITE(DEVICE_STATE_STRING_VPIN) {
  String deviceStateString = param.asString();
  updateDevicesWith(deviceStateString);
}

BLYNK_WRITE(ID_VPIN) {
  selectedID = param.asInt();
  Blynk.virtualWrite(NAME_VPIN, getDeviceFrom(selectedID).getName());
  if (!isStateVPinSync) {
    bool state = getDeviceFrom(selectedID).getState();
    Blynk.virtualWrite(STATE_VPIN, state);
  } else {
    isStateVPinSync = false;
  }
}

BLYNK_WRITE(STATE_VPIN) {
  bool state = param.asInt();
  updateDeviceWith(selectedID, state);
  String deviceStateString = getDeviceStateString();
  Blynk.virtualWrite(DEVICE_STATE_STRING_VPIN, deviceStateString);
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(DEVICE_STATE_STRING_VPIN);
  Blynk.syncVirtual(ID_VPIN);
  isStateVPinSync = true;
  Blynk.syncVirtual(STATE_VPIN);
}

void setup() {
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  for (int i = 0; i < NUM_DEVICES; i++) {
    pinMode(devices[i].getPin(), OUTPUT);
  }
}

void loop() {
  Blynk.run();
}

void updateDevicesWith(String deviceStateString) {
  uint8_t id;
  uint8_t state;
  String tmp = "";

  for (int i = 0; i < deviceStateString.length(); i++) {
    if (deviceStateString[i] == ':') {
      id = tmp.toInt();
      tmp = "";
      continue;
    }

    if (deviceStateString[i] != ' ') {
      tmp += deviceStateString[i];
    }

    if (tmp == "ON" || tmp == "OFF") {
      state = tmp == "ON";
      updateDeviceWith(id, state);
      tmp = "";
    }
  }
}

void updateDeviceWith(uint8_t id, bool state) {
  for (int i = 0; i < NUM_DEVICES; i++) {
    if (id == devices[i].getID()) {
      digitalWrite(devices[i].getPin(), state);
      devices[i].setState(state);
      return;
    }
  }
}

Device getDeviceFrom(uint8_t id) {
  for (int i = 0; i < NUM_DEVICES; i++) {
    if (id == devices[i].getID()) {
      return devices[i];
    }
  }
  return devices[0];
}

String getDeviceStateString() {
  String deviceStateString = "";

  for (int i = 0; i < NUM_DEVICES; i++) {
    String id = (String)devices[i].getID();
    String state = devices[i].getState() ? "ON": "OFF";
    deviceStateString += id+":"+state+" ";
  }

  deviceStateString.remove(deviceStateString.length()-1);

  return deviceStateString;
}