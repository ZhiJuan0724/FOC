/**
 * 1.不要问为什么
 * 2.问就是为了提高单片机运算速率
 * 3.不可随意更改
 */

#ifndef __MATRIXFUNCTION_H
#define __MATRIXFUNCTION_H

#include "stdint.h"
#include "stdbool.h"

// 矩阵实例化结构体
typedef struct
{
    uint16_t numRows;
    uint16_t numCols;
    float *pData;
} matrix_instance_t;

// 矩阵初始化
void mat_init(matrix_instance_t *pMat,
              uint16_t nRows,
              uint16_t nColumns,
              float *pData);
// 矩阵加法
void mat_add(const matrix_instance_t *pMatA,
             const matrix_instance_t *pMatB,
             matrix_instance_t *pDst);
// 矩阵减法
void mat_sub(const matrix_instance_t *pMatA,
             const matrix_instance_t *pMatB,
             matrix_instance_t *pDst);
// 矩阵乘法
void mat_mult(const matrix_instance_t *pMatA,
              const matrix_instance_t *pMatB,
              matrix_instance_t *pDst);
// 矩阵缩放
void mat_scale(const matrix_instance_t *pMat,
               float scale,
               matrix_instance_t *pDst);

#endif
