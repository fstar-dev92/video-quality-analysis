#ifndef IPP_H
#define IPP_H

#include <stdint.h>

// Intel IPP type definitions
typedef float Ipp32f;   // 32-bit floating point
typedef double Ipp64f;  // 64-bit floating point

// Image size structure
typedef struct {
    int width;
    int height;
} IppiSize;

// IPP function declarations
// Note: These are placeholder declarations for the functions used in the codebase
// In a real implementation, these would be properly declared from the IPP headers

// L2 norm calculation for 8-bit unsigned single channel images
int ippiNorm_L2_8u_C1R(
    const unsigned char* pSrc,
    int srcStep,
    IppiSize roiSize,
    Ipp64f* pNorm
);

// L2 norm difference calculation for 8-bit unsigned single channel images
int ippiNormDiff_L2_8u_C1R(
    const unsigned char* pSrc1,
    int src1Step,
    const unsigned char* pSrc2,
    int src2Step,
    IppiSize roiSize,
    Ipp64f* pNormDiff
);

// Enhanced L2 norm difference calculation for 8-bit unsigned single channel images
// with additional features like error checking and optimized computation
int ippiNormDiff_L2_8u_C1R_Enhanced(
    const unsigned char* pSrc1,
    int src1Step,
    const unsigned char* pSrc2,
    int src2Step,
    IppiSize roiSize,
    Ipp64f* pNormDiff,
    int* pErrorCode
);

// Optimized L2 norm difference calculation with vectorized computation
int ippiNormDiff_L2_8u_C1R_Optimized(
    const unsigned char* pSrc1,
    int src1Step,
    const unsigned char* pSrc2,
    int src2Step,
    IppiSize roiSize,
    Ipp64f* pNormDiff,
    int* pErrorCode
);

#endif // IPP_H
