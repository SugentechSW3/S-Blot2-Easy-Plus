/**
@file KISA_SHA_256.h
@brief SHA256 ��ȣ �˰�����
@author Copyright (c) 2013 by KISA
@remarks http://seed.kisa.or.kr/
*/

#ifndef SHA256_H
#define SHA256_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#undef BIG_ENDIAN
#undef LITTLE_ENDIAN

#define LITTLE_ENDIAN


    typedef unsigned long ULONG;
    typedef ULONG* ULONG_PTR;

    typedef unsigned int UINT;
    typedef UINT* UINT_PTR;

    typedef signed int SINT;
    typedef SINT* SINT_PTR;

    typedef unsigned char UCHAR;
    typedef UCHAR* UCHAR_PTR;

    typedef unsigned char BYTE;

#define SHA256_DIGEST_BLOCKLEN	64
#define SHA256_DIGEST_VALUELEN	32

    typedef struct {
        UINT uChainVar[SHA256_DIGEST_VALUELEN / 4];
        UINT uHighLength;
        UINT uLowLength;
        UINT remain_num;
        BYTE szBuffer[SHA256_DIGEST_BLOCKLEN];
    } SHA256_INFO;


    void SHA256_Init(OUT SHA256_INFO *Info);

    void SHA256_Process(OUT SHA256_INFO *Info, IN const BYTE *pszMessage, IN UINT uDataLen);

    void SHA256_Close(OUT SHA256_INFO *Info, OUT BYTE *pszDigest);

    void SHA256_Encrpyt(IN const BYTE *pszMessage, IN UINT uPlainTextLen, OUT BYTE *pszDigest);

#ifdef  __cplusplus
}
#endif

#endif
