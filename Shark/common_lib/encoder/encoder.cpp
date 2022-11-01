#include "encoder.hpp"

namespace motor {
/**
 * encoderLoop放在can中断中
 */
    void Encoder::encoderSloveLoop() {
        times_ ++;
        if(times_>1000){ //1000次没有收到，就设置为false 表示电机离线
            encoder_.isConnect = false;
        }
        if(can_->getRxStdID() == stdID_){
            encoder_.isConnect = true;
            times_ = 0; //discount flag
            encoder_.angel =  (float)(((float)getDJiMotorData(0)) * 360.0 / 8192.0);
            encoder_.speed = ((float)getDJiMotorData(2))/reduction_ratio_;
            encoder_.torque = getDJiMotorData(4);
            encoder_.tempature = can_->buf_rx_.getBufPtr()[6];
            allAngelRecoderLoop();
        }
    }

    float Encoder::getContinueAngel(float last, float now) const {
        float Incre = 0;
        Incre = now - last;
        if(Incre >= 0 && Incre < ContinueAngelMax){//正转
            Incre += 0;
            return Incre;}
        if(Incre < -ContinueAngelMax){//正转，并且不连续
            Incre += 360.0;
            return Incre;
        }
        if(Incre <= 0 && Incre > -ContinueAngelMax){//反转
            Incre += 0;
            return Incre;
        }
        if(Incre > ContinueAngelMax){//反转，并且不连续
            Incre -= 360.0;
            return Incre;
        }
        return  Incre;
    }

    void Encoder::allAngelRecoderLoop() {
        accum_angel_.lastAngel = accum_angel_.nowAngel;
        accum_angel_.nowAngel = encoder_.angel;
        accum_angel_.increAngel  = getContinueAngel(accum_angel_.lastAngel,accum_angel_.nowAngel);
        accum_angel_.allAngel +=  accum_angel_.increAngel;
        encoder_.angelAll = accum_angel_.allAngel/reduction_ratio_;
    }

    int16_t Encoder::getDJiMotorData(uint8_t index) {
        return  (int16_t)(can_->buf_rx_.getBufPtr()[index] << 8 | can_->buf_rx_.getBufPtr()[index+1]);
    }

    void Encoder::setStdId(uint32_t stdId) {
        stdID_ = stdId;
    }

    void Encoder::setReductionRatio(float ratio) {
        reduction_ratio_ = ratio;
    }
} // controller