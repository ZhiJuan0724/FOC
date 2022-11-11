#include "MatrixFunctions.h"

void mat_init(matrix_instance_t *pMat, uint16_t nRows, uint16_t nColumns, float *pData)
{
    pMat->numRows = nRows;
    pMat->numCols = nColumns;
    pMat->pData = pData;
}

void mat_add(const matrix_instance_t *pMatA, const matrix_instance_t *pMatB, matrix_instance_t *pDst)
{
    float *pInA = pMatA->pData;
    float *pInB = pMatB->pData;
    float *pOut = pDst->pData;

    uint32_t numSamples; // 元素个数
    uint32_t blkCnt;     // 循环次数

    numSamples = (uint32_t)pMatA->numRows * pMatA->numCols;

    //----------------------快速赋值?----------------------
    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = numSamples >> 2U;

    while (blkCnt > 0U)
    {
        /* C(m,n) = A(m,n) + B(m,n) */

        /* Add and store result in destination buffer. */
        *pOut++ = (*pInA++) + (*pInB++);

        *pOut++ = (*pInA++) + (*pInB++);

        *pOut++ = (*pInA++) + (*pInB++);

        *pOut++ = (*pInA++) + (*pInB++);

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = numSamples % 0x4U;
    //--------------------------------------------------

    while (blkCnt > 0U)
    {
        *pOut++ = (*pInA++) + (*pInB++);
        blkCnt--;
    }
}

void mat_sub(const matrix_instance_t *pMatA, const matrix_instance_t *pMatB, matrix_instance_t *pDst)
{
    float *pInA = pMatA->pData;
    float *pInB = pMatB->pData;
    float *pOut = pDst->pData;

    uint32_t numSamples;
    uint32_t blkCnt;

    numSamples = (uint32_t)pMatA->numRows * pMatA->numCols;

    //----------------------快速赋值?----------------------
    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = numSamples >> 2U;

    while (blkCnt > 0U)
    {
        /* C(m,n) = A(m,n) + B(m,n) */

        /* Add and store result in destination buffer. */
        *pOut++ = (*pInA++) - (*pInB++);

        *pOut++ = (*pInA++) - (*pInB++);

        *pOut++ = (*pInA++) - (*pInB++);

        *pOut++ = (*pInA++) - (*pInB++);

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = numSamples % 0x4U;
    //--------------------------------------------------

    while (blkCnt > 0U)
    {
        *pOut++ = (*pInA++) + (*pInB++);
        blkCnt--;
    }
}

void mat_mult(const matrix_instance_t *pMatA, const matrix_instance_t *pMatB, matrix_instance_t *pDst)
{
    float *pIn1 = pMatA->pData;
    float *pIn2 = pMatB->pData;
    float *pInA = pMatA->pData;
    float *pInB = pMatB->pData;
    float *pOut = pDst->pData;
    float *px; // 临时输出数据矩阵指针
    float sum; // 和
    uint16_t numRowsA = pMatA->numRows;
    uint16_t numColsB = pMatB->numCols;
    uint16_t numColsA = pMatA->numCols;
    uint32_t col, i = 0U, row = numRowsA, colCnt; // 循环计数

    /* The following loop performs the dot-product of each row in pMatA with each column in pMatB */
    /* row loop */
    do
    {
        /* Output pointer is set to starting address of row being processed */
        px = pOut + i;

        /* For every row wise process, column loop counter is to be initiated */
        col = numColsB;

        /* For every row wise process, pIn2 pointer is set to starting address of pMatB data */
        pIn2 = pMatB->pData;

        /* column loop */
        do
        {
            /* Set the variable sum, that acts as accumulator, to zero */
            sum = 0.0f;

            /* Initialize pointer pIn1 to point to starting address of column being processed */
            pIn1 = pInA;

            //------------------快速赋值?----------------------
            /* Loop unrolling: Compute 4 MACs at a time. */
            colCnt = numColsA >> 2U;

            /* matrix multiplication */
            while (colCnt > 0U)
            {
                /* c(m,p) = a(m,1) * b(1,p) + a(m,2) * b(2,p) + .... + a(m,n) * b(n,p) */

                /* Perform the multiply-accumulates */
                sum += *pIn1++ * *pIn2;
                pIn2 += numColsB;

                sum += *pIn1++ * *pIn2;
                pIn2 += numColsB;

                sum += *pIn1++ * *pIn2;
                pIn2 += numColsB;

                sum += *pIn1++ * *pIn2;
                pIn2 += numColsB;

                /* Decrement loop counter */
                colCnt--;
            }

            /* Loop unrolling: Compute remaining MACs */
            colCnt = numColsA % 0x4U;

            //---------------------------------------------------

            while (colCnt > 0U)
            {
                /* c(m,p) = a(m,1) * b(1,p) + a(m,2) * b(2,p) + .... + a(m,n) * b(n,p) */

                /* Perform the multiply-accumulates */
                sum += *pIn1++ * *pIn2;
                pIn2 += numColsB;

                /* Decrement loop counter */
                colCnt--;
            }

            /* Store result in destination buffer */
            *px++ = sum;

            /* Decrement column loop counter */
            col--;

            /* Update pointer pIn2 to point to starting address of next column */
            pIn2 = pInB + (numColsB - col);

        } while (col > 0U);

        /* Update pointer pInA to point to starting address of next row */
        i = i + numColsB;
        pInA = pInA + numColsA;

        /* Decrement row loop counter */
        row--;

    } while (row > 0U);
}

void mat_scale(const matrix_instance_t *pMat, float scale, matrix_instance_t *pDst)
{
    float *pIn = pMat->pData;
    float *pOut = pDst->pData;
    uint32_t numSamples;
    uint32_t blkCnt;

    numSamples = (uint32_t)pMat->numRows * pMat->numCols;

    //------------------快速赋值?----------------------
    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = numSamples >> 2U;

    while (blkCnt > 0U)
    {
        /* C(m,n) = A(m,n) * scale */

        /* Scale and store result in destination buffer. */
        *pOut++ = (*pIn++) * scale;
        *pOut++ = (*pIn++) * scale;
        *pOut++ = (*pIn++) * scale;
        *pOut++ = (*pIn++) * scale;

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = numSamples % 0x4U;
    //---------------------------------------------------

    while (blkCnt > 0U)
    {
        /* C(m,n) = A(m,n) * scale */

        /* Scale and store result in destination buffer. */
        *pOut++ = (*pIn++) * scale;

        /* Decrement loop counter */
        blkCnt--;
    }
}
