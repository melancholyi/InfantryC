#include "com_vision.hpp"

namespace communication {
    ComVision::ComVision(UART_HandleTypeDef &huart) : TransporterUsart(huart) {
        memset(&vision_msg_,0,sizeof (vision_msg_));
        memset(&send_msg_,0,sizeof (send_msg_));
    }

    const VisionCtrlMsg &ComVision::getVisionMsg() const {
        return vision_msg_;
    }

    void ComVision::slovePacket() {
        if(isRecFlag()){
            buf_rx_.getData(0,vision_msg_.head);
            buf_rx_.getData(7,vision_msg_.tail);

            /**reopen DMA usart**/
            setRecFlag(false);
            TransporterUsart<8>::open();
        }
    }

    void ComVision::sendPacket() {
        buf_tx_.setData(0,0xFF);
        buf_tx_.setData(8,0x0D);
        TransporterUsart<8>::write();
    }



} // communication