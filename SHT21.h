/* 
 * This library is based on the Adafruit SHT31 library by Limor Fried/Ladyada (Adafruit Industries): 
 * https://github.com/adafruit/Adafruit_SHT31/
 * 
 * Modified for SHT21 by Tobias Mädel <t.maedel@alfeld.de> / @manawyrm
 * 
 * BSD license applies
 */

#ifndef SHT21_H
#define SHT21_H

#include "Arduino.h"
#include "Wire.h"

#define SHT21_DEFAULT_ADDR 0x40 /**< SHT21 Default Address */
#define SHT21_MEAS_NOHOLD_TEMP 0xF3
#define SHT21_MEAS_NOHOLD_HUMI 0xF5
#define SHT21_SOFTRESET 0xFE   /**< Soft Reset */

extern TwoWire Wire; /**< Forward declarations of Wire for board/variant
                        combinations that don't have a default 'Wire' */

class SHT21 {
public:
  /**
   *  Constructor.
   */
  SHT21(TwoWire *theWire = &Wire);

  /**
   * Initialises the I2C bus, and assigns the I2C address to us.
   *
   * @param i2caddr   The I2C address to use for the sensor.
   *
   * @return True if initialisation was successful, otherwise False.
   */
  bool begin(uint8_t i2caddr = SHT21_DEFAULT_ADDR);

  /**
   * Gets a single temperature reading from the sensor.
   *
   * @return A float value indicating the temperature.
   */
  int16_t readTemperature(void);

  /**
   * Gets a single relative humidity reading from the sensor.
   *
   * @return A float value representing relative humidity.
   */
  uint8_t readHumidity(void);

  /**
   * Perform a temp + humidity measurement
   *
   * @return True if successful, otherwise false.
   */
  bool startMeasurement(void);

  /**
   * Performs a reset of the sensor to put it into a known state.
   */
  void reset(void);

  /**
   * Enables or disabled the heating element.
   *
   * @param h True to enable the heater, False to disable it.
   */
  void heater(bool h);

  TwoWire *_wire; /**< Wire object */

private:
  /**
   * Placeholder to track humidity internally.
   */
  uint8_t humidity;

  /**
   * Placeholder to track temperature internally.
   */
  int16_t temp;

  /**
   * Placeholder to track the I2C address.
   */
  uint8_t _i2caddr;

  /**
   * Internal function to perform and I2C write.
   *
   * @param cmd   The 8-bit command ID to send.
   */
  void writeCommand(uint8_t cmd);

  /**
   * Internal function to read data over the I2C bus.
   *
   * @return True if successful, otherwise False.
   */
  bool readData(void);
};

#endif
