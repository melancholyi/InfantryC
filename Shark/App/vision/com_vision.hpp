#ifndef INFANTRYC_COM_VISION_HPP
#define INFANTRYC_COM_VISION_HPP
#include "transporter_usart.hpp"

namespace communication {

    /** �Զ��崮��ͨ������ **/
    struct VisionCtrlMsg{
        uint8_t head;
        uint8_t tail;
        uint8_t check;

        uint32_t packet_id;
        float yaw_target;
        float pitch_target;
        uint8_t mode;
        uint8_t cmd;
    };
    /** �Զ��崮�ڷ��ͽṹ�� **/
    struct SendToVision{

    };
    //TODO:��ʱ����ͨ������8λ���ݰ����в��ԣ�����ʵ�ʿ���ʱ����Ҫ����
    /** Ӧ�ò�--�Ӿ�ͨ�� , �̳�transporter_usart�м����ݴ���㣬��һ������ **/
    class ComVision : public TransporterUsart<8>{
    public:
        /** ���캯�� **/
        explicit ComVision(UART_HandleTypeDef &huart);

        /** ���ݽӿں��� **/
        [[nodiscard]] const VisionCtrlMsg &getVisionMsg() const;

        /** ���Ͱ� **/
        void sendPacket();

        /** ��� **/
        void slovePacket();

    private:

    private:
        VisionCtrlMsg vision_msg_{};
        SendToVision send_msg_;
    };

} // communication

#endif //INFANTRYC_COM_VISION_HPP
