/*---------------------------------------------------------------------------*/
/** \file
 * Header file for InOutManipulator.
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

#include <ios>
#include <cstdlib>

TEST(InOutManipulator, hexToOct) {
    std::string strHex("C8");
    std::stringstream ssHex(strHex);
    ASSERT_THAT(ssHex.str(), ::testing::StrEq(strHex));

    unsigned long ulDec;
    ssHex >> std::hex >> ulDec;

    ASSERT_THAT(ulDec, ::testing::Eq(200ul));

    std::stringstream ssDec;
    ssDec << ulDec;

    ASSERT_THAT(ssDec.str(), ::testing::StrEq("200"));

    std::stringstream ssOct;
    ssOct << std::oct << ulDec;
    ASSERT_THAT(ssOct.str(), ::testing::StrEq("310"));

    std::stringstream ss2Oct(ssOct.str());

    unsigned long ulOct;
    ss2Oct >> ulOct;

    ASSERT_THAT(ulOct, ::testing::Eq(310ul));
}


