/*!
 * @file DFRobot_CCS811.h
 * @brief 定义DFRobot_CCS811 类的基础结构
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yufeng](yufeng.luo@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-13
 * @https://github.com/DFRobot/DFRobot_CCS811
 */

#ifndef _DFROBOT_CCS811_H
#define _DFROBOT_CCS811_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Wire.h>


/*I2C ADDRESS*/
#define CCS811_I2C_ADDRESS1                      0x5A
#define CCS811_I2C_ADDRESS2                      0x5B

#define CCS811_REG_STATUS                        0x00
#define CCS811_REG_MEAS_MODE                     0x01
#define CCS811_REG_ALG_RESULT_DATA               0x02
#define CCS811_REG_RAW_DATA                      0x03
#define CCS811_REG_ENV_DATA                      0x05
#define CCS811_REG_NTC                           0x06
#define CCS811_REG_THRESHOLDS                    0x10
#define CCS811_REG_BASELINE                      0x11
#define CCS811_REG_HW_ID                         0x20
#define CCS811_REG_HW_VERSION                    0x21
#define CCS811_REG_FW_BOOT_VERSION               0x23
#define CCS811_REG_FW_APP_VERSION                0x24
#define CCS811_REG_INTERNAL_STATE                0xA0
#define CCS811_REG_ERROR_ID                      0xE0
#define CCS811_REG_SW_RESET                      0xFF

#define CCS811_BOOTLOADER_APP_ERASE              0xF1
#define CCS811_BOOTLOADER_APP_DATA               0xF2
#define CCS811_BOOTLOADER_APP_VERIFY             0xF3
#define CCS811_BOOTLOADER_APP_START              0xF4

#define CCS811_HW_ID                             0x81
//打开这个宏，可以看到程序的详细运行过程
//#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

typedef enum{
    eMode0, //Idle (Measurements are disabled in this mode)
    eMode1, //Constant power mode, IAQ measurement every second
    eMode2, //Pulse heating mode IAQ measurement every 10 seconds
    eMode3, //Low power pulse heating mode IAQ measurement every 60 seconds
    eMode4  //Constant power mode, sensor measurement every 250ms 1xx: Reserved modes (For future use)
}eDRIVE_MODE_t;


class DFRobot_CCS811
{
public:
    #define ERR_OK             0      //无错误
    #define ERR_DATA_BUS      -1      //数据总线错误
    #define ERR_IC_VERSION    -2      //芯片版本不匹配
    /**
     * @brief 构造函数
     * @param 传入Wire地址
     */
    DFRobot_CCS811(TwoWire *pWire = &Wire){_pWire = pWire;};
    
         /**
          * @brief 初始化函数
          * @return 返回0表示初始化成功，返回其他值表示初始化失败
          */
    int  begin();
         /**
          * @brief 判断有无数据读取
          * @return 返回1表示有数据读取，返回0表示无数据读取
          */
    bool checkDataReady();
         /**
          * @brief 复位传感器，清除所有配置数据
          */
    void softReset(),
         /**
          * @brief 设置环境参数
          * @param temperature 设置环境温度,单位为℃,范围是-40~85℃
          * @param humidity    设置环境湿度,单位为RH,范围是0~100RH
          */
         setInTempHum(float temperature, float humidity),
         /**
          * @brief 设置测量和条件配置参数
          * @param thresh:0 for Interrupt mode operates normally; 1 for interrupt mode only asserts the nINT signal (driven low) if the new
          * @param interrupt:0 for Interrupt generation is disabled; 1 for the nINT signal is asserted (driven low) when a new sample is ready in
          * @param mode:in typedef enum eDRIVE_MODE_t
          */
         setMeasurementMode(uint8_t thresh, uint8_t interrupt, eDRIVE_MODE_t mode),
         /**
          * @brief 设置中断阈值
          * @param lowToMed:低到中范围触发中断的值
          * @param medToHigh:中到高范围触发中断的值
          * @param hysteresis:超出阈值的滞后值
          */
         setThresholds(uint16_t lowToMed, uint16_t medToHigh, uint8_t hysteresis),
         /**
          * @brief 设置温度偏移校准值，用于校准NTC测量的误差
          * @param offset:该偏移量用于温度计算中
          */
         setTempOffset(float offset);
         /**
          * @brief 获取当前配置参数
          * @return 配置参数代码，需要转换成二进制代码进行解析
          *         第2位0: Interrupt mode (if enabled) operates normally,1: Interrupt mode (if enabled) only asserts the nINT signal (driven low) if the new
          *         第3位0: Interrupt generation is disabled,1: The nINT signal is asserted (driven low) when a new sample is ready in
          *         第4:6位:in typedef enum eDRIVE_MODE_t
          */
    uint8_t getMeasurementMode();

              /**
               * @brief 获取当前二氧化碳浓度
               * @return 返回当前二氧化碳浓度，单位为ppm
               */
    uint16_t  getCO2PPM(),
              /**
               * @brief 获取当前VOC浓度
               * @return 返回当前VOC浓度，单位为ppb
               */
              getTVOCPPB();
              /**
               * @brief 获取当前气温
               * @return 返回当前气温，单位为摄氏度
               */
    double    getCurrentTemp();
              /**
               * @brief 获取当前温度偏移校准值
               * @return 返回当前偏移校准值，单位为摄氏度
               */
    
protected:

    typedef struct{
        /*
         * The CCS811 received an I²C write request addressed to this station but with invalid register address ID
         */
        uint8_t sWRITE_REG_INVALID: 1;
        /*
         * The CCS811 received an I²C read request to a mailbox ID that is invalid
         */
        uint8_t sREAD_REG_INVALID: 1;
        /*
         * The CCS811 received an I²C request to write an unsupported mode to MEAS_MODE
         */
        uint8_t sMEASMODE_INVALID: 1;
        /*
         * The sensor resistance measurement has reached or exceeded the maximum range
         */
        uint8_t sMAX_RESISTANCE: 1;
        /*
         * The The Heater current in the CCS811 is not in range
         */
        uint8_t sHEATER_FAULT: 1;
        /*
         * The Heater voltage is not being applied correctly
         */
        uint8_t sHEATER_SUPPLY: 1;
    } __attribute__ ((packed))sError_id;
    
    typedef struct{
        /* 
         * ALG_RESULT_DATA crosses one of the thresholds set in the THRESHOLDS register 
         * by more than the hysteresis value (also in the THRESHOLDS register)
         */
        uint8_t sINT_THRESH: 1;
        /* 
         * At the end of each measurement cycle (250ms, 1s, 10s, 60s) a flag is set in the
         * STATUS register regardless of the setting of this bit.
         */
        uint8_t sINT_DATARDY: 1;
        /* 
         * A new sample is placed in ALG_RESULT_DATA and RAW_DATA registers and the
         * DATA_READY bit in the STATUS register is set at the defined measurement interval.
         */
        uint8_t sDRIVE_MODE: 3;
    } __attribute__ ((packed))sMeas_mode;
    
    typedef struct{
        /* 
         * This bit is cleared by reading ERROR_ID
         * It is not sufficient to read the ERROR field of ALG_RESULT_DATA and STATUS
         */
        uint8_t sERROR: 1;
        /* 
         * ALG_RESULT_DATA is read on the I²C interface
         */
        uint8_t sDATA_READY: 1;
        uint8_t sAPP_VALID: 1;
        /* 
         * After issuing a VERIFY command the application software must wait 70ms before 
         * issuing any transactions to CCS811 over the I²C interface
         */
        uint8_t sAPP_VERIFY: 1;
        /* 
         * After issuing the ERASE command the application software must wait 500ms 
         * before issuing any transactions to the CCS811 over the I2C interface.
         */
        uint8_t sAPP_ERASE: 1;
        uint8_t sFW_MODE: 1;
    } __attribute__ ((packed))sStatus;
    
    
    void getData(void);
    
    void writeConfig();
         
    virtual void writeReg(uint8_t reg, const void* pBuf, size_t size);
    virtual uint8_t readReg(uint8_t reg, const void* pBuf, size_t size);
    
    

private:
    TwoWire *_pWire;
    uint8_t _deviceAddr = CCS811_I2C_ADDRESS1;
    
    uint16_t eCO2;
    uint16_t eTVOC;
    uint8_t thresh, interrupt;
    eDRIVE_MODE_t driveMode;
    float tempOffset;
};

#endif
