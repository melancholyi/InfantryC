#include "code_specification.hpp"

////���ļ�ȫСд + �»���

/** ycy��һЩЩ�뷨
 * ������ǳ����ģ�飬���ҷֲ㣬�����м��Ӧ�ù����࣬�Ӷ�ʵ��һ��������ܣ��Դ���ߴ��븴���Ժ��ȶ����Լ��ɶ��ԣ�
 * Ȼ��ÿ������ֻ��Ҫ�����Լ���Ӧ�ò�Ϳ���
 *
 * TODO��cpp���Ե�Ӧ�ã���
 * ��װ�������������������Ҫ���ģ�����
 * �̳У����Դ��ݣ����Բο��� transporter_usart
 * ��ϣ�����ϣ��ҵ�motor����Ǳ���һЩ���� + Encoder�� + ctrl�� + canͨ����ϳɵ�һ����
 *              ���� ������ = motor��  + ����ͨ���� + canͨ���� + ����behavior�๹��
 *              ������ ��̨�� = motor�� + �Ӿ�ͨ�� + canͨ�� + ��̨��Ӧ(ģʽ�л� + ������ת....)
 * ��̬: ���Կ�һ���ҵ�motor�е�ctrl���Ϸ���
 */


namespace yourNamespace {

    //TODO:С���ɣ�д�ú���֮���ں����Ϸ� ���� �� /** Ȼ���س��ͻ��Զ������·��Ķ�������Ҫ�Լ������������ú������������
    /**
     * //TODO :д�������
     * @param mVal1     : ���ܲ���1
     * @param mPubVal2  : ���ܲ���2
     * @attention       : //TODO:��������attention��˵��
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