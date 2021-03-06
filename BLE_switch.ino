/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "esp_pm.h"
#include "Server_Callbacks.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

esp_pm_config_esp32_t pm_config = {
    .max_cpu_freq = RTC_CPU_FREQ_2M,
    .max_freq_mhz = 2,
    .min_cpu_freq = RTC_CPU_FREQ_XTAL,
    .min_freq_mhz = 2,
    .light_sleep_enable = true
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  pinMode(SWITCH_PIN, OUTPUT);

  BLEDevice::init("CHARGE CONTROLLER");                                // Creates visible name of device
  BLEServer *pServer = BLEDevice::createServer();                      // Sets the deivice as a server
  BLEService *pService = pServer->createService(SERVICE_UUID);         // Creates a service
  BLECharacteristic *pCharacteristic = pService->createCharacteristic( // Creates a characteristic in that service with the UUID and Properties
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("I want to keep your battery healthy");
  pCharacteristic->setCallbacks(new ServerCallbacks());
  pService->start();

  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  esp_pm_configure(&pm_config);
};

void loop()
{
  // put your main code here, to run repeatedly:
  delay(2000);
};
