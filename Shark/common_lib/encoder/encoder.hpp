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
        /** �ӿں��� **/
    public:
        /** ���캯�� **/
        explicit Encoder(communication::TransporterCan& can): can_(&can){
            memset(&encoder_,0,sizeof (encoder_));
            encoder_.isConnect = false;
            memset(&accum_angel_,0,sizeof (accum_angel_));
            stdID_ = 0;
            times_ = 0;
        }
        /** ����loop **/
        void encoderSloveLoop();

        /** �ӿں������õ����������Ϣ **/
        [[nodiscard]] const EncoderMsg &getEncoder() const {
            return encoder_;
        }
        /** ����id **/
        void setStdId(uint32_t stdId);

    private:
        const float ContinueAngelMax =132.0;//�Ƕ��������ȽϽǶ�
        int16_t getDJiMotorData(uint8_t index);
        /** ���ӽǶȣ���Ȧ��� **/
        [[nodiscard]] float getContinueAngel(float last,float now) const;
        /** �Ƕ��ۼ�loop **/
        void allAngelRecoderLoop();

    private:
        uint32_t  stdID_;           /** stdid **/
        EncoderMsg encoder_{};      /** ecd����ṹ�� **/
        EncoderAngelAll accum_angel_{}; /** �Ƕ��ۼӽṹ�� **/
        communication::TransporterCan* can_; /** ָ��can��װ���ָ�� **/
        uint16_t  times_;         /** ���ʧ������,1ms+1 **/
    };

} // controller

#endif //INFANTRYC_ENCODER_HPP
