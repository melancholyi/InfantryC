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
//    private: //˽�е�ԭ���ǣ�motor�����ʱ���Ѿ���motor����һ�𣬽�����ͨ��motor��ctrl_��Ա���������ʿ�����
//        ////���Ʒ������� //TODO:ע��ctrl��Ա����Ҫ����motors��Ա��������
//        controller::CascadePID mYawCtrl_;
//        controller::CascadePID mPitchCtrl_;
//        controller::GeneralPID mRammerCtrl_;
//        controller::GeneralPID mFricCtrl_[2];
//
//
//    public:
//        ////�����������
//        motor::Motor mMotors[5]; //��ʼ��˳����ö�����ͣ������Ϳ����������
//
//    };
//
//} // application
//
//#endif //INFANTRYC_GIMBAL_HPP
