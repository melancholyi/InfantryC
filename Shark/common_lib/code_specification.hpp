#ifndef INFANTRYC_CODE_SPECIFICATION_HPP
#define INFANTRYC_CODE_SPECIFICATION_HPP

namespace yourNamespace { /** �����ռ䣬ͬһ��ģ�����ͬһ�������ռ䣬ʹ�ü�൥�� **/

    /** 1 ö�����Ͷ����� **/
    enum eMyEnum{ //// e��ͷ + ���շ� ��ʾ��ö��
        ENUM1_E = 0, ////��д + �»��� + _E��׺
        ENUM2_E = 1,
        ENUM3_E = 2
    };

    /** 2 �ṹ�嶨��ȥ **/
    struct sMyStruct{
        /** TODO������ṹ���еĳ�Ա����,��ʽ�����Ǻ����������ϲ��С�շ� **/
        float var1;
        int var2;
        eMyEnum var3 ;

        /** cpp �нṹ��Ҳ�й��캯�� ��������һ��**/
        sMyStruct(){

        }

    };



    /** 3 �ඨ���� **/
    class cCodeSpecification {  /** ��һ�࣬������ͷ��ĸ��д����ǰ��� c ǰ׺����ʾΪ���Զ�������Ϊ�� **/
    /** ��Ϊ�Ĳ��֣����д����� ˽�д����� ���������� ˽�������� **/
        /** ���к����� **/
    public:
        /** ���к�������h����֯�����ã�Ȼ��������֮��alt + insert generatԴ�ļ�����**/

        /** 1 ���캯�� **/
        //// ����������������
        /// clion ����ͨ��alt + insert ���ɴ���
        cCodeSpecification(float mVal1, int mPubVal2);

        /** 2 ��������������һ���ò��������������Ҫstl����new��̬�����˿ռ䣬��������������delete **/


        /** ��������ν�� + ������ʽ����������ĸ��Ҫ��д�������� **/
        void sloveNum(float aa);

        /** ��ѭ��������Loop��׺ **/
        void ctrlLoop();

        /** �ṹ������������������ֵ�ǣ�ֱ��ʹ������ **/
        /** !!!! ���ҳ�Ա������getter �� setter ����ֱ��alt + insert���ɣ�������������**/
        const sMyStruct &getMStructVal() const; ////�Զ���const�������޸����Ա����
        void setMStructVal(const sMyStruct &mStructVal); ////�ṹ��ʹ��&�������ͣ��������ݸ�������ʱ��

        ////���о����������ú�ָ��(��Ȼ��Ҫ�Ƚ�����)

        ////���ú�������
        float getSum(int num1, int num2, int num3);
        float getSum(int num1, int num2, float num3);
        float getSum(int num1, int num2);


        /** ˽�к����� **/
    private:
        /** Ҫ��һ�£������������ʹ�ã�һ����ظ�����̫���ĺ�����װ���� **/


        /** ���������� **/
    public:
        /** ��Ա����ǰ��mǰ׺ָʾΪ��ĳ�Ա���� TODO:���Ǻ������»��߻����շ壬��ȶһ��,�������� m+���շ� **/
        float mVal1;
        int mPubVal2;
        sMyStruct mStructVal;

        /** ˽�������� **/
    private:

    };

} // yourNamespace

#endif //INFANTRYC_CODE_SPECIFICATION_HPP
