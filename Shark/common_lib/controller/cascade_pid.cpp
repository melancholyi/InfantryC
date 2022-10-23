#include "cascade_pid.hpp"

namespace controller {

    void CascadePID::ctrlLoop() {
        pos_pid_.ctrlLoop();
        spd_pid_.setTarget(pos_pid_.getDebugParam().output_);
        spd_pid_.ctrlLoop();
    }

    const DebugParam &CascadePID::getDebugParam(whichPID which) const{
        if(which == POS_E){
            return pos_pid_.getDebugParam();
        }
        else if(which == VEC_E){
            return spd_pid_.getDebugParam();
        }
    }

    void CascadePID::setInputSrc(const float &src, whichPID which) {
        if(which == POS_E){
            pos_pid_.setInputSrc(src);
        }
        else if(which == VEC_E){
            spd_pid_.setInputSrc(src);
        }
    }

    void CascadePID::setPID(float kp, float ki, float kd, whichPID which) {
        if(which == POS_E){
            pos_pid_.setPID(kp,ki,kd);
        }
        else if(which == VEC_E){
            spd_pid_.setPID(kp,ki,kd);
        }
    }

    void CascadePID::setTarget(float tar, whichPID which) {
        if(which == POS_E){
            pos_pid_.setTarget(tar);
        }
        else if(which == VEC_E){
            spd_pid_.setTarget(tar);
        }
    }

    void CascadePID::setOutMaxAndStep(float outMax, float outStep, whichPID which) {
        if(which == POS_E){
            pos_pid_.setOutMaxAndStep(outMax, outStep);
        }
        else if(which == VEC_E){
            spd_pid_.setOutMaxAndStep(outMax, outStep);
        }
    }

    const float& CascadePID::getCurrentOut() const {
        return spd_pid_.getCurrentOut();
    }

    void CascadePID::setPID(float kp, float ki, float kd) {

    }

    void CascadePID::setTarget(float tar) {

    }

    void CascadePID::setInputSrc(const float &src) {

    }

    void CascadePID::setOutMaxAndStep(float outMax, float outStep) {

    }

    const DebugParam &CascadePID::getDebugParam() const {
        static DebugParam debug{};
        debug.input_ = 0;
        return debug;
    }
} // controller