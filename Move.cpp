/*---------------------------------------------------------------------------*/
/** \file
 * Source file for Move.
 *
 * \author weigand
 *
 * (C) Copyright 2013 by TriaGnoSys GmbH, 82234 Wessling, Germany
 *
 * \verbatim
 * $Id:$
 * \endverbatim
 */
/*---------------------------------------------------------------------------*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>

/* use this class also to play with default and delete */

class CMatrix {
public:
    CMatrix(const CMatrix & matrix); // copy constructor
//    CMatrix(CMatrix && matrix); // move constructor
//    CMatrix& operator=(const CMatrix & matrix) = default; // copy assignment
//    CMatrix& operator=(CMatrix && matrix); // move assignment (just to show)

    CMatrix(std::size_t rows, std::size_t columns);
    CMatrix & operator* (CMatrix const & other) const; // used for demonstrating copy
//    CMatrix operator* (CMatrix const & other) const; // used for move

    std::size_t rows() const;
    std::size_t columns() const;
    std::vector<double> elements() const;

private:
    std::size_t m_columns;
    std::size_t m_rows;
    std::vector<double> m_elements;
};

/* needed stuff */
CMatrix::CMatrix(const CMatrix & matrix) :
    m_columns(matrix.columns()),
    m_rows(matrix.rows()),
    m_elements(matrix.elements()) {}

CMatrix::CMatrix(std::size_t rows, std::size_t columns) :
        m_columns(columns),
        m_rows(rows),
        m_elements(std::vector<double>(rows*columns)) {}

std::size_t CMatrix::rows() const {
    return m_rows;
}

std::size_t CMatrix::columns() const {
    return m_columns;
}

std::vector<double> CMatrix::elements() const {
    return m_elements;
}



/* Motivation: swap */

void swapMatrices(CMatrix& matrixA, CMatrix& matrixB) {
    CMatrix matrixTmp(matrixA); // two copies A
    matrixA = matrixB; // two copies B
    matrixB = matrixTmp; // two copies Tmp
}

/* Motivation: multiply (possible mem leak (new without delete) */
CMatrix & CMatrix::operator* (CMatrix const & other) const
{
    // snip: ... assert matrix sizes are compatible ...
    CMatrix * result = new CMatrix(rows(), other.columns());
    // snip: ... compute and store matrix elements ...
    return *result;
}

TEST(Move, matrixMemoryLeak) {
    CMatrix matrixA(2,5);
    CMatrix matrixB(2,5);

    CMatrix & matrixC = matrixA * matrixB;

    delete (&matrixC); // this fixes mem leak

    // no fix for chained calls like A * (B * C)
}

/* with move constructor */

//#include <utility>
//
//CMatrix CMatrix::operator* (CMatrix const & other) const
//{
//    // snip: ... assert matrix sizes are compatible ...
//    CMatrix result(rows(), other.columns());
//    // snip: ... compute and store matrix elements ...
//    return std::move(result);
//}
//
//CMatrix::CMatrix(CMatrix && matrix) :
//    m_columns(matrix.columns()),
//    m_rows(matrix.rows()),
//    m_elements(std::move(matrix.elements())) {}
//
//TEST(Move, withMove) {
//    CMatrix matrixA(2,5);
//    CMatrix matrixB(2,5);
//
//    CMatrix matrixC = matrixA * matrixB;
//}
