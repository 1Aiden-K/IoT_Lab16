/*
 * Project IoT_Lab16
 * Description:
 * Author:
 * Date:
 */

#include "oled-wing-adafruit.h"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);

int sent = 0;
bool dc = false;

LEDStatus status;
OledWingAdafruit display;

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context)
{
  for (size_t i = 0; i < len; i++)
  {
    if (data[i] == 49)
    {
      display.setCursor(0, 0);
      display.clearDisplay();
      sent = sent + 1;
      display.println("1 was sent.");
      display.println(String(sent) + " times.");
      display.display();
    }
    else if (data[i] == 48)
    {
      dc = true;
    }
  }
}

const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);
const size_t UART_TX_BUF_SIZE = 20;

void setup()
{
  BLE.on();

  status.setActive();

  BLE.addCharacteristic(txCharacteristic);
  BLE.addCharacteristic(rxCharacteristic);

  BleAdvertisingData data;
  data.appendServiceUUID(serviceUuid);
  data.appendLocalName("Jesus");
  BLE.advertise(&data);

  status.setColor(RGB_COLOR_YELLOW);

  display.setup();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
}

void loop()
{
  display.loop();
  if (BLE.connected())
  {
    status.setColor(RGB_COLOR_BLUE);

    if (dc)
    {
      BLE.disconnect();
      dc = false;
    }
  }
  else
  {
    status.setColor(RGB_COLOR_YELLOW);
  }
}