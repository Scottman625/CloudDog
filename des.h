/*********************************************************
 *  des.h
 *  用户使用des算法头文件
 *	
 *********************************************************/
#ifndef _OPENDESS_H_
#define _OPENDESS_H_

#ifdef __cplusplus
extern "C" {
#endif


//加密小于4k的數據（encrypt：加密）
//pInData：明文數據
//nInDataLen：明文數據長度
//pOutData：加密后的數據
//pOutDataLen：加密數據的長度
int DesEnc(
		unsigned char *pInData,
		int            nInDataLen,
		unsigned char *pOutData,
		int           *pOutDataLen);

//加密等于4k的數據
int DesEnc_raw(
	unsigned char *pInData,
	int            nInDataLen,
	unsigned char *pOutData,
	int           *pOutDataLen);

//解密小于4k的數據(decrypt：解密)
//pInData：密文數據
//nInDataLen：密文數據長度
//pOutData：解密后的數據
//pOutDataLen：解密數據的長度
int DesDec(
	   unsigned char *pInData,
	   int            nInDataLen,
	   unsigned char *pOutData,
	   int           *pOutDataLen);

//解密等于4k的數據
int DesDec_raw(
	unsigned char *pInData,
	int            nInDataLen,
	unsigned char *pOutData,
	int           *pOutDataLen);



#ifdef __cplusplus
}
#endif

#endif

