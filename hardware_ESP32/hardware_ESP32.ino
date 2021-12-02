#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "grove_two_rgb_led_matrix.h"

#include "icons.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "e30a2f5c-0a65-4473-996b-4f37bcf84137"
#define CHARACTERISTIC_UUID "89a81d4a-b71f-48ce-b653-46d59180826b"
#define CHARACTERISTIC_COLOR_UUID "9d3b9940-cf8a-467b-8362-d8aadfeede63"

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;
BLECharacteristic *pCharacteristicColor;

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define SERIAL SerialUSB
#else
#define SERIAL Serial
#endif

GroveTwoRGBLedMatrixClass matrix;

std::string prevValue = "Hello, World!";
int printColor = 0x33;

void printText( std::string newValue ) {

  matrix.displayFrames(ICON_check, 1000, true, 1);
  delay(1000);
  
  char *printVal = new char[newValue.length() + 1];
  strcpy(printVal, newValue.c_str());
  int printTime = newValue.length() * 500;
  matrix.displayString(printVal, printTime, false, 0xfe);
  delete [] printVal;
  delay(printTime);

  matrix.displayFrames(ICON_bt, 200, true, 1);
}

void printIcon( uint8_t icon[] ) {
  matrix.displayFrames(icon, 3000, false, 1);
  delay(3000);
  matrix.displayFrames(ICON_bt, 200, true, 1);
}

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
  
  // END MATRIX
  
  Serial.println("Starting BLE Server!");
  
  BLEDevice::init("ESP32_Matrix");
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);

  // Text
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue(prevValue);

  /*
  // Color
  pCharacteristicColor = pService->createCharacteristic(
                                         CHARACTERISTIC_COLOR_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristicColor->setValue(printColor);
  */
  
  pCharacteristic->setValue(prevValue);
  pService->start();
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  //pAdvertising->start();
  Serial.println("Characteristic defined! Now you can read it in the Client!");

  delay(500);
  printText( prevValue );
}

void loop()
{
  /*
  // change color if new color
  std::string color = pCharacteristicColor->getValue();
  if( stoi(color) != printColor ){
    printColor = stoi(color);  
  }
  */

  // display value if is new value
  std::string value = pCharacteristic->getValue();
  if( value != prevValue ){
    if( value == "smiley" ){
      printIcon(ICON_smiley);
    }else if( value == "heart" ){
      printIcon(ICON_heart);
    }else if( value == "rainbow" ){
      printIcon(ICON_rainbow);
    }else if( value == "blokdots" ){
      printIcon(ICON_blokdots);
    }else{
      printText( value );
    }
    Serial.print("The new characteristic value is: ");
    Serial.println(value.c_str());
    prevValue = value;
  }
  
  delay(1000);
}
