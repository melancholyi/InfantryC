#ifndef INFANTRYC_NULL_PID_HPP
#define INFANTRYC_NULL_PID_HPP
#include "control_interface.hpp"
namespace controller {

    class NullPID : public ControlInterface{
    public:
        float a = 0.0;
        DebugParam debug{};

        NullPID() {
            a = 0;
            debug.input_ = 0.0;
            debug.output_ = 0.0;
            debug.output_ = 0.0;
        }

        void ctrlLoop() override {

        }

        void setPID(float kp, float ki, float kd, whichPID which) override {

        }

        void setTarget(float tar, whichPID which) override {

        }

        void setInputSrc(const float &src, whichPID which) override {

        }

        void setOutMaxAndStep(float outMax, float outStep, whichPID which) override {

        }

        [[nodiscard]] const DebugParam &getDebugParam(whichPID which) const override {
            return debug;
        }

        void setPID(float kp, float ki, float kd) override {

        }

        void setTarget(float tar) override {

        }

        void setInputSrc(const float &src) override {

        }

        void setOutMaxAndStep(float outMax, float outStep) override {

        }

        [[nodiscard]] const DebugParam &getDebugParam() const override {
            return debug;
        }

        [[nodiscard]] const float &getCurrentOut() const override {
            return a;
        }

    };

} // controller

#endif //INFANTRYC_NULL_PID_HPP
