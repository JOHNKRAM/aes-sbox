#include<bits/stdc++.h>
#include<omp.h>
using namespace std;
bool check(int x)
{
    int i,j,k,l;
    for(i=1;i<5;i++)for(j=0;j<1<<i;j++)
    {
        for(k=x,l=8;l>=i;l--)if(k>>l)k^=1<<l^j<<l-i;
        if(!k)return 0;
    }
    return 1;
}
int mul2(int x,int p)
{
    return x>>7&1?x<<1^p:x<<1;
}
int mul(int x,int y,int p)
{
    int s=0;
    for(;y;y>>=1,x=mul2(x,p))if(y&1)s^=x;
    return s;
}
int i,j,k,n,a[30];
int inv[30][256];
int rotl[256][256];
int o[256];
int S(int x,int y,int z,int w)
{
    return rotl[y][inv[x][w]]^z;
}
vector<vector<int>> gen_diff(int x,int y,int z)
{
    vector<vector<int>> d(256,vector<int>(256));
    int i,j;
    for(i=0;i<256;i++)for(j=0;j<256;j++)d[i^j][S(x,y,z,i)^S(x,y,z,j)]++;
    return d;
}
const vector<pair<int,int>> d0{make_pair(0,129),make_pair(2,126),make_pair(4,1)};
void check_diff(int x,int y,int z=0)
{
    vector<vector<int>> d=gen_diff(x,y,z);
    map<int,int> m;
    int i,j;
    for(i=1;i<256;i++)
    {
        m.clear();
        for(j=0;j<256;j++)m[d[i][j]]++;
        assert((vector<pair<int,int>>(m.begin(),m.end()))==d0);
    }
}
vector<vector<int>> gen_linear(int x,int y,int z)
{
    vector<vector<int>> l(256,vector<int>(256,-128));
    int i,j,k;
    for(i=0;i<256;i++)for(j=0;j<256;j++)for(k=0;k<256;k++)if(!o[(i&k)^(j&S(x,y,z,k))])l[i][j]++;
    return l;
}
vector<vector<pair<int,int>>> l0(256);
void check_linear(int x,int y,int z=0)
{
    vector<vector<int>> l=gen_linear(x,y,z);
    map<int,int> m;
    m.clear();
    int i,j;
    for(i=0;i<256;i++)
    {
        //for(j=0;j<256;j++)m[l[i][j]]++;
        for(j=0;j<256;j++)m[abs(l[i][j])]++;
    }
    cout<<x<<' '<<y<<' '<<z<<endl;
    for(auto p:m)cout<<p.first<<' '<<p.second<<endl;
    /*if(l0[z].empty())l0[z]=vector<pair<int,int>>(m.begin(),m.end());
    else assert((vector<pair<int,int>>(m.begin(),m.end()))==l0[z]);*/
    /*if(l0[!!z].empty())l0[!!z]=vector<pair<int,int>>(m.begin(),m.end());
    else assert((vector<pair<int,int>>(m.begin(),m.end()))==l0[!!z]);*/
}
void out_sbox(int x,int y,int z)
{
    cout<<"S-Box:"<<endl;
    int i,j;
    for(i=0;i<16;i++,cout<<endl)for(j=0;j<16;j++)printf("0x%.2x,",S(x,y,z,i<<4|j));
}
void out_inv_sbox(int x,int y,int z)
{
    cout<<"Inverse S-Box:"<<endl;
    int i,j,p[256];
    for(i=0;i<256;i++)p[S(x,y,z,i)]=i;
    for(i=0;i<16;i++,cout<<endl)for(j=0;j<16;j++)printf("0x%.2x,",p[i<<4|j]);
}
void out_diff(int x,int y,int z)
{
    vector<vector<int>> d=gen_diff(x,y,z);
    int i,j;
    cout<<"Difference Distribution Table:"<<endl;
    printf("    ");
    for(i=0;i<256;i++)printf(" 0x%.2x",i);
    cout<<endl;
    for(i=0;i<256;i++,cout<<endl)for(printf("0x%.2x",i),j=0;j<256;j++)printf("%5d",d[i][j]);
}
void out_linear(int x,int y,int z)
{
    vector<vector<int>> l=gen_linear(x,y,z);
    int i,j;
    cout<<"Linear Approaximation Table:"<<endl;
    printf("    ");
    for(i=0;i<256;i++)printf(" 0x%.2x",i);
    cout<<endl;
    for(i=0;i<256;i++,cout<<endl)for(printf("0x%.2x",i),j=0;j<256;j++)printf("%5d",l[i][j]);
    map<int,int> m;
    for(i=0;i<256;i++)for(j=0;j<256;j++)m[l[i][j]]++;
    for(auto p:m)cout<<p.first<<' '<<p.second<<endl;
}
void out(int x,int y,int z)
{
    out_sbox(x,y,z);
    out_inv_sbox(x,y,z);
    out_diff(x,y,z);
    out_linear(x,y,z);
}
int main(int argc,char *argv[])
{
    FILE* _=freopen("sbox.out","w",stdout);
    for(i=256;i<512;i++)if(check(i))a[n++]=i;
    for(i=0;i<30;i++)
    {
        for(j=1;j<256;j++)for(k=1;k<256;k++)if(mul(j,k,a[i])==1)
        {
            inv[i][j]=k;
            break;
        }
        inv[i][0]=0;
    }
    for(i=0;i<256;i++)
    {
        if(i)o[i]=o[i>>1]^(i&1);
        else o[i]=0;
        if(o[i])for(j=0;j<8;j++)if(i>>j&1)for(k=0;k<256;k++)rotl[i][k]^=k<<j&255|k>>8-j;
    }
    //for(i=0;i<30;i++)for(j=0;j<256;j++)if(o[j])check_diff(i,j);
    /*for(i=0;i<30;i++)for(j=0;j<256;j++)if(o[j])
    {
        #pragma omp parallel for num_threads(32)
        for(k=0;k<256;k++)check_linear(i,j,k);
    }*/
    //for(k=0;k<256;k++)check_linear(0,1,k);
    //for(k=0;k<2;k++)check_linear(0,1,k);
    /*int s=0;
    for(i=0;i<30;i++)for(j=0;j<256;j++)if(o[j])for(k=0;k<256;k++)
    {
        int w;
        vector<int> p(256);
        for(w=0;w<256;w++)p[w]=S(i,j,k,w);
        int t=0;
        w=0;
        do
        {
            t++;
            w=p[w];
        }while(w);
        if(t==256)
        {
            s++;
            cout<<i<<' '<<j<<' '<<k<<endl;
        }
    }
    cout<<s<<endl;*/
    //int x=0,y=0x1f,z=0x63;
    int x=0,y=0x1f,z=0x15;
    if(argc>1)
    {
        x=atoi(argv[1]);
    }
    if(argc>2)
    {
        y=atoi(argv[2]);
    }
    if(argc>3)
    {
        z=atoi(argv[3]);
    }
    assert(x>=0&&x<30);
    assert(y>=0&&y<256);
    assert(o[y]);
    assert(z>=0&&z<256);
    out(x,y,z);
    return 0;
}