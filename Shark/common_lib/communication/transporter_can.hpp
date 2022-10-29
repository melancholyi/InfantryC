#ifndef INFANTRYC_TRANSPORTER_CAN_HPP
#define INFANTRYC_TRANSPORTER_CAN_HPP
#include "main.h"
#include <cstring>
#include "retarget.h"
#include "transporter_interface.hpp"
#include "fixed_packet.hpp"

namespace communication {
    enum CanRxFIFOx{
        FIFO0_E = CAN_RX_FIFO0,
        FIFO1_E = CAN_RX_FIFO1,
        NONE = 3
    };

    class TransporterCan : public TransportInterface{
    public:
        /********** constructor **********/
        /**
         *
         * @param hcan
         * @param fifo
         */
        explicit TransporterCan(CAN_HandleTypeDef &hcan,CanRxFIFOx fifo) : buf_tx_(),buf_rx_(),hcan_(&hcan),fifo_(fifo){
            memset(&rx_header_,0,sizeof (rx_header_));
            memset(&tx_header_,0,sizeof (tx_header_));
            tx_header_.IDE = CAN_ID_STD;
            tx_header_.RTR = CAN_RTR_DATA;
            tx_header_.DLC = 0x08;
            is_open_ = false;
        }
        /********** implement **********/
        bool open() override;
        bool close() override;
        bool isOpen() override;
        void reStart() override;
        int read() override;
        int write() override;

        /********** extern function *********/
        void setTxStdID(uint32_t stdID);
        [[nodiscard]] uint32_t getRxStdID() const;

    private:
        void filterInit();
        HAL_StatusTypeDef canActivate();
        HAL_StatusTypeDef canDeactivate();
        /********************** member variable **********************/
    public:
        FixedPacket<8> buf_tx_;
        FixedPacket<8> buf_rx_;

    private:
        CAN_HandleTypeDef *hcan_;
        CAN_RxHeaderTypeDef rx_header_{};
        CAN_TxHeaderTypeDef tx_header_{};
        CanRxFIFOx fifo_;
        bool is_open_;
    };
} // communication

#endif //INFANTRYC_TRANSPORTER_CAN_HPP
