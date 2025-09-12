/** @file ipp_enhanced.c
 *  @brief Enhanced IPP function implementations
 *
 *  This file contains enhanced implementations of Intel IPP functions
 *  with additional features like error checking and optimized computation.
 */

#include <ipp.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

// Error codes for enhanced functions
#define IPP_SUCCESS 0
#define IPP_NULL_POINTER_ERROR -1
#define IPP_INVALID_SIZE_ERROR -2
#define IPP_INVALID_STEP_ERROR -3
#define IPP_MEMORY_ERROR -4

/**
 * @brief Enhanced L2 norm difference calculation for 8-bit unsigned single channel images
 * 
 * This function calculates the L2 norm difference between two 8-bit unsigned single-channel images
 * with enhanced error checking and optimized computation.
 * 
 * @param pSrc1 Pointer to the first source image
 * @param src1Step Step size for the first source image (bytes per row)
 * @param pSrc2 Pointer to the second source image
 * @param src2Step Step size for the second source image (bytes per row)
 * @param roiSize Size of the region of interest
 * @param pNormDiff Pointer to store the calculated L2 norm difference
 * @param pErrorCode Pointer to store error code (can be NULL)
 * @return 0 on success, non-zero on error
 */
int ippiNormDiff_L2_8u_C1R_Enhanced(
    const unsigned char* pSrc1,
    int src1Step,
    const unsigned char* pSrc2,
    int src2Step,
    IppiSize roiSize,
    Ipp64f* pNormDiff,
    int* pErrorCode
)
{
    // Input validation
    if (pSrc1 == NULL || pSrc2 == NULL || pNormDiff == NULL) {
        if (pErrorCode) *pErrorCode = IPP_NULL_POINTER_ERROR;
        return IPP_NULL_POINTER_ERROR;
    }
    
    if (roiSize.width <= 0 || roiSize.height <= 0) {
        if (pErrorCode) *pErrorCode = IPP_INVALID_SIZE_ERROR;
        return IPP_INVALID_SIZE_ERROR;
    }
    
    if (src1Step < roiSize.width || src2Step < roiSize.width) {
        if (pErrorCode) *pErrorCode = IPP_INVALID_STEP_ERROR;
        return IPP_INVALID_STEP_ERROR;
    }
    
    // Initialize error code to success
    if (pErrorCode) *pErrorCode = IPP_SUCCESS;
    
    // Calculate L2 norm difference
    Ipp64f sum_squared_diff = 0.0;
    const unsigned char* p1 = pSrc1;
    const unsigned char* p2 = pSrc2;
    
    for (int y = 0; y < roiSize.height; y++) {
        for (int x = 0; x < roiSize.width; x++) {
            int diff = (int)p1[x] - (int)p2[x];
            sum_squared_diff += (Ipp64f)(diff * diff);
        }
        p1 += src1Step;
        p2 += src2Step;
    }
    
    // Calculate the square root of the sum of squared differences
    *pNormDiff = sqrt(sum_squared_diff);
    
    return IPP_SUCCESS;
}

/**
 * @brief Alternative implementation with vectorized computation
 * 
 * This is an alternative implementation that could be optimized with SIMD instructions
 * for better performance on modern processors.
 */
int ippiNormDiff_L2_8u_C1R_Optimized(
    const unsigned char* pSrc1,
    int src1Step,
    const unsigned char* pSrc2,
    int src2Step,
    IppiSize roiSize,
    Ipp64f* pNormDiff,
    int* pErrorCode
)
{
    // Input validation (same as enhanced version)
    if (pSrc1 == NULL || pSrc2 == NULL || pNormDiff == NULL) {
        if (pErrorCode) *pErrorCode = IPP_NULL_POINTER_ERROR;
        return IPP_NULL_POINTER_ERROR;
    }
    
    if (roiSize.width <= 0 || roiSize.height <= 0) {
        if (pErrorCode) *pErrorCode = IPP_INVALID_SIZE_ERROR;
        return IPP_INVALID_SIZE_ERROR;
    }
    
    if (src1Step < roiSize.width || src2Step < roiSize.width) {
        if (pErrorCode) *pErrorCode = IPP_INVALID_STEP_ERROR;
        return IPP_INVALID_STEP_ERROR;
    }
    
    if (pErrorCode) *pErrorCode = IPP_SUCCESS;
    
    // Optimized computation with reduced memory access
    Ipp64f sum_squared_diff = 0.0;
    const int width = roiSize.width;
    const int height = roiSize.height;
    
    for (int y = 0; y < height; y++) {
        const unsigned char* row1 = pSrc1 + y * src1Step;
        const unsigned char* row2 = pSrc2 + y * src2Step;
        
        // Process multiple pixels at once for better cache performance
        int x = 0;
        for (; x < width - 3; x += 4) {
            int diff0 = (int)row1[x] - (int)row2[x];
            int diff1 = (int)row1[x+1] - (int)row2[x+1];
            int diff2 = (int)row1[x+2] - (int)row2[x+2];
            int diff3 = (int)row1[x+3] - (int)row2[x+3];
            
            sum_squared_diff += (Ipp64f)(diff0 * diff0 + diff1 * diff1 + 
                                        diff2 * diff2 + diff3 * diff3);
        }
        
        // Handle remaining pixels
        for (; x < width; x++) {
            int diff = (int)row1[x] - (int)row2[x];
            sum_squared_diff += (Ipp64f)(diff * diff);
        }
    }
    
    *pNormDiff = sqrt(sum_squared_diff);
    return IPP_SUCCESS;
}

/**
 * @brief L2 norm calculation for 8-bit unsigned single channel images
 * 
 * This function calculates the L2 norm of an 8-bit unsigned single-channel image.
 * 
 * @param pSrc Pointer to the source image
 * @param srcStep Step size for the source image (bytes per row)
 * @param roiSize Size of the region of interest
 * @param pNorm Pointer to store the calculated L2 norm
 * @return 0 on success, non-zero on error
 */
int ippiNorm_L2_8u_C1R(
    const unsigned char* pSrc,
    int srcStep,
    IppiSize roiSize,
    Ipp64f* pNorm
)
{
    // Input validation
    if (pSrc == NULL || pNorm == NULL) {
        return IPP_NULL_POINTER_ERROR;
    }
    
    if (roiSize.width <= 0 || roiSize.height <= 0) {
        return IPP_INVALID_SIZE_ERROR;
    }
    
    if (srcStep < roiSize.width) {
        return IPP_INVALID_STEP_ERROR;
    }
    
    // Calculate L2 norm
    Ipp64f sum_squared = 0.0;
    const unsigned char* p = pSrc;
    
    for (int y = 0; y < roiSize.height; y++) {
        for (int x = 0; x < roiSize.width; x++) {
            Ipp64f pixel_value = (Ipp64f)p[x];
            sum_squared += pixel_value * pixel_value;
        }
        p += srcStep;
    }
    
    // Calculate the square root of the sum of squared values
    *pNorm = sqrt(sum_squared);
    
    return IPP_SUCCESS;
}
