#include "MD5.h"
#include <cmath>
#include <exception>
#include <iostream>

MD5::MD5()
{
    if (sizeof(word) != 4 || sizeof(byte) != 1 || sizeof(size_t) != 8)
    {
        throw "can not run on this machine";
    }

    word tempRotateShiftBits[64] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

    word tempSalt[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

    for (int i = 0; i < 64; ++i)
    {
        rotateShiftBits[i] = tempRotateShiftBits[i];
        salt[i] = tempSalt[i];
    }
}

std::string MD5::digest(std::vector<byte> raw)
{
    A = 0x67452301;
    B = 0xefcdab89;
    C = 0x98badcfe;
    D = 0x10325476;

    for (int i = 0; i < raw.size(); ++i)
    {
        // std::cout << std::dec << (word)raw[i] << " ";
    }

    //std::cout << "\n";

    padding(raw);

    for (int i = 0; i < raw.size(); ++i)
    {
        if(i && (i % 64 == 0)) {
          //  std::cout << std::endl;
        }
       // std::cout << "(" << std::dec << i << ", "<< std::hex << (word)raw[i] << ") ";
    }

   // std::cout << "\n";
   // std::cout << std::dec << raw.size() << std::hex << (word)raw[112] << " ";

    for (int i = 0; i < raw.size(); i = i + 64)
    {
        word a = A, b = B, c = C, d = D;
        word x, y;

        for (int j = 0; j < 64; ++j)
        {
            if (j >= 0 && j <= 15)
            {
                x = F(b, c, d);
                y = j;
            }
            else if (j >= 16 && j <= 31)
            {
                x = G(b, c, d);
                y = (5 * j + 1) % 16;
            }
            else if (j >= 32 && j <= 47)
            {
                x = H(b, c, d);
                y = (3 * j + 5) % 16;
            }
            else
            {
                x = I(b, c, d);
                y = (7 * j) % 16;
            }

       //  std::cout << std::dec << (i + 4 * y) << " " << std::hex << (((word)raw[i + 4 * y])) << " " << ((word)raw[i + 4 * y + 1]) << " " << ((word)raw[i + 4 * y + 2]) << " " << ((word)raw[i + 4 * y + 3] ) << std::endl;
            x += a + salt[j] + (((word)raw[i + 4 * y]) + ((word)raw[i + 4 * y + 1] << 8) + ((word)raw[i + 4 * y + 2] << 16) + ((word)raw[i + 4 * y + 3] << 24));
            a = d;
            d = c;
            c = b;
            b = b + ((x << rotateShiftBits[j]) | (x >> (32 - rotateShiftBits[j])));

            //std::cout << std::hex <<  << std::endl;
            //std::cout << std::hex << a << " " << b << " " << c << " " << d << std::endl;
        }
        A += a;
        B += b;
        C += c;
        D += d;
    }

    return convert(A) + convert(B) + convert(C) + convert(D);
}

std::string MD5::digest(std::string raw)
{
    std::vector<byte> temp;
    for (int i = 0; i < raw.size(); ++i)
    {
        temp.push_back((byte)raw[i]);
    }

    return digest(temp);
}
void MD5::padding(std::vector<byte> &raw)
{
    size_t length = raw.size() % 64;
    length = length > 56 ? 120 - length : 56 - length;

    std::vector<byte> paddingBytes;
    if (length)
    {
        paddingBytes = std::vector<byte>(length, 0);
        paddingBytes[0] = 0x80;
    }

    length = raw.size() * 8;
    for (int i = 0; i < 8; ++i)
    {
        paddingBytes.push_back(length & 0xff);
        length = length >> 8;
    }

    //std::cout << std::dec << raw.size() << " " << paddingBytes.size() << std::endl;
    raw.insert(raw.end(), paddingBytes.begin(), paddingBytes.end());
}

word MD5::F(word x, word y, word z)
{
    return (x & y) | ((~x) & z);
}

word MD5::G(word x, word y, word z)
{
    return (x & z) | (y & (~z));
}

word MD5::H(word x, word y, word z)
{
    return x ^ y ^ z;
}

word MD5::I(word x, word y, word z)
{
    return y ^ (x | (~z));
}

std::string MD5::convert(word num)
{
    std::string ans = "";
    std::string hexChar = "0123456789abcdef";
    char low, high;

    for (int i = 0; i < 4; ++i)
    {
        low = hexChar[num & 0xf];
        num = num >> 4;
        high = hexChar[num & 0xf];
        num = num >> 4;
        ans.push_back(high);
        ans.push_back(low);
    }

    return ans;
}