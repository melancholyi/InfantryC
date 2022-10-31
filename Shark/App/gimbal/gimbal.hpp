//#ifndef INFANTRYC_GIMBAL_HPP
//#define INFANTRYC_GIMBAL_HPP
//#include "motor.hpp"
//#include "cascade_pid.hpp"
////#include "general_pid.hpp"
//
//namespace application {
//
//    class Gimbal {
//    public:
//        explicit  Gimbal();
//        void encoderLoop();
//        void controlLoop();
//    private:
//
//    private: //私有的原因是，motor构造的时候，已经与motor绑定在一起，今后可以通过motor的ctrl_成员变量来访问控制器
//        ////控制方案声明 //TODO:注意ctrl成员变量要先于motors成员变量声明
//        controller::CascadePID mYawCtrl_;
//        controller::CascadePID mPitchCtrl_;
//        controller::GeneralPID mRammerCtrl_;
//        controller::GeneralPID mFricCtrl_[2];
//
//
//    public:
//        ////电机数组声明
//        motor::Motor mMotors[5]; //初始化顺序按照枚举类型，这样就可以随机访问
//
//    };
//
//} // application
//
//#endif //INFANTRYC_GIMBAL_HPP
