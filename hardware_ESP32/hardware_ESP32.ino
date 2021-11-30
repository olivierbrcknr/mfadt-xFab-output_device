#include "BluetoothSerial.h"
#include "grove_two_rgb_led_matrix.h"

#include "icons.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define SERIAL SerialUSB
#else
#define SERIAL Serial
#endif

#define DISPLAY_COLOR    0X11

GroveTwoRGBLedMatrixClass matrix;

void setup() {

  Serial.begin(115200);

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
  
  SerialBT.begin("ESP_Matrix"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
}
