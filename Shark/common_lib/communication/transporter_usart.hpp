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
    /** ģ���࣬����ֻ��hpp **/
    template<int capacity>
    class TransporterUsart :public TransportInterface{
    public:
        /** ���캯�� **/
        explicit TransporterUsart(UART_HandleTypeDef &huart) :buf_rx_(),buf_tx_(),huart_(&huart) {
            is_open_ = false;
            received_flag_ = false;
        }

        /** �򿪴��� **/
        bool open() override {
            __HAL_UART_ENABLE_IT(huart_,UART_IT_IDLE);
            HAL_UART_Receive_DMA(huart_, (uint8_t*) buf_rx_.getBufPtr(), capacity * 2);
            this->is_open_ = true;
            return true;
        }
        /** �رմ��� **/
        bool close() override {
            __HAL_UART_DISABLE_IT(huart_,UART_IT_IDLE);
            HAL_UART_DMAStop(huart_);
            this->is_open_ = false;
            return true;
        }
        /** �������� **/
        void reStart() override {
            __HAL_UART_DISABLE(huart_);
            __HAL_DMA_DISABLE(huart_->hdmarx);
            huart_->hdmarx->Instance->NDTR = capacity*2;
            __HAL_DMA_ENABLE(huart_->hdmarx);
            __HAL_UART_ENABLE(huart_);
        }

        /** �ж��ж�ȡ����,�����ڴ��������buf_rx�� **/
        int read() override {
            UsartIdleITCallBack();
            return 0;
        }

        /** ͨ�����ڷ�������,���������ݣ����ⲿ������� **/
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
                if(__HAL_UART_GET_FLAG(huart_,UART_FLAG_IDLE) != RESET)//idle��־����λ
                {
                    __HAL_UART_CLEAR_IDLEFLAG(huart_);//�����־λ
                    close();//�ر�DMA
                    rx_len = capacity * 2 - __HAL_DMA_GET_COUNTER(huart_->hdmarx);// ��ȡDMA��δ��������ݸ���
                    if(rx_len == capacity){
                        ////printf("%d\n",rx_len);
                        received_flag_ = true;
                        /** ��Ҫ�ڼ̳����н��н����Ȼ���ٴ�DMA **/
                        //write((uint8_t *)buf_rx_.getBufPtr());
                    }
                }
            }
        }
        /** member variable **/
    public:
        FixedPacket<capacity*2> buf_rx_; //������
        FixedPacket<capacity*2> buf_tx_; //������
    private:
        UART_HandleTypeDef* huart_;  /** Hal���װ��ָ�� **/
        bool is_open_;
        bool received_flag_;
    };

} // communication
#endif //INFANTRYC_TRANSPORTER_USART_HPP
