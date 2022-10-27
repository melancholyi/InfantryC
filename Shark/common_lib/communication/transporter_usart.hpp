#ifndef INFANTRYC_TRANSPORTER_USART_HPP
#define INFANTRYC_TRANSPORTER_USART_HPP

#include "main.h"
#include "transporter_interface.hpp"
#include <cstring>
#include "retarget.h"
#include "fixed_packet.hpp"
extern DMA_HandleTypeDef hdma_usart6_rx;
extern UART_HandleTypeDef huart6;
namespace communication {
    /** 模板类，所以只有hpp **/
    template<int capacity>
    class TransporterUsart :public TransportInterface{
    public:
        /** 构造函数 **/
        explicit TransporterUsart(UART_HandleTypeDef &huart) :buf_rx_(),buf_tx_(),huart_(&huart) {
            is_open_ = false;
            received_flag_ = false;
        }

        /** 打开串口 **/
        bool open() override {
            __HAL_UART_ENABLE_IT(huart_,UART_IT_IDLE);
            HAL_UART_Receive_DMA(huart_, (uint8_t*) buf_rx_.getBufPtr(), capacity * 2);
            this->is_open_ = true;
            return true;
        }
        /** 关闭串口 **/
        bool close() override {
            __HAL_UART_DISABLE_IT(huart_,UART_IT_IDLE);
            HAL_UART_DMAStop(huart_);
            this->is_open_ = false;
            return true;
        }
        /** 重启串口 **/
        void reStart() override {
            __HAL_UART_DISABLE(huart_);
            __HAL_DMA_DISABLE(huart_->hdmarx);
            huart_->hdmarx->Instance->NDTR = capacity*2;
            __HAL_DMA_ENABLE(huart_->hdmarx);
            __HAL_UART_ENABLE(huart_);
        }

        /** 中断中读取数据,并放在此类的数据buf_rx中 **/
        int read() override {
            UsartIdleITCallBack();
            return 0;
        }

        /** 通过串口发送数据,仅发送数据，在外部填充数据 **/
        int write() override {
            HAL_UART_Transmit_DMA(huart_, (uint8_t*)buf_tx_.getBufPtr(), capacity);
            return 0;
        }

        /** getter and setter **/
        bool isOpen() override {
            return is_open_;
        }

        [[nodiscard]] bool isRecFlag() const {
            return received_flag_;
        }

        void setRecFlag(bool recFlag) {
            received_flag_ = recFlag;
        }



    private: /**private function**/
        void UsartIdleITCallBack(){
            uint16_t rx_len = 0;
            if(!received_flag_){
                if(__HAL_UART_GET_FLAG(huart_,UART_FLAG_IDLE) != RESET)//idle标志被置位
                {
                    __HAL_UART_CLEAR_IDLEFLAG(huart_);//清除标志位
                    close();//关闭DMA
                    rx_len = capacity * 2 - __HAL_DMA_GET_COUNTER(huart_->hdmarx);// 获取DMA中未传输的数据个数
                    if(rx_len == capacity){
                        ////printf("%d\n",rx_len);
                        received_flag_ = true;
                        /** 需要在继承类中进行解包，然后再打开DMA **/
                        //write((uint8_t *)buf_rx_.getBufPtr());
                    }
                }
            }
        }
        /** member variable **/
    public:
        FixedPacket<capacity*2> buf_rx_; //接受区
        FixedPacket<capacity*2> buf_tx_; //发送区
    private:
        UART_HandleTypeDef* huart_;  /** Hal库封装的指针 **/
        bool is_open_;
        bool received_flag_;
    };

} // communication
#endif //INFANTRYC_TRANSPORTER_USART_HPP
