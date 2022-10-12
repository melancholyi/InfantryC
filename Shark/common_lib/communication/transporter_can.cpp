#include "transporter_can.hpp"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

namespace communication {
    void TransporterCan::filterInit() {
        CAN_FilterTypeDef filter;
        filter.FilterActivation = ENABLE;
        filter.FilterMode = CAN_FILTERMODE_IDMASK;
        filter.FilterScale = CAN_FILTERSCALE_32BIT;
        filter.FilterIdHigh = 0x0000;
        filter.FilterIdLow = 0x0000;
        filter.FilterMaskIdHigh = 0x0000;
        filter.FilterMaskIdLow = 0x0000;

        // which fifo
        if(hcan_==&hcan1) {
            filter.FilterBank = 0;
        }else if(hcan_ == &hcan2){
            filter.SlaveStartFilterBank = 14;
            filter.FilterBank = 14;
        }

        //set fifo rx
        if(fifo_ == FIFO0_E){
            filter.FilterFIFOAssignment = CAN_RX_FIFO0;
        }else if(fifo_ == FIFO1_E) {
            filter.FilterFIFOAssignment = CAN_RX_FIFO1;
        }
        HAL_CAN_ConfigFilter(hcan_, &filter);
    }

    HAL_StatusTypeDef TransporterCan::canActivate() {
        HAL_StatusTypeDef state;
        if(fifo_ == FIFO0_E){
            state = HAL_CAN_ActivateNotification(hcan_,CAN_IT_RX_FIFO0_MSG_PENDING);
        }
        else if(fifo_ == FIFO1_E){
            state = HAL_CAN_ActivateNotification(hcan_,CAN_IT_RX_FIFO1_MSG_PENDING);
        }
        else{ //disable can rx
            state = HAL_CAN_DeactivateNotification(hcan_,CAN_IT_RX_FIFO0_MSG_PENDING);
            state = HAL_CAN_DeactivateNotification(hcan_,CAN_IT_RX_FIFO1_MSG_PENDING);
        }
        return state;
    }

    HAL_StatusTypeDef TransporterCan::canDeactivate() {
        HAL_StatusTypeDef state = HAL_BUSY;
        if(fifo_ == FIFO0_E){
            state = HAL_CAN_DeactivateNotification(hcan_,CAN_IT_RX_FIFO0_MSG_PENDING);
        }
        else if(fifo_ == FIFO1_E){
            state = HAL_CAN_DeactivateNotification(hcan_,CAN_IT_RX_FIFO1_MSG_PENDING);
        }
        return state;
    }


    bool TransporterCan::open() {
        filterInit();
        HAL_StatusTypeDef state;
        state = HAL_CAN_Start(hcan_);
        if(state != HAL_OK){
            return false;
        }
        state = canActivate();
        if(state == HAL_OK){
            is_open_ = true;
            return true;
        }else{
            return false;
        }
    }

    bool TransporterCan::close() {
        HAL_StatusTypeDef state;
        state = HAL_CAN_Stop(hcan_);
        if(state!=HAL_OK){
            return false;
        }
        state = canDeactivate();
        if(state == HAL_OK){
            return true;
            is_open_ = false;
        }else{
            return false;
        }
    }

    bool TransporterCan::isOpen() {
        return is_open_;
    }

    void TransporterCan::reStart() {
        close();
        open();
    }

    int TransporterCan::read() {
        HAL_CAN_GetRxMessage(hcan_,fifo_,&rx_header_,(uint8_t*)buf_rx_.getBufPtr());
        return 0;
    }

    int TransporterCan::write() {
        uint32_t send_mail_box;
        HAL_CAN_AddTxMessage(hcan_, &tx_header_, (uint8_t*)buf_tx_.getBufPtr(), &send_mail_box);
        return 0;
    }

    uint32_t TransporterCan::getRxStdID() const{
        return rx_header_.StdId;
    }

    void TransporterCan::setTxStdID(uint32_t stdID) {
        tx_header_.StdId = stdID;
    }

} // communication

