#ifndef INFANTRYC_MOTOR_HPP
#define INFANTRYC_MOTOR_HPP
#include "encoder.hpp"
#include "cascade_pid.hpp"
#include "general_pid.hpp"
#include "control_interface.hpp"
#include <vector>

namespace privateNS{
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
    enum eCtrlWay{
        VC_E = 0,
        PC_E = 1,
        NO_E = 2
    };
    struct sCanSendMsg{
        eCanX can;
        uint32_t stdId;
        uint16_t id;
        float *current;
        sCanSendMsg(eCanX can, uint32_t stdId, uint16_t id, const float &current = 0 ) : can(can), stdId(stdId), id(id),
                                                                                           current((float*)&current) {}
    };

    class Motor {
    /** public function **/
    public:
        Motor(eMotorApp app, eMotorType type, eCanX can, eCANID id, //normal msg
              communication::TransporterCan& encoder,    //±àÂëecd
              controller::ControlInterface *ctrlPtr = nullptr ):  //is use imu as speed encoder
              /** init list: ??????????**/
                    app_(app),type_(type),can_(can),id_(id),
                    encoder_(encoder),
                    ctrl_(ctrlPtr){
            /** constructor function body:?? **/
            sloveID();
            encoder_.setStdId(recID_);
            direct_cur_ = 0.0;
            if(ctrl_ != nullptr){
                ctrl_->setInputSrc(encoder_.getEncoder().angelAll,controller::POS_E);
                ctrl_->setInputSrc(encoder_.getEncoder().speed,controller::VEC_E);
                ctrl_->setInputSrc(encoder_.getEncoder().speed);
                sCanSendMsg temp(can_, sendID_, id_, ctrl_->getCurrentOut());
                sendMsg_.push_back(temp);
            }
            else{
                sCanSendMsg temp(can_, sendID_, id_, direct_cur_);
                sendMsg_.push_back(temp);
            }
        }

        void setDirectCur(float directCur);

        /** private function **/
    private:
        void sloveID();

    /** public member variable **/
    public:
        controller::ControlInterface* ctrl_;
        Encoder encoder_;
        static std::vector<sCanSendMsg> sendMsg_;
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
    };

} // controller

#endif //INFANTRYC_MOTOR_HPP
