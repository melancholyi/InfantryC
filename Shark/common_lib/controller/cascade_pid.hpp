#ifndef INFANTRYC_CASCADE_PID_HPP
#define INFANTRYC_CASCADE_PID_HPP
#include "general_pid.hpp"

namespace controller {

    class CascadePID :public ControlInterface{
    public:
        //constructor
        CascadePID(float errAllMax1, float outMax1, float outStep1,float errAllMax2, float outMax2, float outStep2):pos_pid_(errAllMax1,outMax1,outStep1),spd_pid_(errAllMax2,outMax2,outStep2){
        }

        void ctrlLoop() override;
        void setPID(float kp, float ki, float kd , whichPID which) override;
        void setTarget(float tar, whichPID which) override;
        void setInputSrc(const float &src, whichPID which) override;
        void setOutMaxAndStep(float outMax, float outStep, whichPID which) override;
        [[nodiscard]] const DebugParam &getDebugParam(whichPID which) const override;
        [[nodiscard]]  const float& getCurrentOut() const override;

        //unuse
        void setPID(float kp, float ki, float kd) override;

        void setTarget(float tar) override;

        void setInputSrc(const float &src) override;

        void setOutMaxAndStep(float outMax, float outStep) override;

        const DebugParam &getDebugParam() const override;
        /** member varible **/
    public:


    private:
        GeneralPID pos_pid_;
        GeneralPID spd_pid_;
        float output_{};
    };

} // controller

#endif //INFANTRYC_CASCADE_PID_HPP
