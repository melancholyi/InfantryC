#ifndef INFANTRYC_GENERAL_PID_HPP
#define INFANTRYC_GENERAL_PID_HPP
#include <cstdio>
#include <cstring>
#include "control_interface.hpp"

namespace controller {
    typedef enum {
        Normal_e = 0,                //PID����������״̬
        Ramp_e = 1,                    //PID������б�º���״̬
        Ramping_Add = 2,        //б�»�û��ɣ������Ǽ�
        Ramping_Reduce = 3,    //б�»�û��ɣ������Ǽ�
    } ePID_RampState;

    struct sGeneralPID {
        float Kp1;                                //P
        float Ki1;                                //I
        float Kd1;                                //D

        float Err_now;                //��ǰ���
        float Err_last;                //��һ�����
        float Err_lastlast;        //���ϴ����
        float Err_all;                //������
        float Target;                    //Ŀ��
        float *Input;                    //����
        float Out;                        //���
        float lastout;                //��һ�����
        ePID_RampState State_RampOrNormal;//PID״̬
        float RampTarget;                     //б�º�����Ŀ��
        float RampCountTime;             //б�¼������� Target���Ƚ��������Ƿ�����һ������
        float RampTartgetTime;         //�������ٴβŰ�count����1������
        float RampTartgetStep;         //б�º�������
        float ErrAllMax;            //PID�����޷�
        float OutMax;                    //PID����޷�
        float OutStep;                //PID�����������
        float Precision;            //PID��С����
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
