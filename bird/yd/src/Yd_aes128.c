/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:
  Author:       Version:        Date: 2013-05-29
  Description:   YD business data processing module
  Others:
  Function List:
  History:
    1. Date:
       Author:
       Modification:
****************************************************************************/
#include "Bird_app.h"
#include "Yd_aes128.h"

//S-box transformation table

static U8 s_box[256] =
{
    // 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, // 0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, // 1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, // 2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, // 3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, // 4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, // 5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, // 6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, // 7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, // 8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, // 9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, // a
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, // b
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, // c
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, // d
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, // e
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};// f

// Inverse S-box transformation table

static U8 inv_s_box[256] =
{
    // 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, // 0
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, // 1
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, // 2
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, // 3
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, // 4
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, // 5
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, // 6
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, // 7
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, // 8
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, // 9
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, // a
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, // b
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, // c
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, // d
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, // e
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};// f

void sub_word(U8 *w)
{

    U8 i;

    for (i = 0; i < 4; i++)
    {
        w[i] = s_box[16*((w[i] & 0xf0) >> 4) + (w[i] & 0x0f)];
    }
}


// Function used in the Key Expansion routine that takes a four-byte word and performs a cyclic permutation.

void rot_word(U8 *w)
{

    U8 tmp;
    U8 i;

    tmp = w[0];

    for (i = 0; i < 3; i++)
    {
        w[i] = w[i+1];
    }

    w[3] = tmp;
}
// Addition of 4 byte words m(x) = x4+1

void coef_add(U8 a[], U8 b[], U8 d[])
{

    d[0] = a[0]^b[0];
    d[1] = a[1]^b[1];
    d[2] = a[2]^b[2];
    d[3] = a[3]^b[3];
}
U8 gmult(U8 a, U8 b) //01 02  04  08 10 20 40  80  1B 36
{

    U8 p = 0;
    U8 i = 0;
    U8 hbs = 0;

    for (i = 0; i < 8; i++)
    {
        if (b & 1)
        {
            p ^= a;
        }

        hbs = a & 0x80;
        a <<= 1;
        if (hbs) a ^= 0x1b; // 0000 0001 0001 1011
        b >>= 1;
    }

    return (U8)p;
}


// Generates the round constant Rcon[i]



U8 * Rcon(U16 i)
{
    U8 R[] = {0x02, 0x00, 0x00, 0x00};
    if (i == 1)
    {
        R[0] = 0x01; // x^(1-1) = x^0 = 1
    }
    else if (i > 1)
    {
        R[0] = 0x02;
        i--;
        while (i-1 > 0)
        {
            R[0] = gmult(R[0], 0x02);
            i--;
        }
    }

    return R;
}


// Key Expansion

void key_expansion(U8 *key, U8 *w)
{

    U8 tmp[4];
    U16 i;
    U16 j;
    U16 len = Nb*(Nr+1);
    for (i = 0; i < Nk; i++)
    {
        w[4*i+0] = key[4*i+0];
        w[4*i+1] = key[4*i+1];
        w[4*i+2] = key[4*i+2];
        w[4*i+3] = key[4*i+3];
    }

    for (i = Nk; i < len; i++)
    {
        tmp[0] = w[4*(i-1)+0];
        tmp[1] = w[4*(i-1)+1];
        tmp[2] = w[4*(i-1)+2];
        tmp[3] = w[4*(i-1)+3];

        if (i%Nk == 0)
        {
            rot_word(tmp);
            sub_word(tmp);
            coef_add(tmp, Rcon(i/Nk), tmp);

        }
        w[4*i+0] = w[4*(i-Nk)+0]^tmp[0];
        w[4*i+1] = w[4*(i-Nk)+1]^tmp[1];
        w[4*i+2] = w[4*(i-Nk)+2]^tmp[2];
        w[4*i+3] = w[4*(i-Nk)+3]^tmp[3];
    }
}

void addroundkey(U8 *data,U8 r,U8 *w)
{

    U8 c;
    for (c = 0; c < Nb; c++)
    {
        data[Nb*0+c] = data[Nb*0+c]^w[4*Nb*r+4*c+0];	 //debug, so it works for Nb !=4
        data[Nb*1+c] = data[Nb*1+c]^w[4*Nb*r+4*c+1];
        data[Nb*2+c] = data[Nb*2+c]^w[4*Nb*r+4*c+2];
        data[Nb*3+c] = data[Nb*3+c]^w[4*Nb*r+4*c+3];
    }
}

//����


void subbytes(U8 * data)
{
    U8 i;
    U8 j;
    U8 a;
    U8 b;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < Nb; j++)
        {
            a = (data[Nb*i+j] & 0xf0) >> 4;
            b = data[Nb*i+j] & 0x0f;
            data[Nb*i+j] = s_box[16*a+b];
        }
    }
}
void shiftrows(U8 * data)
{

    U8 i;
    U8 k;
    U8 s;
    U8 tmp;
    for(i=0; i<4; i++)
    {
        s = 0;
        while (s < i)
        {
            tmp = data[Nb*i+0];
            for (k = 1; k < Nb; k++)
            {
                data[Nb*i+k-1] = data[Nb*i+k];
            }
            data[Nb*i+Nb-1] = tmp;
            s++;
        }

    }
}
void coef_mult(U8 *a, U8 *b, U8 *d)
{

    d[0] = gmult(a[0],b[0])^gmult(a[3],b[1])^gmult(a[2],b[2])^gmult(a[1],b[3]);
    d[1] = gmult(a[1],b[0])^gmult(a[0],b[1])^gmult(a[3],b[2])^gmult(a[2],b[3]);
    d[2] = gmult(a[2],b[0])^gmult(a[1],b[1])^gmult(a[0],b[2])^gmult(a[3],b[3]);
    d[3] = gmult(a[3],b[0])^gmult(a[2],b[1])^gmult(a[1],b[2])^gmult(a[0],b[3]);
}

void mixcolumns(U8 *data)
{

    U8 a[] = {0x02, 0x01, 0x01, 0x03}; // a(x) = {02} + {01}x + {01}x2 + {03}x3
    U8 i;
    U8 j;
    U8 col[4];
    U8 res[4];
    for (j = 0; j < Nb; j++)
    {
        for (i = 0; i < 4; i++)
        {
            col[i] = data[Nb*i+j];
        }
        coef_mult(a, col, res);
        for (i = 0; i < 4; i++)
        {
            data[Nb*i+j] = res[i];
        }
    }
}

void aes128_encode(U8 *in,U8 * w)
{
    U16 i;
    U8 d1[200];
    U16 j;
    memset(d1,0,sizeof(d1));

    for(i=0; i<4; i++)
    {
        for(j=0; j<Nb; j++)
        {
            d1[Nb*i+j]=in[i+4*j];//�����û�
        }
    }
    addroundkey(d1,0,w);
    for(i=0; i<Nr-1; i++)
    {
        subbytes(d1);
        shiftrows(d1);
        mixcolumns(d1);
        addroundkey(d1,i+1,w);
    }
    subbytes(d1);
    shiftrows(d1);
    addroundkey(d1,Nr,w);
    for(i=0; i<4; i++)
    {
        for(j=0; j<Nb; j++)
        {
            in[i+4*j]=d1[i*Nb+j];//�����û�
        }
    }

}
void aes128_encodece(U8 *in,U8 * w)
{
    U16 i;
    U8 d1[200];
    U16 j;
    memset(d1,0,sizeof(d1));

    for(i=0; i<4; i++)
    {
        for(j=0; j<Nb; j++)
        {
            d1[Nb*i+j]=in[i+4*j];//�����û�
        }
    }
    //addroundkey(d1,0,w);
    for(i=0; i<Nr-1; i++)
    {
        subbytes(d1);
        shiftrows(d1);
        mixcolumns(d1);
        //addroundkey(d1,i+1,w);
    }
    subbytes(d1);
    shiftrows(d1);
    //addroundkey(d1,Nr,w);
    for(i=0; i<4; i++)
    {
        for(j=0; j<Nb; j++)
        {
            in[i+4*j]=d1[i*Nb+j];//�����û�
        }
    }

}

void bird_set_aes128_encode(U8 *data,U16 l)
{
    U8 w[600];
    U16 i;
    U16 j;
    U8 in[100];
    // 128 bit key
    U8 key[] =
    {
        0xd6, 0x23, 0x43, 0xaa,
        0x13, 0x1d, 0xf5, 0x54,
        0x5e, 0x6f, 0x4b, 0x98,
        0x47, 0x2e, 0x56, 0xeb
    }; //����
    U8 iv[] =
    {
        0x52,0x16,0x32,0x2c,
        0xf0,0x7c,0xd8,0x8e,
        0xa9,0xd8,0x25,0x17,
        0xc8,0x17,0xdf,0x4b
    }; //����
    memset(w,0,sizeof(w));
    kal_prompt_trace(  MOD_SOC,"start encode ");
    key_expansion(key, w);
    for(i=0; i<l; i++)
    {
        for(j=0; j<16; j++)
        {
            in[j]=data[i*16+j];
            in[j]=in[j]^iv[j];
        }
        aes128_encode(in,w);
        for(j=0; j<16; j++)
        {
            data[i*16+j]=in[j];
            iv[j]=in[j];
        }
    }
    kal_prompt_trace(  MOD_SOC,"end encode ");
}

//����
void inv_shiftrows(U8 * data)
{
    U8 i;
    U8 k;
    U8 s;
    U8 tmp;
    for(i=0; i<4; i++)
    {
        s = 0;
        while (s < i)
        {
            tmp = data[Nb*i+Nb-1];
            for (k = Nb-1; k > 0; k--)
            {
                data[Nb*i+k] = data[Nb*i+k-1];
            }
            data[Nb*i+0] = tmp;
            s++;
        }
    }
}

void inv_subbytes(U8 * data)
{
    U8 i;
    U8 j;
    U8 a;
    U8 b;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < Nb; j++)
        {
            a = (data[Nb*i+j] & 0xf0) >> 4;
            b = data[Nb*i+j] & 0x0f;
            data[Nb*i+j] = inv_s_box[16*a+b];
        }
    }
}

void inv_mixcolumns(U8 * data)
{
    U8 a[] = {0x0E, 0x09, 0x0D, 0x0B};
    U8 i;
    U8 j;
    U8 col[4];
    U8 res[4];

    for (j = 0; j < Nb; j++)
    {
        for (i = 0; i < 4; i++)
        {
            col[i] = data[Nb*i+j];
        }
        coef_mult(a, col, res);
        for (i = 0; i < 4; i++)
        {
            data[Nb*i+j] = res[i];
        }
    }
}

void aes128_inv_encode(U8 *in,U8 * w)
{
    U8 i;
    U8 d1[200];
    U16 j;
    memset(d1,0,sizeof(d1));

    for(i=0; i<4; i++)
    {
        for(j=0; j<Nb; j++)
        {
            d1[Nb*i+j]=in[i+4*j];
        }
    }
    addroundkey(d1,Nr,w);
    for(i=Nr-1; i>0; i--)
    {
        inv_shiftrows(d1);
        inv_subbytes(d1);
        addroundkey(d1,i,w);
        inv_mixcolumns(d1);

    }
    inv_shiftrows(d1);
    inv_subbytes(d1);
    addroundkey(d1,0,w);
    for(i=0; i<4; i++)
    {
        for(j=0; j<Nb; j++)
        {
            in[i+4*j]=d1[i*Nb+j];
        }
    }

}
void bird_set_inv_aes128_encode(U8 *data,U16 l)
{
    U8 w[600];
    U8 i;
    U8 j;
    U8 in[100];
    // 128 bit key
    U8 key[] =
    {
        0xd6, 0x23, 0x43, 0xaa,
        0x13, 0x1d, 0xf5, 0x54,
        0x5e, 0x6f, 0x4b, 0x98,
        0x47, 0x2e, 0x56, 0xeb
    };
    U8 iv[] =
    {
        0x52,0x16,0x32,0x2c,
        0xf0,0x7c,0xd8,0x8e,
        0xa9,0xd8,0x25,0x17,
        0xc8,0x17,0xdf,0x4b
    };
    memset(w,0,sizeof(w));
    kal_prompt_trace(  MOD_SOC,"start inv_encode ");
    key_expansion(key, w);
    for(i=0; i<l; i++)
    {
        for(j=0; j<16; j++)
        {
            in[j]=data[i*16+j];
        }
        aes128_inv_encode(in,w);
        for(j=0; j<16; j++)
        {
            in[j]=in[j]^iv[j];
            iv[j]= data[i*16+j];
            data[i*16+j]=in[j];

        }
    }
    kal_prompt_trace(  MOD_SOC,"end inv_encode ");
}

