/*!
 * @file DFRobot_CCS811.cpp
 * @brief 定义DFRobot_CCS811 类的基础结构
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yufeng](yufeng.luo@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-13
 * @https://github.com/DFRobot/DFRobot_CCS811
 */

#include <DFRobot_CCS811.h>

int DFRobot_CCS811::begin(void)
{
    uint8_t id=0;
    //uint8_t _mode[1] = {0x40};
    Wire.begin();
    softReset();
    delay(100);
    if(readReg(CCS811_REG_HW_ID,&id,1) != 1){DBG("");
        DBG("bus data access error");DBG("");
        return ERR_DATA_BUS;DBG("");
    }

    DBG("real sensor id=");DBG(id);
    if(id != CCS811_HW_ID){DBG("");
        return ERR_IC_VERSION;
    }
    writeReg(CCS811_BOOTLOADER_APP_START, NULL, 0);
    setMeasurementMode(0,0,eMode4);
    setInTemHum(25, 50);
    return ERR_OK;
}

void DFRobot_CCS811::softReset(){
    uint8_t value[4] = {0x11, 0xE5, 0x72, 0x8A};
    writeReg(CCS811_REG_SW_RESET, value, 4);
}

bool DFRobot_CCS811::checkDataReady()
{
    int8_t status[1] = {0};
    readReg(CCS811_REG_STATUS, status, 1);
    DBG(status[0],HEX);
    if(!((status[0] >> 3) & 0x01))
        return false;
    else 
        return true;
}

void DFRobot_CCS811::setMeasurementMode(uint8_t thresh, uint8_t interrupt, eDRIVE_MODE_t mode){
    uint8_t measurement[1] = {0};
    measurement[0] = (thresh << 2) | (interrupt << 3) | (mode << 4);
    //Serial.println(*measurement);
    writeReg(CCS811_REG_MEAS_MODE, measurement, 1);
}

void DFRobot_CCS811::setThresholds(uint16_t lowToMed, uint16_t medToHigh, uint8_t hysteresis)
{
    uint8_t buf[] = {(uint8_t)((lowToMed >> 8) & 0xF), (uint8_t)(lowToMed & 0xF),
    (uint8_t)((medToHigh >> 8) & 0xF), (uint8_t)(medToHigh & 0xF), hysteresis};
    
    writeReg(CCS811_REG_THRESHOLDS, buf, 5);
}

uint16_t DFRobot_CCS811::getCO2(){
    uint8_t buffer[8];
    readReg(CCS811_REG_ALG_RESULT_DATA, buffer, 8);
    eCO2 = (((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1]);
    return eCO2;
}

uint16_t DFRobot_CCS811::getTVOC(){
    uint8_t buffer[8];
    readReg(CCS811_REG_ALG_RESULT_DATA, buffer, 8);
    eTVOC = (((uint16_t)buffer[2] << 8) | (uint16_t)buffer[3]);
    return eTVOC;
}

void DFRobot_CCS811::setInTemHum(float temperature, float humidity)    // compensate for temperature and relative humidity
{
    int _temp, _rh;
    if(temperature>0)
        _temp = (int)temperature + 0.5;  // this will round off the floating point to the nearest integer value
    else if(temperature<0) // account for negative temperatures
        _temp = (int)temperature - 0.5;
    _temp = _temp + 25;  // temperature high byte is stored as T+25°C in the sensor's memory so the value of byte is positive
    _rh = (int)humidity + 0.5;  // this will round off the floating point to the nearest integer value
    
    uint8_t envData[4];
    
    envData[0] = _rh << 1;  // shift the binary number to left by 1. This is stored as a 7-bit value
    envData[1] = 0;  // most significant fractional bit. Using 0 here - gives us accuracy of +/-1%. Current firmware (2016) only supports fractional increments of 0.5
    envData[2] = _temp << 1;
    envData[3] = 0;
    
    writeReg(CCS811_REG_ENV_DATA, &envData, 4);
}

void DFRobot_CCS811::writeReg(uint8_t reg, const void* pBuf, size_t size)
{
    if(pBuf == NULL){
        DBG("pBuf ERROR!! : null pointer");
    }
    uint8_t * _pBuf = (uint8_t *)pBuf;
    _pWire->beginTransmission(_deviceAddr);
    _pWire->write(&reg, 1);
    
    for(uint16_t i = 0; i < size; i++){
        _pWire->write(_pBuf[i]);
    }
    _pWire->endTransmission();
}

uint8_t DFRobot_CCS811::readReg(uint8_t reg, const void* pBuf, size_t size)
{
    if(pBuf == NULL){
        DBG("pBuf ERROR!! : null pointer");
    }
    uint8_t * _pBuf = (uint8_t *)pBuf;
    _pWire->beginTransmission(_deviceAddr);
    _pWire->write(&reg, 1);
    
    if( _pWire->endTransmission() != 0){
        return 0;
    }

    _pWire->requestFrom(_deviceAddr, (uint8_t) size);
    for(uint16_t i = 0; i < size; i++){
        _pBuf[i] = _pWire->read();
    }
    _pWire->endTransmission();
    return size;
}
