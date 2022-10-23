#include "motor.hpp"

namespace motor {
    std::vector<sCanSendMsg> Motor::sendMsg_;
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

    void Motor::setDirectCur(float directCur) {
        direct_cur_ = directCur;
    }
} // controller