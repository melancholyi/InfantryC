#include "code_specification.hpp"

////类文件全小写 + 下划线

/** ycy的一些些想法
 * 整体就是抽象分模块，并且分层，构建中间层应用工具类，从而实现一个基本框架，以此提高代码复用性和稳定性以及可读性，
 * 然后每个兵种只需要开发自己的应用层就可以
 *
 * TODO：cpp特性的应用：↓
 * 封装：抽象，这个是我们最需要做的，抽象
 * 继承：线性传递，可以参考我 transporter_usart
 * 组合：类组合，我的motor类就是本体一些特征 + Encoder类 + ctrl类 + can通信组合成的一个类
 *              例如 底盘类 = motor类  + 串口通信类 + can通信类 + 底盘behavior类构成
 *              再例如 云台类 = motor类 + 视觉通信 + can通信 + 云台反应(模式切换 + 卡单翻转....)
 * 多态: 可以看一下我的motor中的ctrl整合方案
 */


namespace yourNamespace {

    //TODO:小技巧，写好函数之后，在函数上方 输入 ： /** 然后点回车就会自动出现下方的东西，需要自己描述函数作用和输入输出参数
    /**
     * //TODO :写函数简介
     * @param mVal1     : 介绍参数1
     * @param mPubVal2  : 介绍参数2
     * @attention       : //TODO:可以增加attention等说明
     */
    cCodeSpecification::cCodeSpecification(float mVal1, int mPubVal2) : mVal1(mVal1), mPubVal2(mPubVal2) {}

    const sMyStruct &cCodeSpecification::getMStructVal() const {
        return mStructVal;
    }

    void cCodeSpecification::setMStructVal(const sMyStruct &mStructVal) {
        cCodeSpecification::mStructVal = mStructVal;
    }

    void cCodeSpecification::sloveNum(float aa) {

    }

    void cCodeSpecification::ctrlLoop() {

    }

    float cCodeSpecification::getSum(int num1, int num2, int num3) {
        return 0;
    }

    float cCodeSpecification::getSum(int num1, int num2, float num3) {
        return 0;
    }

    float cCodeSpecification::getSum(int num1, int num2) {
        return 0;
    }
} // yourNamespace