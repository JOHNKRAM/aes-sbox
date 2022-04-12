#pragma once
class AES
{
    unsigned char State[4][4];
    unsigned char w[240];
    int Nr;
    void AddRoundKey(int round);
    void SubBytes();
    void InvSubBytes();
    void ShiftRows();
    void InvShiftRows();
    void MixColumns();
    void InvMixColumns();
public:
    enum AESType
    {
        AES128,
        AES192,
        AES256,
    };
    AES(unsigned char* keyBytes,AESType type);
    void Enc(unsigned char* Out,unsigned char* In);
    void Dec(unsigned char* Out,unsigned char* In);
};