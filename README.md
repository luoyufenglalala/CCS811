# DFRobot_CCS811
这是一个用于测试二氧化碳和VOC浓度的传感器<br>
这个传感器通过IIC通信，测量范围为400-8000ppm<br>

![正反面svg效果图](https://github.com/ouki-wang/DFRobot_Sensor/raw/master/resources/images/SEN0245svg1.png)


## 产品链接（链接到英文商城）
    SKU：产品名称
   
## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

这个库可以用于读取二氧化碳和VOC浓度
这个库可以改变读取数据频率和设置环境参数以校准读取数据

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++

  typedef enum{
      eMode0, //Idle (Measurements are disabled in this mode)
      eMode1, //Constant power mode, IAQ measurement every second
      eMode2, //Pulse heating mode IAQ measurement every 10 seconds
      eMode3, //Low power pulse heating mode IAQ measurement every 60 seconds
      eMode4  //Constant power mode, sensor measurement every 250ms 1xx: Reserved modes (For future use)
  }eDRIVE_MODE_t;

  /**
   * @brief 初始化函数
   * @return 返回0表示初始化成功，返回其他值表示初始化失败
   */
  int begin(void);
  
  /**
   * @brief 判断数据能否读取
   * @return true表示能成功读取,false表示读取失败
   */
  bool checkDataReady();

  /**
   * @brief 软件复位，清除所有配置寄存器参数
   */
  void softReset();
  
    /**
   * @brief 设置环境参数
   * @param temperature 传入温度值，单位为摄氏度，范围(-40~85℃)
   * @param humidity    传入湿度值，单位为RH，范围(0~100)
   */
  void setInTemHum(float temperature, float humidity);
  
  /**
   * @brief 设置测量和条件配置参数
   * @param thresh:0 for Interrupt mode operates normally; 1 for interrupt mode only asserts the nINT signal (driven low) if the new
   * @param interrupt:0 for Interrupt generation is disabled; 1 for the nINT signal is asserted (driven low) when a new sample is ready in
   * @param mode:in typedef enum eDRIVE_MODE_t
   */
  void setMeasurementMode(uint8_t thresh, uint8_t interrupt, eDRIVE_MODE_t mode);
   
  /**
   * @brief 获取当前二氧化碳浓度
   * @return 当前二氧化碳浓度，单位为ppm
   */
  uint16_t  getCO2();

  /**
   * @brief 获取当前VOC浓度
   * @return 返回当前VOC浓度，单位为ppb
   */
  uint16_t getTVOC();
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno        |      √       |              |             | 
Mega2560        |      √       |              |             | 
Leonardo        |      √       |              |             | 
ESP32        |      √       |              |             | 
Micro:bit        |      √       |              |             | 


## History

- data 2019-7-19
- version V0.1


## Credits

Written by(yufeng.luo@dfrobot.com), 2019. (Welcome to our [website](https://www.dfrobot.com/))





