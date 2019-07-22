/*!
 * @file readData.ino
 * @brief 读取二氧化碳浓度，有害气体浓度
 * @n 实验现象：每0.5秒读取一次数据，并打印到串口
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
        Serial.println("Data is not ready!");
    }
    //延时不能低于250ms
    delay(500);
}