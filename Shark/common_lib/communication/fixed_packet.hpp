#ifndef INFANTRYC_FIXED_PACKET_HPP
#define INFANTRYC_FIXED_PACKET_HPP
#include <cstdio>
#include <cstring>
namespace communication {
    template <int capacity>
    class FixedPacket {
    public:
        FixedPacket(){
            memset(buffer_,0,sizeof (buffer_));
            //ptr = new uint8_t(capacity);
        }
//        ~FixedPacket(){
//            delete [] ptr;
//        }
        template<typename type>
        bool setData(uint16_t start,const type data){
            if(start < 0 || start > capacity-1){
                return false;
            }else{
                memcpy(buffer_ + start, &data, sizeof (data));
                return true;
            }
        }

        /**
         * get data from buffer
         * @tparam type the type of data
         * @param start the begin pos of data in buffer
         * @param data  buffer-->data
         * @return  is sucess
         */
        template<typename type>
        bool getData(uint16_t start, type& data){
            if(start < 0 || start > capacity-1){
                return false;
            }else{
                memcpy(&data, buffer_ + start, sizeof (data));
//                data = (uint16_t)((buffer_)[0] << 8 | (buffer_)[1]);
                return true;
            }
        }

        void clear(){
            memset(buffer_,0,sizeof (buffer_));
        }

        [[nodiscard]] const uint8_t *getBufPtr() const {
            return buffer_;
        }

    private:
        uint8_t buffer_[capacity]{};
        //uint8_t* ptr;
    };

} // communication

#endif //INFANTRYC_FIXED_PACKET_HPP
