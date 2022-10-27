#ifndef INFANTRYC_MOTOR_HPP
#define INFANTRYC_MOTOR_HPP
#include "encoder.hpp"
#include "cascade_pid.hpp"
#include "general_pid.hpp"
#include "control_interface.hpp"
#include <vector>

namespace privateNS{
    enum eBuferIndex{
        CAN1_0x200_I0 = 0,
        CAN2_0x200_I1 = 1,
        CAN1_0x1FF_I2 = 2,
        CAN2_0x1FF_I3 = 3,
        CAN1_0x2FF_I4 = 4,
        CAN2_0x2FF_I5 = 5
    };
    struct sBuffer{
        uint32_t stdId ;
        uint8_t buffer[8]{} ;
        sBuffer(){
            stdId = 0;
            memset(buffer,0, sizeof(buffer));
        }
    };

    enum eMotorMaxCur{
        MAXCUR_M3508_E = 16384,
        MAXCUR_M2006_E = 10000,
        MAXCUR_M6020_E = 30000
    };

    enum eCanSendStdID{
        SEND_0X1FF_E = 0x1FF,
        SEND_0X2FF_E = 0x2FF,
        SEND_0X200_E = 0x200
//        M6020_SendID_1_4_e =	0x1FF,
//        M6020_SendID_5_7_e =	0x2FF,
//        M3508_SendID_1_4_e =	0x200,
//        M3508_SendID_5_8_e	=	0x1FF,
//        M2006_SendID_1_4_e =	0x200,
//        M2006_SendID_5_8_e	=	0x1FF
    };
};

namespace motor {
    enum eMotorType{
        M3508_E = 0,
        M2006_E = 1,
        M6020_E = 2
    };
    /** 自定义 **/
    enum eMotorApp{
        GM_YAW_E = 0,
        GM_PITCH_E = 1,
        GM_RAMMER_E = 2,
        GM_FRIC_0E = 3,
        GM_FRIC_1E = 4,

        LEG_LT_E = 0,
        LEG_LB_E = 1,
        LEG_RT_E = 2,
        LEG_RB_E = 3,

        DRV_LEFT_E = 0,
        DRV_RIGHT_E = 1,
    };
    enum eCanX{
        CAN1_E = 0,
        CAN2_E = 1
    };
    enum eCANID{
        ONE_E = 1,
        TWO_E = 2,
        THREE_E = 3,
        FOUR_E = 4,
        FIVE_E = 5,
        SIX_E = 6,
        SEVEN_E = 7,
        EIGHT_E = 8
    };

    /** 发送数据结构体,保存发送电流时需要的信息 **/
    struct sCanSendMsg{
        eCanX can; //can
        uint32_t stdId;     /** 发送的 stdid **/
        privateNS::eBuferIndex index; /** 六种发送组合类型的index **/
        uint8_t id;         //// 设置的电机ID
        uint8_t begin;      //// int16_t类型电流在uint8_t arr[8]中放置的位置起始位置
        float *current;     //// 指向电流地址的指针，可以通过  &current  绑定地址
        sCanSendMsg(eCanX can, uint32_t stdId, uint16_t id, const float &current = 0 ) : can(can), stdId(stdId), id(id),
                                                                                           current((float*)&current) {
            /** 通过id计算应该在buffer[8] 的数据 **/
            begin = (2*((id-1)%4));

            /** 设置index **/
            if(can == CAN1_E){
                if(stdId == 0x200){
                    index = privateNS::CAN1_0x200_I0;
                }else if(stdId == 0x1FF){
                    index = privateNS::CAN1_0x1FF_I2;
                }else if(stdId == 0x2FF){
                    index = privateNS::CAN1_0x2FF_I4;
                }
            }
            else if(can == CAN2_E){
                if(stdId == 0x200){
                    index = privateNS::CAN2_0x200_I1;
                }else if(stdId == 0x1FF){
                    index = privateNS::CAN2_0x1FF_I3;
                }else if(stdId == 0x2FF){
                    index = privateNS::CAN2_0x2FF_I5;
                }
            }
        }
    };

    class Motor {
    /** public function **/
    public:
        /**?
         * @brief:motor constructor
         * @param app   ?: motor application
         * @param type  ?: Dji motor type
         * @param can   ?: Motor connect which can
         * @param id    ?: Dji motor can id
         * @param can   ?: encoder can
         * @param ctrlPtr ?: ctrl ptr interface
         */
        Motor(eMotorApp app, eMotorType type, eCanX canx, eCANID id,//// normal msg
              communication::TransporterCan& can,//// encoder can
              controller::ControlInterface *ctrlPtr = nullptr )://// ptr interface
                    ///这用到了cpp多态的思想，来把双环和单环封装在一个类
              /** init list: ??**/
                    app_(app),type_(type),can_(canx),id_(id),
                    encoder_(can),
                    ctrl_(ctrlPtr){
            /** constructor function body:?? **/
            sloveID();
            encoder_.setStdId(recID_);     /** 设置电调反馈的stdid,用来编码 **/
            direct_cur_ = 0.0;

            /** 判断是否配置错误 **/
            for(sCanSendMsg msg : sendMsg_){
                /** can相同 stdid相同 摆放的位置也相同,协议上冲突 **/
                if(msg.stdId == sendID_ && msg.can == can_ && msg.begin == (2*((id-1)%4))){
                    config_error  = true;
                    break;
                }
            }
            /** 没有出错，才把这个电机放入电流发送的vector中 **/
            if(!config_error){
                if(ctrl_ != nullptr){
                    /** 绑定PID控制类的输入来源，默认使用电机编码数据 **/
                    /** 双环时候生效 **/
                    ctrl_->setInputSrc(encoder_.getEncoder().angelAll,controller::POS_E);
                    ctrl_->setInputSrc(encoder_.getEncoder().speed,controller::VEC_E);

                    /** 速度环环时候生效 **/
                    ctrl_->setInputSrc(encoder_.getEncoder().speed);

                    /** 将电机需要通过can发送的信息保存到static vector var中 **/
                    sCanSendMsg temp(can_, sendID_, id_, ctrl_->getCurrentOut());
                    sendMsg_.push_back(temp);

                    //动态分配空间 电流发送的buffer
                    ptrCurrents[temp.index] = new privateNS::sBuffer;
                }

                else{ //// 没控制，则把直接设置的电流值地址绑定为发送来源
                    sCanSendMsg temp(can_, sendID_, id_, direct_cur_);
                    sendMsg_.push_back(temp);

                    //// 动态分配空间 电流发送的buffer
                    ptrCurrents[temp.index] = new privateNS::sBuffer;
                }

                //// 绑定发送电流的can  指针指向transporter_can类地址
                if(ptrCan1_ == nullptr && can_ == CAN1_E){
                    ptrCan1_ = &can;
                }
                else if(ptrCan1_ == nullptr && can_ == CAN2_E){
                    ptrCan2_ = &can;
                }
            }
        }

        /**
         * Motor 析构函数
         */
        virtual ~Motor();

        /**
         * 静态函数，发送所有实例化对象的电流
         */
        static void sendAllMotorCur();



        /**
         * 直接设置电机输出的电流
         * @param directCur : 电流值
         */
        [[maybe_unused]] void setDirectCur(float directCur);


        /** private function **/
        static std::vector<sCanSendMsg> sendMsg_;
    private:

        void sloveID();
        inline float limltCur(float cur){
            float input = cur;
            if(input > (float)maxCur_) input = (float)maxCur_ - 500;
            else if(input < -(float)maxCur_) input = -((float)maxCur_ - 500);
            return input;
        };

    /** public member variable **/
    public:
        controller::ControlInterface* ctrl_;
        Encoder encoder_;
    /** private member variable **/
    private:
        eMotorApp app_;
        eMotorType type_;
        eCanX can_;
        eCANID id_;

        privateNS::eMotorMaxCur maxCur_;
        uint32_t sendID_{};
        uint32_t  recID_{};
        float direct_cur_;

        static communication::TransporterCan* ptrCan1_;
        static communication::TransporterCan* ptrCan2_;
        //// 记录所有点击需要发送的目的地和电流信息
        static privateNS::sBuffer *ptrCurrents[6]; //六种类型的指针
        static bool config_error;
    };

} // controller

#endif //INFANTRYC_MOTOR_HPP
