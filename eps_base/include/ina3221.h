/*
 * ina3221.h
 *
 *  Created on: Jul 8, 2020
 *      Author: vince montero
 */
#ifndef INCLUDE_INA3221_H_
#define INCLUDE_INA3221_H_
//
//   SDL_Arduino_INA3221 Library
//   SDL_Arduino_INA3221.cpp Arduino code - runs in continuous mode
//   Version 1.2
//   SwitchDoc Labs   September 2019
//
//
/**************************************************************************/
/*!
    Initial code from INA219 code (Basically just a core structure left)
    @author   K. Townsend (Adafruit Industries)
    @license  BSD (see BSDlicense.txt)
    */
/**************************************************************************/
#include "HL_i2c.h"
#include <stdlib.h>
#include <stdio.h>
/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define INA3221_ADDRESS                         (0x40)    // 1000000 (A0+A1=GND)
    #define INA3221_READ                            (0x01)
/*=========================================================================*/
/*=========================================================================
    CONFIG REGISTER (R/W)
    -----------------------------------------------------------------------*/
    #define INA3221_REG_CONFIG                      (0x00)
    /*---------------------------------------------------------------------*/
    #define INA3221_CONFIG_RESET                    (0x8000)  // Reset Bit
    #define INA3221_CONFIG_ENABLE_CHAN1             (0x4000)  // Enable Channel 1
    #define INA3221_CONFIG_ENABLE_CHAN2             (0x2000)  // Enable Channel 2
    #define INA3221_CONFIG_ENABLE_CHAN3             (0x1000)  // Enable Channel 3
    #define INA3221_CONFIG_AVG2                     (0x0800)  // AVG Samples Bit 2 - See table 3 spec
    #define INA3221_CONFIG_AVG1                     (0x0400)  // AVG Samples Bit 1 - See table 3 spec
    #define INA3221_CONFIG_AVG0                     (0x0200)  // AVG Samples Bit 0 - See table 3 spec
    #define INA3221_CONFIG_VBUS_CT2                 (0x0100)  // VBUS bit 2 Conversion time - See table 4 spec
    #define INA3221_CONFIG_VBUS_CT1                 (0x0080)  // VBUS bit 1 Conversion time - See table 4 spec
    #define INA3221_CONFIG_VBUS_CT0                 (0x0040)  // VBUS bit 0 Conversion time - See table 4 spec
    #define INA3221_CONFIG_VSH_CT2                  (0x0020)  // Vshunt bit 2 Conversion time - See table 5 spec
    #define INA3221_CONFIG_VSH_CT1                  (0x0010)  // Vshunt bit 1 Conversion time - See table 5 spec
    #define INA3221_CONFIG_VSH_CT0                  (0x0008)  // Vshunt bit 0 Conversion time - See table 5 spec
    #define INA3221_CONFIG_MODE_2                   (0x0004)  // Operating Mode bit 2 - See table 6 spec
    #define INA3221_CONFIG_MODE_1                   (0x0002)  // Operating Mode bit 1 - See table 6 spec
    #define INA3221_CONFIG_MODE_0                   (0x0001)  // Operating Mode bit 0 - See table 6 spec
/*=========================================================================*/
/*=========================================================================
    SHUNT VOLTAGE REGISTERS (R)
    -----------------------------------------------------------------------*/
    #define INA3221_REG_SHUNTVOLTAGE_1                (0x01)
    #define INA3221_REG_SHUNTVOLTAGE_2                (0x03)
    #define INA3221_REG_SHUNTVOLTAGE_3                (0x05)
 /*=========================================================================*/
/*=========================================================================
    CHANNELS
    -----------------------------------------------------------------------*/
    #define CH_1                                       (1)
    #define CH_2                                       (2)
    #define CH_3                                       (3)

/*=========================================================================
    BUS VOLTAGE REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA3221_REG_BUSVOLTAGE_1                  (0x02)
    #define INA3221_REG_BUSVOLTAGE_2                  (0x02)
    #define INA3221_REG_BUSVOLTAGE_3                  (0x06)
/*=========================================================================*/
    #define SHUNT_RESISTOR_VALUE  (0.1)   // default shunt resistor value of 0.1 Ohm
    void ina3221Config(void);
    void ina3221SetRegisterPointer(uint8 reg);
    void ina3221WriteRegister(uint8 reg, uint8 Data_MSByte, uint8 Data_LSByte);
    int16_t ina3221ReadRegister(uint8 reg);
    double ina3221GetBusVoltage_V(int channel);
    double ina3221GetShuntVoltage_mV(int channel);
    int ina3221GetManufID(void);


#endif /* INCLUDE_INA3221_H_ */