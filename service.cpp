#include"service.h"
#include<cstring>
#include<cassert>
void AESServiceCBC::Enc(unsigned char* out,unsigned char* in,int len)
{
    assert(len%16==0);
    unsigned char target[16],text[16];
    memcpy(text,iv,16);
    for(int i=0;i<len;i+=16)
    {
        for(int j=0;j<16;j++)target[j]=in[i+j]^text[j];
        aes.Enc(text,target);
        memcpy(out+i,text,16);
    }
}
void AESServiceCBC::Dec(unsigned char* out,unsigned char* in,int len)
{
    assert(len%16==0);
    for(int i=0;i<len;i+=16)aes.Dec(out+i,in+i);
    for(int i=0;i<16;i++)out[i]^=iv[i];
    for(int i=16;i<len;i++)out[i]^=in[i-16];
}
void AESServiceCFB::Enc(unsigned char* out,unsigned char* in,int len)
{
    __uint128_t t=0;
    unsigned char target[16],text[16];
    for(int i=15;~i;i--)t=t<<8|iv[i];
    assert(len*8%r==0);
    memset(out,0,len);
    for(int i=0;i<len<<3;i+=r)
    {
        for(int j=0;j<16;j++)target[j]=t>>(j<<3)&255;
        aes.Enc(text,target);
        if(r<128)t>>=r;
        else t=0;
        for(int j=0;j<r;j++)
        {
            out[i+j>>3]|=((in[i+j>>3]>>(i+j&7)^text[j>>3]>>(j&7))&1)<<(i+j&7);
            t|=(__uint128_t)((in[i+j>>3]>>(i+j&7)^text[j>>3]>>(j&7))&1)<<(j+128-r);
        }
    }
}
void AESServiceCFB::Dec(unsigned char* out,unsigned char* in,int len)
{
    __uint128_t t=0;
    unsigned char target[16],text[16];
    for(int i=15;~i;i--)t=t<<8|iv[i];
    assert(len*8%r==0);
    memset(out,0,len);
    for(int i=0;i<len<<3;i+=r)
    {
        for(int j=0;j<16;j++)target[j]=t>>(j<<3)&255;
        aes.Enc(text,target);
        if(r<128)t>>=r;
        else t=0;
        for(int j=0;j<r;j++)
        {
            out[i+j>>3]|=((in[i+j>>3]>>(i+j&7)^text[j>>3]>>(j&7))&1)<<(i+j&7);
            t|=(__uint128_t)(in[i+j>>3]>>(i+j&7)&1)<<(j+128-r);
        }
    }
}
void AESServiceCTR::Enc(unsigned char* out,unsigned char* in,int len)
{
    __uint128_t t=0;
    unsigned char target[16],text[16];
    for(int i=0;i<16;i++)t=t<<8|iv[i];
    for(int i=0;i<len;i+=16)
    {
        for(int j=0;j<16;j++)target[j]=t+(i>>4)>>((j^15)<<3)&255;
        aes.Enc(text,target);
        for(int j=0;j<16;j++)out[i+j]=in[i+j]^text[j];
    }
}
void AESServiceCTR::Dec(unsigned char* out,unsigned char* in,int len)
{
    __uint128_t t=0;
    unsigned char target[16],text[16];
    for(int i=0;i<16;i++)t=t<<8|iv[i];
    for(int i=0;i<len;i+=16)
    {
        for(int j=0;j<16;j++)target[j]=t+(i>>4)>>((j^15)<<3)&255;
        aes.Enc(text,target);
        for(int j=0;j<16;j++)out[i+j]=in[i+j]^text[j];
    }
}