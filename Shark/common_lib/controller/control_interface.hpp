#ifndef INFANTRYC_CONTROL_INTERFACE_HPP
#define INFANTRYC_CONTROL_INTERFACE_HPP

namespace controller {

    enum whichPID{
        POS_E = 0,
        VEC_E = 1,
    };

    struct DebugParam{
        float target_;
        float input_;
        float output_;
    };

    class ControlInterface {
    public:

        virtual void ctrlLoop() = 0;
        //interface to param
        virtual void setPID(float kp, float ki, float kd , whichPID which) = 0;
        virtual void setTarget(float tar, whichPID which) = 0;
        virtual void setInputSrc(const float &src , whichPID which) = 0;
        virtual void setOutMaxAndStep(float outMax, float outStep, whichPID which) = 0;
        [[nodiscard]] virtual const DebugParam &getDebugParam(whichPID which) const = 0;

        //–È∫Ø ˝÷ÿ‘ÿ
        virtual void setPID(float kp, float ki, float kd ) = 0;
        virtual void setTarget(float tar) = 0;
        virtual void setInputSrc(const float &src) = 0;
        virtual void setOutMaxAndStep(float outMax, float outStep) = 0;
        [[nodiscard]] virtual const DebugParam &getDebugParam() const = 0;
        [[nodiscard]] virtual const float& getCurrentOut() const = 0;
    };
} // controller

#endif //INFANTRYC_CONTROL_INTERFACE_HPP
