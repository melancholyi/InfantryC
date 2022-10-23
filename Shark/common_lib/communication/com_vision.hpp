#ifndef INFANTRYC_COM_VISION_HPP
#define INFANTRYC_COM_VISION_HPP
#include "transporter_usart.hpp"

namespace communication {

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
    struct SendToVision{

    };
    //TODO:暂时串口通信先用8位数据包进行测试，后续实际开发时候，需要更改
    class ComVision : public TransporterUsart<8>{
    public:
        explicit ComVision(UART_HandleTypeDef &huart);

        [[nodiscard]] const VisionCtrlMsg &getVisionMsg() const;

        void sendPacket();
        void slovePacket();

    private:

    private:
        VisionCtrlMsg vision_msg_{};
        SendToVision send_msg_;
    };

} // communication

#endif //INFANTRYC_COM_VISION_HPP
