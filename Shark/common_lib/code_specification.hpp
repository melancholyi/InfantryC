#ifndef INFANTRYC_CODE_SPECIFICATION_HPP
#define INFANTRYC_CODE_SPECIFICATION_HPP

namespace yourNamespace { /** 命名空间，同一个模块放在同一个命名空间，使用简洁单词 **/

    /** 1 枚举类型定义区 **/
    enum eMyEnum{ //// e开头 + 大驼峰 表示是枚举
        ENUM1_E = 0, ////大写 + 下划线 + _E后缀
        ENUM2_E = 1,
        ENUM3_E = 2
    };

    /** 2 结构体定义去 **/
    struct sMyStruct{
        /** TODO：这个结构体中的成员数据,格式还不是很清楚，个人喜欢小驼峰 **/
        float var1;
        int var2;
        eMyEnum var3 ;

        /** cpp 中结构体也有构造函数 可以利用一下**/
        sMyStruct(){

        }

    };



    /** 3 类定义区 **/
    class cCodeSpecification {  /** 单一类，类名开头字母大写且最前面加 c 前缀，表示为此自定义类型为类 **/
    /** 分为四部分，共有代码区 私有代码区 共有数据区 私有数据区 **/
        /** 共有函数区 **/
    public:
        /** 所有函数现在h中组织构建好，然后声明好之后，alt + insert generat源文件函数**/

        /** 1 构造函数 **/
        //// ！！！！！！！！
        /// clion 可以通过alt + insert 生成代码
        cCodeSpecification(float mVal1, int mPubVal2);

        /** 2 析构函数，我们一般用不到，但是如果需要stl过着new动态分配了空间，就在析构函数中delete **/


        /** 函数采用谓词 + 宾语形式，并且首字母不要大写，例如下 **/
        void sloveNum(float aa);

        /** 主循环函数带Loop后缀 **/
        void ctrlLoop();

        /** 结构体做变量或者做返回值是，直接使用引用 **/
        /** !!!! 并且成员变量的getter 和 setter 可以直接alt + insert生成，这样是最合理的**/
        const sMyStruct &getMStructVal() const; ////自动带const，不让修改类成员变量
        void setMStructVal(const sMyStruct &mStructVal); ////结构体使用&引用类型，减少数据副本复制时间

        ////还有就是善用引用和指针(当然需要比较熟练)

        ////善用函数重载
        float getSum(int num1, int num2, int num3);
        float getSum(int num1, int num2, float num3);
        float getSum(int num1, int num2);


        /** 私有函数区 **/
    private:
        /** 要求一致，这里就是类内使用，一般把重复或者太长的函数封装起来 **/


        /** 共有数据区 **/
    public:
        /** 成员变量前加m前缀指示为类的成员变量 TODO:但是后续用下划线还是驼峰，商榷一下,我倾向于 m+大驼峰 **/
        float mVal1;
        int mPubVal2;
        sMyStruct mStructVal;

        /** 私有数据区 **/
    private:

    };

} // yourNamespace

#endif //INFANTRYC_CODE_SPECIFICATION_HPP
