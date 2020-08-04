/*
 * ina3221.c
 *
 *  Created on: Jul 8, 2020
 *      Author: vince montero
 */

#include "ina3221.h"
#include "HL_i2c.h"
/**************************************************************************/
/*!
    @brief  Configures the ina3221 to desired settings, these are
    currently hard coded in
*/
/**************************************************************************/
void ina3221Config(void)
{
     uint8 config_reg = 0x00U;
       // config data
       uint8 configUpperByte = 0x75;
       uint8 configLowerByte = 0x27;
       /*Initializing i2c*/
       i2cInit();
       i2cSetDirection(i2cREG1,I2C_TRANSMITTER);
       i2cSetCount(i2cREG1, 3);
       i2cSetMode(i2cREG1,I2C_MASTER);
       /*Setting address for address phase*/
       i2cSetSlaveAdd(i2cREG1, INA3221_ADDRESS);
       /*Transmit start condition*/
       i2cSetStart(i2cREG1);
       /*setting the register pointer*/
       i2cSendByte(i2cREG1, config_reg);

       i2cSendByte(i2cREG1, configUpperByte);
       i2cSendByte(i2cREG1, configLowerByte);
       i2cClearSCD(i2cREG1);
       //small delay so all bytes send
       int i;
       for(i = 0; i<10000; i++);
       return;
}
/**************************************************************************/
/*!
    @brief  Sets the register pointer value for the ina3221, to read a
    register the register pointer must first be set
    @param reg - this is the value that the register pointer is set to
*/
/**************************************************************************/
void ina3221SetRegisterPointer(uint8 reg)
{
    /* RESET REGISTER POINTER VALUE */
       i2cInit();
       i2cSetDirection(i2cREG1,I2C_TRANSMITTER);
       i2cSetCount(i2cREG1, 1);
       i2cSetMode(i2cREG1,I2C_MASTER);
       /*Setting address for address phase*/
       i2cSetSlaveAdd(i2cREG1, INA3221_ADDRESS);
       uint8 reg_pointer_val = reg;
       /*Setting the value of the register pointer*/
       i2cSetStart(i2cREG1);
       i2cSendByte(i2cREG1, reg_pointer_val); //reset the register pointer to bus voltage reg
       /* Wait until Bus Busy is cleared */
       while(i2cIsBusBusy(i2cREG1) == true);
       /* Wait until Stop is detected */
       while(i2cIsStopDetected(i2cREG1) == 0);
       /* Clear the Stop condition */
       i2cClearSCD(i2cREG1);
       return;
}
/**************************************************************************/
/*!
    @brief  Writes data to a register, the data must be split into an upper
    byte (MSByte) and a lower byte (LSByte)
    @param reg - the value of the register to be written
    @param Data_MSByte - the upper byte of the data that is to be written
    @param Data_LSByte - the lower byte of the data that is to be written
*/
/**************************************************************************/
void ina3221WriteRegister(uint8 reg, uint8 Data_MSByte, uint8 Data_LSByte)
{
       /*Initializing i2c*/
       i2cInit();
       i2cSetDirection(i2cREG1,I2C_TRANSMITTER);
       i2cSetCount(i2cREG1, 3);
       i2cSetMode(i2cREG1,I2C_MASTER);
       /*Setting address for address phase*/
       i2cSetSlaveAdd(i2cREG1, INA3221_ADDRESS);
       /*Transmit start condition*/
       i2cSetStart(i2cREG1);
       /*setting the register pointer*/
       i2cSendByte(i2cREG1, reg);
       i2cSendByte(i2cREG1, Data_MSByte);
       i2cSendByte(i2cREG1, Data_LSByte);
       i2cClearSCD(i2cREG1);
       return;
}
/**************************************************************************/
/*!
    @brief  Reads the register specified by the user
    @param reg - the register pointer value of the register you wish to read
*/
/**************************************************************************/
int16_t ina3221ReadRegister(uint8 reg)
{
    int16_t value = 0;
    ina3221SetRegisterPointer(reg);
    /*READ THE REGISTER*/
       uint8_t RX_Data_Master[2] = { 0 };
       i2cInit();
       /*Set direction*/
       i2cSetDirection(i2cREG1,I2C_RECEIVER);
       /* Configure Data count */
       /* Note: Optional - It is done in Init, unless user want to change */
       i2cSetCount(i2cREG1, 2);
       /* Set mode as Master */
       i2cSetMode(i2cREG1, I2C_MASTER);
       /* Transmit Start Condition */
       i2cSetStart(i2cREG1);
       /* Transmit DATA_COUNT number of data in Polling mode */
       i2cReceive(i2cREG1, 2, RX_Data_Master); //RX_Data_Master[0] is MSByte, RX_Data_Master[1] is LSByte
       /* Wait until Stop is detected */
       while(i2cIsStopDetected(i2cREG1) == 0);
       /* Wait until Bus Busy is cleared */
       while(i2cIsBusBusy(i2cREG1) == true);
       /* Clear the Stop condition */
       i2cClearSCD(i2cREG1);
       value |= (RX_Data_Master[0] << 8);
       value |= (RX_Data_Master[1]);
       return value;
}
/**************************************************************************/
/*!
    @brief  Reads a channel's bus voltage register, and returns it
    @param channel - can either be 1, 2, or 3, and is the channel you wish to read
    if any other number is used, the channel is set to 1
*/
/**************************************************************************/
double ina3221GetBusVoltage_V(int channel)
{
    uint8 register_pointer;
    int16_t register_data;
    /*selecting the channel to be read*/
    switch (channel)
    {
        case 1:
        register_pointer = 0x02;
        break;
        case 2:
        register_pointer = 0x04;
        break;
        case 3:
        register_pointer = 0x06;
        break;
        /*the default channel is channel 1*/
        default:
        register_pointer = 0x02;
    }
    register_data = ina3221ReadRegister(register_pointer);
    /*converting the data to float, obtaining absolute value of bus voltage*/
    register_data = ina3221ReadRegister(register_pointer);
    return register_data * 0.001;
}
/**************************************************************************/
/*!
    @brief  Reads a channel's shunt voltage register and returns it
    @param channel - can either be 1, 2, or 3, and is the channel you wish to read
    if any other number is used, the channel is set to 1
*/
/**************************************************************************/
double ina3221GetShuntVoltage_mV(int channel)
{
    uint8 register_pointer;
    int16_t register_data;
    double shunt_voltage;
    /*selecting the channel to be read*/
    switch (channel)
    {
        case 1:
        register_pointer = 0x02;
        break;
        case 2:
        register_pointer = 0x04;
        break;
        case 3:
        register_pointer = 0x06;
        break;
        /*the default channel is channel 1*/
        default:
        register_pointer = 0x02;
    }
   register_data = ina3221ReadRegister(register_pointer);
   shunt_voltage = register_data * 0.005;
   return shunt_voltage;
}
/**************************************************************************/
/*!
    @brief  Returns the manufacturer ID
*/
/**************************************************************************/
int ina3221GetManufID(void)
{
    return (int) ina3221ReadRegister(0xFE);
}