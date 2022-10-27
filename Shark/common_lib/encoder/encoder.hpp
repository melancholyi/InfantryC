#ifndef INFANTRYC_ENCODER_HPP
#define INFANTRYC_ENCODER_HPP
#include "transporter_can.hpp"
#include <cstring>
#include <cstdio>

namespace motor {

    struct EncoderMsg{
        bool isConnect;
        uint32_t stdId;
        float angelAll;
        float angel;
        float speed;
        int16_t torque;
        uint8_t tempature;
    };

    struct EncoderAngelAll{
        float nowAngel;
        float lastAngel;
        float increAngel;
        float allAngel;
    };

    class Encoder {
        /** 接口函数 **/
    public:
        /** 构造函数 **/
        explicit Encoder(communication::TransporterCan& can): can_(&can){
            memset(&encoder_,0,sizeof (encoder_));
            encoder_.isConnect = false;
            memset(&accum_angel_,0,sizeof (accum_angel_));
            stdID_ = 0;
            times_ = 0;
        }
        /** 编码loop **/
        void encoderSloveLoop();

        /** 接口函数，得到电机编码信息 **/
        [[nodiscard]] const EncoderMsg &getEncoder() const {
            return encoder_;
        }
        /** 设置id **/
        void setStdId(uint32_t stdId);

    private:
        const float ContinueAngelMax =132.0;//角度连续化比较角度
        int16_t getDJiMotorData(uint8_t index);
        /** 增加角度，过圈检查 **/
        [[nodiscard]] float getContinueAngel(float last,float now) const;
        /** 角度累加loop **/
        void allAngelRecoderLoop();

    private:
        uint32_t  stdID_;           /** stdid **/
        EncoderMsg encoder_{};      /** ecd编码结构体 **/
        EncoderAngelAll accum_angel_{}; /** 角度累加结构体 **/
        communication::TransporterCan* can_; /** 指向can封装类的指针 **/
        uint16_t  times_;         /** 电机失联次数,1ms+1 **/
    };

} // controller

#endif //INFANTRYC_ENCODER_HPP
