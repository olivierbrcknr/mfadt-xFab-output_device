#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "grove_two_rgb_led_matrix.h"

#include "icons.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "e30a2f5c-0a65-4473-996b-4f37bcf84137"
#define CHARACTERISTIC_UUID "89a81d4a-b71f-48ce-b653-46d59180826b"

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define SERIAL SerialUSB
#else
#define SERIAL Serial
#endif

#define DISPLAY_COLOR    0X11

GroveTwoRGBLedMatrixClass matrix;

std::string prevValue = "Hello, World!";

void setup() {
  
  Serial.begin(115200);

  // Starting Matrix
  Wire.begin();
  delay(1000); // Wait for Matrix ready
  uint16_t VID = 0;
  VID = matrix.getDeviceVID();
  if (VID != 0x2886) {
    Serial.println("Can not detect led matrix!!!");
    while (1);
  }
  Serial.println("Matrix init success!!!");
  matrix.displayFrames(ICON_bt, 200, true, 1);
  // END MATRIX
  
  Serial.println("Starting BLE Server!");
  
  BLEDevice::init("ESP32_Matrix");
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue(prevValue);
  pService->start();
  //BLEAdvertising *pAdvertising = pServer->getAdvertising();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  //pAdvertising->start();
  Serial.println("Characteristic defined! Now you can read it in the Client!");
}

void loop()
{
  std::string value = pCharacteristic->getValue();

  if( value != prevValue ){
    prevValue = value;
    matrix.displayFrames(ICON_check, 200, true, 1);
  }
  
  Serial.print("The new characteristic value is: ");
  Serial.println(value.c_str());
  delay(2000);
}
