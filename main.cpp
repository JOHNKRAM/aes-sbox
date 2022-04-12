#include"aes.h"
#include"service.h"
#include<iostream>
#include<cstdlib>
using namespace std;
unsigned char Code[20971520];
unsigned char Enc[20971520];
unsigned char Dec[20971520];
unsigned char Key[16];
unsigned char IV[16];
int len=32;//Bytes,len=20971520 when test speed
AES::AESType aes_type=AES::AES128;
int key_len;
FILE *out;
void check(AESService* service)
{
    cerr<<service->Name()<<':'<<endl;
    int tt=clock();
    service->Enc(Enc,Code,len);
    cerr<<(len*8)/(double)(clock()-tt)*CLOCKS_PER_SEC/1024/1024<<"Mbps(Encoding)"<<endl;
    tt=clock();
    service->Dec(Dec,Enc,len);
    cerr<<(len*8)/(double)(clock()-tt)*CLOCKS_PER_SEC/1024/1024<<"Mbps(Decoding)"<<endl;
    cerr<<endl;
    for(int i=0;i<len;i++)if(Dec[i]!=Code[i])
    {
        puts("failed");
        break;
    }
    if(len<=2000)//16K
    {
        fprintf(out,"%s:\n",service->Name().c_str());
        fputs("Key: ",out);
        for(int i=0;i<key_len;i++)fprintf(out,"%.2x",Key[i]);
        fputs("\n",out);
        AESServiceIV* service_iv=dynamic_cast<AESServiceIV*>(service);
        if(service_iv!=nullptr)
        {
            fputs("IV: ",out);
            service_iv->IV(IV);
            for(int i=0;i<16;i++)fprintf(out,"%.2x",IV[i]);
            fputs("\n",out);
        }
        fputs("Code(before enc): ",out);
        for(int i=0;i<len;i++)fprintf(out,"%.2x",Code[i]);
        fputs("\n",out);
        fputs("Code(after enc): ",out);
        for(int i=0;i<len;i++)fprintf(out,"%.2x",Enc[i]);
        fputs("\n",out);
        fputs("Code(after dec): ",out);
        for(int i=0;i<len;i++)fprintf(out,"%.2x",Dec[i]);
        fputs("\n",out);
        fputs("\n",out);
    }
    delete service;
}
int main(int argc,char *argv[])
{
    if(argc>1)len=atoi(argv[1]);
    switch(aes_type)
    {
        case AES::AES128:
            key_len=16;
            break;
        case AES::AES192:
            key_len=24;
            break;
        case AES::AES256:
            key_len=32;
            break;
    }
    int m=(-len)&15;
    len+=m;
    if(len<=2000)out=fopen("aes.txt","w");
    for(int i=0;i<len;i++)Code[i]=rand()&255;
    for(int i=len-m;i<len;i++)Code[i]=m;
    for(int i=0;i<key_len;i++)Key[i]=rand()&255;
    check(new AESServiceCBC(Key,aes_type));
    check(new AESServiceCFB(Key,aes_type,8));
    check(new AESServiceCFB(Key,aes_type,16));
    check(new AESServiceCFB(Key,aes_type,32));
    check(new AESServiceCFB(Key,aes_type,64));
    check(new AESServiceCFB(Key,aes_type,128));
    check(new AESServiceCTR(Key,aes_type));
    if(len<=2000)fclose(out);
    return 0;
}
