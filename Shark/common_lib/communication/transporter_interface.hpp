#ifndef INFANTRYC_TRANSPORTER_INTERFACE_HPP
#define INFANTRYC_TRANSPORTER_INTERFACE_HPP
#include <cstdint>
namespace communication{
    /**
     * 虚基类 描述了通信类一般的操作函数
     */
    class TransportInterface{
    public:
        virtual bool open() = 0;
        virtual bool close() = 0;
        virtual bool isOpen() = 0;
        virtual void reStart() = 0;
        virtual int read() = 0;
        virtual int write() = 0;
    };
}
#endif //INFANTRYC_TRANSPORTER_INTERFACE_HPP
