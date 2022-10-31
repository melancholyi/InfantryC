//#include "gimbal.hpp"
//#include "motor.hpp"
//#include "transporter_usart.hpp"
//#include "can.h"
//
//communication::TransporterCan can1(hcan1, communication::FIFO0_E);
//communication::TransporterCan can2(hcan2, communication::FIFO0_E);
//
//namespace application {
//    Gimbal::Gimbal() :
//            //构造函数 参数化列表初始化控制器
//            mYawCtrl_{100,100,20,1000,30000,3000},
//            mPitchCtrl_{100,100,20,1000,30000,3000},
//            mFricCtrl_{{1000,10000,2000},
//                      {1000,10000,2000}},
//            mRammerCtrl_{1000,10000,2000},
//
//
//            mMotors{{motor::GM_YAW_E, motor::M6020_E,  motor::CAN2_E, motor::SIX_E, can2, &mYawCtrl_},
//                   {motor::GM_PITCH_E, motor::M6020_E,  motor::CAN1_E, motor::FOUR_E, can1, &mPitchCtrl_},
//                   {motor::GM_RAMMER_E, motor::M2006_E,  motor::CAN1_E, motor::SEVEN_E, can1,&mRammerCtrl_},
//                   {motor::GM_FRIC_0E, motor::M3508_E,  motor::CAN1_E, motor::FIVE_E, can1,&mFricCtrl_[0]},
//                   {motor::GM_FRIC_1E, motor::M3508_E,  motor::CAN1_E, motor::SIX_E, can1,&mFricCtrl_[1]}}{
//    }
//
//    void Gimbal::encoderLoop() {
//        //循环调用
//        for(motor::Motor m : mMotors){ //cpp for循环的便捷版本
//            m.encoder_.encoderSloveLoop();
//        }
//    }
//
//    void Gimbal::controlLoop() {
//        for(motor::Motor m : mMotors){
//            m.ctrl_->ctrlLoop();
//        }
//        ////发送电流
//        motor::Motor::sendAllMotorCur();
//    }
//} // application