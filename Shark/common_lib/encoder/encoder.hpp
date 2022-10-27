#ifndef INFANTRYC_ENCODER_HPP
#define INFANTRYC_ENCODER_HPP
#include "transporter_can.hpp"
#include <cstring>
#include <cstdio>

namespace motor {

    struct EncoderMsg{
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
    public:
        explicit Encoder(communication::TransporterCan& can): can_(&can){
            memset(&encoder_,0,sizeof (encoder_));
            memset(&accum_angel_,0,sizeof (accum_angel_));
            stdID_ = 0;
        }
        /**
         * ????????
         */
        void encoderSloveLoop();

        [[nodiscard]] const EncoderMsg &getEncoder() const {
            return encoder_;
        }

        void setStdId(uint32_t stdId);

    private:
        const float ContinueAngelMax =132.0;//角度连续化比较角度
        int16_t getDJiMotorData(uint8_t index);
        [[nodiscard]] float getContinueAngel(float last,float now) const;
        void allAngelRecoderLoop();

    private:
        uint32_t  stdID_;
        EncoderMsg encoder_{};
        EncoderAngelAll accum_angel_{};
        communication::TransporterCan* can_;
    };

} // controller

#endif //INFANTRYC_ENCODER_HPP
