#include "motor.hpp"

namespace motor {
    bool Motor::config_error = false;
    std::vector<sCanSendMsg> Motor::sendMsg_;
    privateNS::sBuffer *Motor::ptrCurrents [6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}; //六种类型的指针
    bool Motor::flag_ = false;
    uint8_t Motor::group[6] = {0};
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
        direct_cur_ = directCur;
    }

    //静态函数，发送所有电流值
    void Motor::sendAllMotorCur() {
        if(config_error){
            printf("config error!!!!\n");
        }
        else{
            if(!flag_){ //使其只运行一次，寻找到电流放置的准确位置
                for(int now = 0; now < sendMsg_.size(); now++){
                    group[sendMsg_[now].index1] = 1;
                    //同一can 同一stdid 同一begin 则只能多加一组
                    for(int pre = 0; pre < now; pre++){
                        if(sendMsg_[now].index1 == sendMsg_[pre].index1 && sendMsg_[now].begin == sendMsg_[pre].begin){
                            sendMsg_[now].index2 = sendMsg_[pre].index2 + 1;
                            group[sendMsg_[now].index1]++;
                        }
                    }
                }
                flag_ = true;
                //动态分配空间
                for(uint8_t i = 0; i<6 ;i++){
                    if(group[i]>0){
                        ptrCurrents[i] = new privateNS::sBuffer[group[i]];
                    }
                }
            }
            for(sCanSendMsg msg : sendMsg_){
                //printf("setCurrent:%d\n",(int16_t)*msg.current);
                ptrCurrents[msg.index1][msg.index2].buffer[msg.begin] = (int16_t)(*msg.current)>>8;
                ptrCurrents[msg.index1][msg.index2].buffer[msg.begin + 1] = (int16_t)(*msg.current)&0xff;
                ptrCurrents[msg.index1][msg.index2].stdId = msg.stdId;
                //printf("ptrCur:%d\n",(int16_t)(ptrCurrents[msg.index1][msg.index2].buffer[msg.begin]<<8 | ptrCurrents[msg.index1][msg.index2].buffer[msg.begin + 1]));
            }
            /** debug begin**/
//        for(unsigned char i : group)
//            printf("group:%d\n",i);
//        for(auto msg : sendMsg_){
//            printf("msg:%d,%d,%d\n",msg.index1,msg.index2,msg.begin);
//        }
            /** debug end**/

            for(int index1 = 0 ; index1 < 6 ; index1++){
                for(int index2 = 0 ; index2 < group[index1] ;index2++){
                    if(index1 % 2 == 0){ //can1
                        if(ptrCan1_!= nullptr){
                            ptrCan1_->setTxStdID(ptrCurrents[index1][index2].stdId);
                            memcpy((uint8_t*)ptrCan1_->buf_tx_.getBufPtr(),ptrCurrents[index1][index2].buffer,sizeof(ptrCurrents[index1][index2].buffer));
                            //printf("bufer size:%d\n",sizeof(ptrCurrents[index1][index2].buffer));
                            //printf("stdId:%lu\n",ptrCurrents[index1][index2].stdId);
//                        for(int pos = 0; pos<8;pos=pos+2){
//                            printf("cur:%d ",(int16_t)(ptrCan1_->buf_tx_.getBufPtr()[pos]<<8|ptrCan1_->buf_tx_.getBufPtr()[pos+1]));
//                        }
//                        printf("\n");
                            ptrCan1_->write();
                        }
                    }
                    else{ //can2
                        if(ptrCan2_!= nullptr){
                            ptrCan2_->setTxStdID(ptrCurrents[index1][index2].stdId);
                            memcpy((uint8_t*)ptrCan2_->buf_tx_.getBufPtr(),ptrCurrents[index1][index2].buffer,sizeof(ptrCurrents[index1][index2].buffer));
                            ptrCan2_->write();
                        }
                    }
                }
        }
        }

        ////debug current set is correct
//        for(int index1 = 0 ; index1 < 6 ; index1++){
//            for(int index2 = 0 ; index2 < group[index1] ;index2++){
//                for(int pos = 0; pos<8;pos=pos+2){
//                    printf("current:%d\n",(int16_t )(ptrCurrents[index1][index2].buffer[pos]<<8 | ptrCurrents[index1][index2].buffer[pos+1]));
//                }
//            }
//        }
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