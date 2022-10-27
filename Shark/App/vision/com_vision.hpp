#ifndef INFANTRYC_COM_VISION_HPP
#define INFANTRYC_COM_VISION_HPP
#include "transporter_usart.hpp"

namespace communication {

    /** 自定义串口通信数据 **/
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
    /** 自定义串口发送结构体 **/
    struct SendToVision{

    };
    //TODO:暂时串口通信先用8位数据包进行测试，后续实际开发时候，需要更改
    /** 应用层--视觉通信 , 继承transporter_usart中间数据传输层，进一步开发 **/
    class ComVision : public TransporterUsart<8>{
    public:
        /** 构造函数 **/
        explicit ComVision(UART_HandleTypeDef &huart);

        /** 数据接口函数 **/
        [[nodiscard]] const VisionCtrlMsg &getVisionMsg() const;

        /** 发送包 **/
        void sendPacket();

        /** 解包 **/
        void slovePacket();

    private:

    private:
        VisionCtrlMsg vision_msg_{};
        SendToVision send_msg_;
    };

} // communication

#endif //INFANTRYC_COM_VISION_HPP
