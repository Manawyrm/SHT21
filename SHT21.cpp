/* 
 * This library is based on the Adafruit SHT31 library by Limor Fried/Ladyada (Adafruit Industries): 
 * https://github.com/adafruit/Adafruit_SHT31/
 * 
 * Modified for SHT21 by Tobias Mädel <t.maedel@alfeld.de> / @manawyrm
 * 
 * BSD license applies
 */

#include "SHT21.h"

/*!
 * @brief  SHT21 constructor using i2c
 * @param  *theWire
 *         optional wire
 */
SHT21::SHT21(TwoWire *theWire) {
  _wire = theWire;
  _i2caddr = 0;

  humidity = 255;
  temp = 65535;
}

bool SHT21::begin(uint8_t i2caddr) {
  _wire->begin();
  _i2caddr = i2caddr;
  //reset();
  return 1;
}

void SHT21::reset(void) {
  writeCommand(SHT21_SOFTRESET);
  delay(15);
}

void SHT21::heater(bool h) {
  //if (h)
    //writeCommand(SHT21_HEATEREN);
  //else
    //writeCommand(SHT21_HEATERDIS);
}

int16_t SHT21::readTemperature(void) {
  return temp;
}

uint8_t SHT21::readHumidity(void) {
  return humidity;
}

/**
 * Performs a CRC8 calculation on the supplied values.
 *
 * @param data  Pointer to the data to use when calculating the CRC8.
 * @param len   The number of bytes in 'data'.
 *
 * @return The computed CRC8 value.
 */
static uint8_t crc8(const uint8_t *data, int len) {
  /*
   *
   * CRC-8 formula from 
   * http://www.embeddedadventures.com/datasheets/Sensirion_Humidity_SHT2x_CRC_Calculation_V1.pdf
   *
   * Test data 0xBE, 0xEF should yield 0x92
   *
   * Initialization data 0x00
   * Polynomial 0x31 (x8 + x5 +x4 +1)
   * Final XOR 0x00
   */

  const uint8_t POLYNOMIAL(0x31);
  uint8_t crc(0x00);

  for (int j = len; j; --j) {
    crc ^= *data++;

    for (int i = 8; i; --i) {
      crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
    }
  }
  return crc;
}

bool SHT21::startMeasurement(void) {
  uint8_t readbuffer[3];

  writeCommand(SHT21_MEAS_NOHOLD_TEMP);

  #ifdef SHT21_SLEEP
    LowPower.powerDown(SLEEP_120MS, ADC_OFF, BOD_OFF);
  #else
    delay(85);
  #endif

  _wire->requestFrom(_i2caddr, sizeof(readbuffer));
  if (_wire->available() != sizeof(readbuffer))
    return false;
  for (size_t i = 0; i < sizeof(readbuffer); i++) {
    readbuffer[i] = _wire->read();
  }

  if (readbuffer[2] != crc8(readbuffer, 2))
    return false;

  readbuffer[1] &= ~0x03; // clear status bits
  int32_t stemp = (int32_t) (((uint32_t)readbuffer[0] << 8) | readbuffer[1]);
  temp = (- 4685L + (17572L * stemp) / 65536L) / 10;

  writeCommand(SHT21_MEAS_NOHOLD_HUMI);

  #ifdef SHT21_SLEEP
    LowPower.powerDown(SLEEP_30MS, ADC_OFF, BOD_OFF);  
  #else
    delay(29);
  #endif

  _wire->requestFrom(_i2caddr, sizeof(readbuffer));
  if (_wire->available() != sizeof(readbuffer))
    return false;
  for (size_t i = 0; i < sizeof(readbuffer); i++) {
    readbuffer[i] = _wire->read();
  }

  if (readbuffer[2] != crc8(readbuffer, 2))
    return false;

  readbuffer[1] &= ~0x03; // clear status bits
  uint32_t shum = ((uint32_t)readbuffer[0] << 8) | readbuffer[1];
//  humidity = (-6.0 + 125.0 / 65536.0 * (float)shum);
  humidity = -6 + (125UL * shum) / 65536UL;

  return true;
}

void SHT21::writeCommand(uint8_t cmd) {
  _wire->beginTransmission(_i2caddr);
  _wire->write(cmd);
  _wire->endTransmission();
}
