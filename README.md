# RM2023 Infantry Chassis
## 1 环境配置
环境配置学习了稚晖君的教程，知乎链接 : [配置CLion用于STM32开发【优雅の嵌入式开发】](https://zhuanlan.zhihu.com/p/145801160)
  - stm32cubemx
  - clion
  - arm-none-eabi-gcc
  - openocd
##
## 2 增加uart和can,幼年cpp版本，后续会改进
```
在此目录下
|   |-- common_lib
|   |   `-- communication
|   |       |-- com_vision.cpp      //视觉通信应用类  
|   |       |-- com_vision.hpp
|   |       |-- fixed_packet.hpp    //数据包模板类--可自定义数据包长度
|   |       |-- transporter_can.cpp //can通信底层类--只负责接受和发送数据，应用层外接
|   |       |-- transporter_can.hpp
|   |       |-- transporter_interface.hpp   //interface通信类
|   |       `-- transporter_usart.hpp //穿透通信底层类，只负责收发数据
```
写的比较垃圾就不深入解释了，以后完善后再解释

