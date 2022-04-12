#include<cstdio>
unsigned char pow1(unsigned char b)
{
    return b;
}
unsigned char pow2(unsigned char b)
{
    return b<<1^(b>>7)*27;
}
unsigned char pow3(unsigned char b)
{
    return pow2(b)^b;
}
unsigned char pow9(unsigned char b)
{
    return pow2(pow2(pow2(b)))^b;
}
unsigned char powb(unsigned char b)
{
    return pow2(pow2(pow2(b)))^pow2(b)^b;
}
unsigned char powd(unsigned char b)
{
    return pow2(pow2(pow2(b)))^pow2(pow2(b))^b;
}
unsigned char powe(unsigned char b)
{
    return pow2(pow2(pow2(b)))^pow2(pow2(b))^pow2(b);
}
int main()
{
    int i,j;
    printf("const unsigned char AesP2[256]=\n{\n");
    for(i=0;i<16;i++,putchar('\n'))for(printf("    "),j=0;j<16;j++)printf("0x%.2x,",pow2(i<<4|j));
    printf("};\n");
    printf("const unsigned char AesP3[256]=\n{\n");
    for(i=0;i<16;i++,putchar('\n'))for(printf("    "),j=0;j<16;j++)printf("0x%.2x,",pow3(i<<4|j));
    printf("};\n");
    printf("const unsigned char AesP9[256]=\n{\n");
    for(i=0;i<16;i++,putchar('\n'))for(printf("    "),j=0;j<16;j++)printf("0x%.2x,",pow9(i<<4|j));
    printf("};\n");
    printf("const unsigned char AesPb[256]=\n{\n");
    for(i=0;i<16;i++,putchar('\n'))for(printf("    "),j=0;j<16;j++)printf("0x%.2x,",powb(i<<4|j));
    printf("};\n");
    printf("const unsigned char AesPd[256]=\n{\n");
    for(i=0;i<16;i++,putchar('\n'))for(printf("    "),j=0;j<16;j++)printf("0x%.2x,",powd(i<<4|j));
    printf("};\n");
    printf("const unsigned char AesPe[256]=\n{\n");
    for(i=0;i<16;i++,putchar('\n'))for(printf("    "),j=0;j<16;j++)printf("0x%.2x,",powe(i<<4|j));
    printf("};\n");
    return 0;
}