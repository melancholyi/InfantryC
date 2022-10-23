#ifndef INFANTRYC_GENERAL_PID_HPP
#define INFANTRYC_GENERAL_PID_HPP
#include <cstdio>
#include <cstring>
#include "control_interface.hpp"

namespace controller {
    typedef enum {
        Normal_e = 0,                //PID工作在正常状态
        Ramp_e = 1,                    //PID工作在斜坡函数状态
        Ramping_Add = 2,        //斜坡还没完成，并且是加
        Ramping_Reduce = 3,    //斜坡还没完成，并且是减
    } ePID_RampState;

    struct sGeneralPID {
        float Kp1;                                //P
        float Ki1;                                //I
        float Kd1;                                //D

        float Err_now;                //当前误差
        float Err_last;                //上一次误差
        float Err_lastlast;        //上上次误差
        float Err_all;                //误差积分
        float Target;                    //目标
        float *Input;                    //输入
        float Out;                        //输出
        float lastout;                //上一次输出
        ePID_RampState State_RampOrNormal;//PID状态
        float RampTarget;                     //斜坡函数的目标
        float RampCountTime;             //斜坡计数，与 Target作比较来决定是否增加一个步幅
        float RampTartgetTime;         //计数多少次才把count加上1个步幅
        float RampTartgetStep;         //斜坡函数步幅
        float ErrAllMax;            //PID积分限幅
        float OutMax;                    //PID输出限幅
        float OutStep;                //PID输出步幅限制
        float Precision;            //PID最小精度
    };




    class GeneralPID :public ControlInterface{
        /** public function **/
    public:
        //ramper state constructor
        GeneralPID(float errAllMax, float outMax, float outStep, float rampTarStep, ePID_RampState state);
        //ramper state constructor
        GeneralPID(float errAllMax, float outMax, float outStep);

        //
        void ctrlLoop() override;
        void setPID(float p, float i,float d) override;
        void setTarget(float tar) override ;
        void setInputSrc(const float& src) override;
        void setOutMaxAndStep(float outMax , float outStep) override;
        [[nodiscard]] const DebugParam &getDebugParam() const override;


        void setPID(float kp, float ki, float kd, whichPID which) override;
        void setTarget(float tar, whichPID which) override;
        void setInputSrc(const float& src, whichPID which) override;
        void setOutMaxAndStep(float outMax, float outStep, whichPID which) override;
        [[nodiscard]] const DebugParam &getDebugParam(whichPID which) const override;

        [[nodiscard]] const float& getCurrentOut() const override;
        /** private function **/
    private:
        void updateParam();
        float calculate();

        /** private member variable **/
    private:
        sGeneralPID PID{};

        /** public member variable **/
    public:
    }; // controller
}
#endif //INFANTRYC_GENERAL_PID_HPP
