#pragma once
#include"aes.h"
#include<cstdlib>
#include<cstring>
#include<string>
using namespace std;
class AESService
{
protected:
    AES aes;
    string name;
public:
    AESService(unsigned char* keyBytes,AES::AESType type,const string& name):aes(keyBytes,type),name(name){}
    string Name(){return name;}
    virtual void Enc(unsigned char* out,unsigned char* in,int len)=0;
    virtual void Dec(unsigned char* out,unsigned char* in,int len)=0;
};
class AESServiceIV:public AESService
{
protected:
    unsigned char iv[16];
public:
    AESServiceIV(unsigned char* keyBytes,AES::AESType type,const string& name):AESService(keyBytes,type,name)
    {
        for(int i=0;i<16;i++)iv[i]=rand()&255;
    }
    void IV(unsigned char* out)
    {
        memcpy(out,iv,16);
    }
};
class AESServiceCBC final:public AESServiceIV
{
public:
    AESServiceCBC(unsigned char* keyBytes,AES::AESType type):AESServiceIV(keyBytes,type,"CBC"){}
    void Enc(unsigned char* out,unsigned char* in,int len)final;
    void Dec(unsigned char* out,unsigned char* in,int len)final;
};
class AESServiceCFB final:public AESServiceIV
{
    int r;
public:
    AESServiceCFB(unsigned char* keyBytes,AES::AESType type,int bits=128):AESServiceIV(keyBytes,type,""),r(bits)
    {
        char c[100];
        sprintf(c,"CFB(%dbits)",r);
        name=string(c);
    }
    void Enc(unsigned char* out,unsigned char* in,int len)final;
    void Dec(unsigned char* out,unsigned char* in,int len)final;
};
class AESServiceCTR final:public AESServiceIV
{
public:
    AESServiceCTR(unsigned char* keyBytes,AES::AESType type):AESServiceIV(keyBytes,type,"CTR"){}
    void Enc(unsigned char* out,unsigned char* in,int len)final;
    void Dec(unsigned char* out,unsigned char* in,int len)final;
};