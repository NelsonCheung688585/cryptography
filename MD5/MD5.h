#ifndef MD5_H
#define MD5_H

#include <vector>
#include <string>

typedef unsigned char byte;
typedef unsigned int word;

class MD5
{
private:
    word A, B, C, D;          // 128-bit的状态的四个组成部分
    word rotateShiftBits[64]; // 每一次操作循环左移的位数
    word salt[64];            // 每一次操作加入的数字
    bool status;
    
public:
    MD5();
    std::string digest(std::vector<byte> raw);
    std::string digest(std::string raw);
private:
    void padding(std::vector<byte> &raw);
    word F(word x, word y, word z);
    word G(word x, word y, word z);
    word H(word x, word y, word z);
    word I(word x, word y, word z);
    std::string convert(word num);
};
#endif