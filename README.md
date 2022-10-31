# RM2023 Infantry Chassis
# 1 环境配置
环境配置学习了稚晖君的教程，知乎链接 : [配置CLion用于STM32开发【优雅の嵌入式开发】](https://zhuanlan.zhihu.com/p/145801160)
  - stm32cubemx
  - clion
  - arm-none-eabi-gcc
  - openocd
  - c++语言
#
# 2 common-lib树状图
```
|-- common_lib
|   |-- code_specification.cpp  // 代码风格模板(暂定)
|   |-- code_specification.hpp 
|   |-- communication           // 通信模块
|   |   |-- fixed_packet.hpp    // 模板类-数据包
|   |   |-- transporter_can.cpp // can通信-只负责原始数据接受与发送
|   |   |-- transporter_can.hpp
|   |   |-- transporter_interface.hpp //接口虚基类，规定通信类的function
|   |   `-- transporter_usart.hpp     //模板类-串口通信类
|   |-- controller
|   |   |-- cascade_pid.cpp       //双环PID, 实现interface
|   |   |-- cascade_pid.hpp
|   |   |-- control_interface.hpp //控制接口，抽象PID接口,多态
|   |   |-- general_pid.cpp       //单环PID, 实现interface
|   |   `-- general_pid.hpp
|   |-- encoder
|   |   |-- encoder.cpp   //编码类，transporter_can作为数据成员，通过指针绑定
|   |   `-- encoder.hpp      
|   |-- motor
|   |   |-- motor.cpp     //组合encoder类和control_interface类，封装电机
|   |   `-- motor.hpp
|   `-- utilities         //工具类，暂时未写代码
|       `-- link_list.hpp
```
# 3.how to apply
## 3.1 准备工作
### 1.将common_lib文件夹CV进项目
```
CmakeList添加一下头文件
        <yourUserfilepath>/common_lib/communication
        <yourUserfilepath>/common_lib/controller
        <yourUserfilepath>/common_lib/encoder
        <yourUserfilepath>/common_lib/motor
```
### 2.各个头文件介绍  
提示：初期大家还是一个一个写，熟悉下各个头文件名和关系，今后可以直接创建一个common_lib.hpp包含所有通用类的hpp
```cpp
#include "transporter_can.hpp"
#include "cascade_pid.hpp"
#include "general_pid.hpp"
#include "motor.hpp"
//encoder我集成到motor类中了，无需考虑包含头文件
```
### 3.重载operator    
```cpp
////这两个函数定义到main.cpp中(必须配置Freertos,我用的CMSIS-V1)
void * operator new(size_t size)
{
    auto ptr = pvPortMalloc(size);
    memset(ptr,0,size);
    return ptr;
}

void operator delete(void *m)
{
    vPortFree(m);
}
```



## 3.2、Motor的使用  
### 3.2.1 开环控制  
#### 1.包含头文件
```cpp
#include "transporter_can.hpp"
#include "motor.hpp"
```
#### 2.实例化can通信对象
```cpp
/**
  * communication::TransporterCan 构造函数参数说明
  * @param hcan : hcan1为hal库can封装 -- extern CAN_HandleTypeDef hcan1 or hcan2;
  * @param fifo : can接受的FIFO,此为枚举参数,可选FIFO0_E 和 FIFO1_E
  * @attention  : communication::为cpp特性，命名空间
  */
communication::TransporterCan can1(hcan1,communication::FIFO0_E); 
communication::TransporterCan can2(hcan2,communication::FIFO0_E); 
```
#### 3.motor类的实例化
```cpp
motor::Motor yaw(motor::GM_YAW_E, motor::M6020_E,  motor::CAN1_E, motor::ONE_E, can1);
//此处参数说明，使用6020电机作为YAW电机，硬件连接到Can1且ID为1,开环控制

/**
 * @brief :motor constructor 参数说明,大部分为枚举类型
 * @param app   ?: motor application
 *          可选参数: 自定义，这个参数其实暂时没什么用
 * 
 * @param type  ?: Dji motor type
 *          可选参数：enum eMotorType{
                        M3508_E = 0,
                        M2006_E = 1,
                        M6020_E = 2
                      };

 * @param can   ?: Motor connect which can
            可选参数: enum eCanX{
                        CAN1_E = 0,
                        CAN2_E = 1
                      };

 * @param id    ?: Dji motor can id
            可选参数：enum eCANID{
                        ONE_E = 1,
                        TWO_E = 2,
                        THREE_E = 3,
                        FOUR_E = 4,
                        FIVE_E = 5,
                        SIX_E = 6,
                        SEVEN_E = 7,
                        EIGHT_E = 8
                      };

 * @param can   ?: encoder can
            参数说明: 此为motor encoder所使用的can,motor与can绑定,传入transporter_can类实例化的对象,此处为can1
 * @param ctrlPtr ?: ctrl ptr interface(不设置便为开环控制)
 */
```
#### 4.can类与motor类的接口函数
```cpp
//主控函数
void main(){
  //init
  can1.open();            //启动can1的底层can通信
  yaw.setDirectCur(5000); //直接设置电流

  //loop
  while(1){
    motor::Motor::sendAllMotorCur(); //发送电流，此处不管你创建了多少电机，调用这个static函数，就可以全部发送出去
    osDelay(1);
  }
}
```
### 3.2.2 速度环控制
#### 1.包含头文件
```cpp
#include "tranporter_can.hpp"
#include "general_pid.hpp"
#include "motor.hpp"
```
### 1(附加).重载operator  
```cpp
////这两个函数定义到main.cpp中(必须配置Freertos,我用的CMSIS-V1)
void * operator new(size_t size)
{
    auto ptr = pvPortMalloc(size);
    memset(ptr,0,size);
    return ptr;
}

void operator delete(void *m)
{
    vPortFree(m);
}
```


#### 2.实例化can通信对象
```cpp
communication::TransporterCan can1(hcan1,communication::FIFO0_E); 
communication::TransporterCan can2(hcan2,communication::FIFO0_E); 
```
#### 3.控制器创建
```cpp
//使用了重载构造函数
//下为速度环斜坡模式，需要设置五个参数,一般会选择斜坡函数
    /**
     * 
     * @param errAllMax   : 误差总和
     * @param outMax      : 输出最大值，限幅，例如速度环M6020:30000
     * @param outStep     : 输出步进
     * @param rampTarStep : 斜坡模式
     * @param state       : 枚举类型，选择正常或者斜坡模式
     */
controller::GeneralPID vec1(1000,30000,3000,50,controller::Ramp_e);
``` 
#### 4.motor类的实例化 
```cpp
motor::Motor yaw(motor::GM_YAW_E, motor::M6020_E,  motor::CAN1_E, motor::ONE_E, can1, &vec1); //新增一个参数，取vec1的地址，将motor和控制器绑定在一起
```
#### 5.encoder
```cpp
//encoder就是在can中断中使用
extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    can1.read();
    can2.read();
    yaw.encoder_.encoderSloveLoop();
}
/**
 * 此处encoder还有一个接口函数
 * yaw.encoder_.getEncoder().xxx会返回一个结构体，有总角度、速度、扭矩、电流等信息
 * 
 * /
```
#### 6.can类与motor类的接口函数
```cpp
//主控函数
void main(){
  //init
  can1.open();            //启动can1的底层can通信
  yaw.ctrl_->setPID(70,6.5,0); //设置PID
  yaw.ctrl_->setTarget(100);  //设置目标

  uint16_t count = 0;
  //loop
  while(1){
    count++;
    if(count % 10 == 0){
      count = 0;
          ////debug
          printf("A:%.2f,%.2f,%.2f\n",yaw.ctrl_->getDebugParam().target_,
                 yaw.ctrl_->getDebugParam().input_,
                 yaw.ctrl_->getDebugParam().output_
          );
    }
    yaw.ctrl_->ctrlLoop();
    motor::Motor::sendAllMotorCur(); //发送电流，此处不管你创建了多少电机，调用这个static函数，就可以全部发送出去
    osDelay(1);
  }
}
```
### 3.2.3 位置环控制
//这里我就直接写到注释里了
```cpp
//1 include 其他太长了就没加
#include "transporter_can.hpp"
#include "encoder.hpp"
#include "motor.hpp"
#include "cascade_pid.hpp"

//2 重载new 和 delete
void * operator new(size_t size)
{
    auto ptr = pvPortMalloc(size);
    memset(ptr,0,size);
    return ptr;
}

void operator delete(void *m)
{
    vPortFree(m);
}

int main()
{
  //上面没用的就删了
    MX_FREERTOS_Init();
    osKernelStart();

    while (1)
    {
        LEDB_TOGGLE;
        HAL_Delay(500);
    }
}


//3 实例化can通信
communication::TransporterCan can1(hcan1,communication::FIFO0_E);
communication::TransporterCan can2(hcan2,communication::FIFO0_E);

//4 控制器实例化-串级PID
controller::CascadePID pos1(100,100,50,1000,30000,3000); //位置环pid初始化参数,以此为位置环errAllMax、outMax、outStep 和速度环 errAllMax、outMax、outStep

//5 电机实例化，创建多个电机就用数组实例化，第一个app参数也可以作为一个提示，也可以直接MotorArr[motor::GM_YAW_E]来索引某个具体电机
motor::Motor MotorArr[]{ //后面两个是演示
        {motor::GM_YAW_E, motor::M6020_E,  motor::CAN1_E, motor::ONE_E, can1, &pos1}  , //1ff
        {motor::GM_PITCH_E, motor::M6020_E,  motor::CAN1_E, motor::TWO_E, can1} ,//1ff
        {motor::GM_FRIC_0E, motor::M3508_E,  motor::CAN1_E, motor::THREE_E, can1},//200
};


extern "C" void StartDebugTask(void const * argument)
{
    osDelay(500);
    /* USER CODE BEGIN StartDebugTask */

    //6 can open 相当于完成can初始化工作
    can1.open();
    can2.open();
    uint32_t count = 0;
    
    //8 PID参数初始化设置
    MotorArr[0].ctrl_->setTarget(360.0 * 3,controller::POS_E); //设置目标位置
    MotorArr[0].ctrl_->setPID(2,0,0,controller::POS_E);   //设置pos环 PID
    MotorArr[0].ctrl_->setPID(70,6.5,0,controller::VEC_E); //设置速度环 PID

    for(;;){
        count++;
        else if(count % 10 == 0){
           count = 0;
            ////debug
            //11.debug printf
            printf("ecd:%.2f,%.2f,%.2f\n",MotorArr[0].ctrl_->getDebugParam(controller::POS_E).target_,
                   MotorArr[0].ctrl_->getDebugParam(controller::POS_E).input_,
                   MotorArr[0].ctrl_->getDebugParam(controller::POS_E).output_
            );
        }

        //9.调用控制器的ctrlLoop函数
        MotorArr[0].ctrl_->ctrlLoop();
        
        //10.发送电流
        motor::Motor::sendAllMotorCur();
        osDelay(1);
    }
    /* USER CODE END StartDebugTask */
}

//7.编码循环
extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    can1.read();
    can2.read();
    MotorArr[motor::GM_YAW_E].encoder_.encoderSloveLoop();
}

```



