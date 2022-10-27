#include "motor.hpp"

namespace motor {
    /** 静态变量初始化 **/
    bool Motor::config_error = false;
    std::vector<sCanSendMsg> Motor::sendMsg_;
    privateNS::sBuffer *Motor::ptrCurrents [6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}; //六种类型的指针
    communication::TransporterCan* Motor::ptrCan1_ = nullptr;
    communication::TransporterCan* Motor::ptrCan2_ = nullptr;


    void Motor::sloveID() {
        if(type_ == M3508_E){
            maxCur_ = privateNS::MAXCUR_M3508_E;
            recID_ = 0x200 + id_;
            if(id_>=ONE_E && id_<=FOUR_E)
                sendID_ = 0x200 ;
            if(id_>=FIVE_E && id_<=EIGHT_E)
                sendID_ = 0x1FF ;
        }
        else if(type_ == M2006_E){
            maxCur_ = privateNS::MAXCUR_M2006_E;
            recID_ = 0x200 + id_;
            if(id_>=ONE_E && id_<=FOUR_E)
                sendID_ = 0x200 ;
            if(id_>=FIVE_E && id_<=EIGHT_E)
                sendID_ = 0x1FF ;
        }
        else if(type_ == M6020_E){
            maxCur_ = privateNS::MAXCUR_M6020_E;
            recID_ = 0x204 + id_;
            if(id_>=ONE_E && id_<=FOUR_E)
                sendID_ = 0x1FF ;
            if(id_>=FIVE_E && id_<=EIGHT_E)
                sendID_ = 0x2FF ;
        }
    }

    [[maybe_unused]] void Motor::setDirectCur(float directCur) {
        direct_cur_ = limltCur(directCur);
    }

    ////静态函数，发送所有电流值
    void Motor::sendAllMotorCur() {
        if(config_error){
            printf("config error!!!!\n");
        }
        else{
            for(sCanSendMsg msg : sendMsg_){
                //printf("setCurrent:%d\n",(int16_t)*msg.current);
                /** 将电机电流信息填充到buffer数组中并设置stdid **/
                if(ptrCurrents[msg.index] != nullptr){
                    ptrCurrents[msg.index]->buffer[msg.begin] = (int16_t)(*msg.current)>>8;
                    ptrCurrents[msg.index]->buffer[msg.begin + 1] = (int16_t)(*msg.current)&0xff;
                    ptrCurrents[msg.index]->stdId = msg.stdId;
                }
                //printf("ptrCur:%d\n",(int16_t)(ptrCurrents[msg.index1][msg.index2].buffer[msg.begin]<<8 | ptrCurrents[msg.index1][msg.index2].buffer[msg.begin + 1]));
            }
            /** debug begin**/
//        for(unsigned char i : group)
//            printf("group:%d\n",i);
//        for(auto msg : sendMsg_){
//            printf("msg:%d,%d,%d\n",msg.index1,msg.index2,msg.begin);
//        }
            /** debug end**/
            for(int index = 0 ; index < 6 ; index++){
                if(index % 2 == 0){ //can1
                    if(ptrCan1_!= nullptr){
                        /** 设置发送id **/
                        ptrCan1_->setTxStdID(ptrCurrents[index]->stdId);
                        /** 复制电流数据 **/
                        memcpy((uint8_t*)ptrCan1_->buf_tx_.getBufPtr(),ptrCurrents[index]->buffer,sizeof(ptrCurrents[index]->buffer));
                        /** can发送 **/
                        ptrCan1_->write();
                    }
                }
                else{ //can2 发送
                    if(ptrCan2_!= nullptr){
                        ptrCan2_->setTxStdID(ptrCurrents[index]->stdId);
                        memcpy((uint8_t*)ptrCan2_->buf_tx_.getBufPtr(),ptrCurrents[index]->buffer,sizeof(ptrCurrents[index]->buffer));
                        ptrCan2_->write();
                    }
                }
            }
        }
    }


    Motor::~Motor() {
        if(sendMsg_.empty()){
            sendMsg_.~vector();
            for(int i = 0 ;i<6;i++){
                if(ptrCurrents[i]!= nullptr){
                    delete [] ptrCurrents[i];
                }
            }
        }
    }


} // controller