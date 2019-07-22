/*!
 * @file setConfig.ino
 * @brief ,改变数据读取频率，读取二氧化碳浓度，有害气体浓度
 * @n 实验现象：每1秒读取一次数据，并打印到串口
 *
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LuoYufeng](yufeng.luo@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-13
 * @https://github.com/DFRobot/DFRobot_Sensor
 */
#include "DFRobot_CCS811.h"

DFRobot_CCS811 sensor;

void setup(void)
{
    Serial.begin(115200);
    //sensor.setI2cAddr(sensor.I2C_ADDRESS1);
    /*在这里一致等到芯片初始化完成才能退出*/
    while(sensor.begin() != 0){
        Serial.println("初始化芯片失败，请确认芯片连接是否正确");
        delay(1000);
    }
    /**
     * @brief 设置测量和条件配置参数
     * @param thresh:0 for Interrupt mode operates normally; 1 for interrupt mode only asserts the nINT signal (driven low) if the new
     * @param interrupt:0 for Interrupt generation is disabled; 1 for the nINT signal is asserted (driven low) when a new sample is ready in
     * @param mode:in typedef enum{
     *                eMode0, //Idle (Measurements are disabled in this mode)
     *                eMode1, //Constant power mode, IAQ measurement every second
     *                eMode2, //Pulse heating mode IAQ measurement every 10 seconds
     *                eMode3, //Low power pulse heating mode IAQ measurement every 60 seconds
     *                eMode4  //Constant power mode, sensor measurement every 250ms 1xx: Reserved modes (For future use)
     *               }eDRIVE_MODE_t;
     */
    sensor.setMeasurementMode(0, 0, sensor.eMode4);
    /**
     * @brief 设置环境参数
     * @param temperature 设置环境温度,单位为℃,范围是-40~85℃
     * @param humidity    设置环境湿度,单位为RH,范围是0~100RH
     */
    sensor.setInTemHum(25,50);
}
void loop() {
    if(sensor.checkDataReady() == true){
        Serial.print("CO2: ");
        Serial.print(sensor.getCO2());
        Serial.print("ppm, TVOC: ");
        Serial.print(sensor.getTVOC());
        Serial.println("ppb");
    }
    else{
        Serial.println("ERROR!");
        while(1);
    }
    delay(1000);
}