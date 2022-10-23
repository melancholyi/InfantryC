#include "general_pid.hpp"

namespace controller {

    GeneralPID::GeneralPID(float errAllMax, float outMax, float outStep, float rampTarStep, ePID_RampState state) {
        memset(&PID, 0, sizeof (PID));
        PID.Input = nullptr;
        PID.ErrAllMax = errAllMax;
        PID.OutMax = outMax;
        PID.OutStep = outStep;
        PID.RampTarget = rampTarStep;
        PID.State_RampOrNormal = state;
    }

    GeneralPID::GeneralPID(float errAllMax, float outMax, float outStep) {
        memset(&PID, 0, sizeof (PID));
        PID.Input = nullptr;
        PID.ErrAllMax = errAllMax;
        PID.OutMax = outMax;
        PID.OutStep = outStep;
        PID.State_RampOrNormal = Normal_e;
    }

    void GeneralPID::ctrlLoop() {
        if(PID.Input!= nullptr){
            updateParam();
            calculate();
        }
    }

    void GeneralPID::setPID(float p, float i, float d) {
        PID.Kp1 = p;
        PID.Ki1 = i;
        PID.Kd1 = d;
    }

    void GeneralPID::setTarget(float tar) {
        if(PID.State_RampOrNormal == Normal_e){
            PID.Target = tar;
        }
        else if(PID.State_RampOrNormal == Ramp_e){
            PID.RampTarget = tar;
        }
    }

    void GeneralPID::setInputSrc(const float &src) {
        PID.Input = (float*)&src;
    }


    void GeneralPID::setOutMaxAndStep(float outMax, float outStep) {
        PID.OutStep = outStep;
        PID.OutMax = outMax;
    }

    void GeneralPID::updateParam() {
        if(PID.State_RampOrNormal == Ramp_e){
            if(PID.RampCountTime < PID.RampTartgetTime){
                ++ PID.RampCountTime;
            }else{
                PID.RampCountTime = 0;
                if(PID.Target <  PID.RampTarget ){//б�º�����û�����꣬
                    PID.Target += PID.RampTartgetStep;
                    if(PID.Target >= PID.RampTarget){
                        PID.Target = PID.RampTarget;
                        PID.State_RampOrNormal = Normal_e;
                    }
                }
                else if(PID.Target >  PID.RampTarget){
                    PID.Target -= PID.RampTartgetStep;
                    if(PID.Target <= PID.RampTarget){
                        PID.Target = PID.RampTarget;
                        PID.State_RampOrNormal = Normal_e;
                    }
                }else{//б�º��������Ѿ���ɣ�Ҫ�˳�б�º���ģʽ
                    PID.State_RampOrNormal = Normal_e;
                }
            }
        }
        PID.Err_lastlast = PID.Err_last;
        PID.Err_last = PID.Err_now;
        PID.Err_now = PID.Target - *PID.Input;

        if( PID.Err_now < PID.Precision && PID.Err_now > -PID.Precision )
            PID.Err_now = 0;
        PID.Err_all += PID.Err_now;
        if( PID.Err_all > PID.ErrAllMax){
            PID.Err_all = PID.ErrAllMax;
        }
        else if( PID.Err_all < -PID.ErrAllMax){
            PID.Err_all = -PID.ErrAllMax;
        }
    }

    float GeneralPID::calculate() {
        //λ��ʽPID����ɢ���㹫ʽ
        PID.Out = PID.Kp1 * PID.Err_now + PID.Kd1 * (PID.Err_now - PID.Err_last);
        PID.Out += ( PID.Err_all * PID.Ki1 );

        //����޷�
        if(PID.Out >= PID.OutMax)
            PID.Out = PID.OutMax;
        if(PID.Out <= -PID.OutMax)
            PID.Out = -PID.OutMax;

        //���΢���޷�
        if(PID.Out - PID.lastout > PID.OutStep)
        {
            PID.Out = PID.lastout + PID.OutStep;
        }
        if(PID.Out - PID.lastout < -PID.OutStep)
        {
            PID.Out = PID.lastout + -PID.OutStep;
        }
        //���ݲ���
        PID.lastout = PID.Out;
        //���out
        return PID.Out;
    }

    const DebugParam &GeneralPID::getDebugParam() const {
        static DebugParam debug;
        debug.target_ = PID.Target;
        debug.input_ = *PID.Input;
        debug.output_ = PID.Out;
        return debug;
    }

    void GeneralPID::setPID(float kp, float ki, float kd, whichPID which) {

    }

    void GeneralPID::setTarget(float tar, whichPID which) {

    }

    void GeneralPID::setInputSrc(const float &src, whichPID which) {

    }

    void GeneralPID::setOutMaxAndStep(float outMax, float outStep, whichPID which) {

    }

    const DebugParam &GeneralPID::getDebugParam(whichPID which) const {
        static DebugParam debug{};
        debug.input_ = 0;
        return debug;
    }

    const float& GeneralPID::getCurrentOut() const {
        return PID.Out;
    }


} // controller