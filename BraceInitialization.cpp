/*---------------------------------------------------------------------------*/
/** \file
 * Header file for BraceInitialization.
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

TEST(BraceInitialization, SupportsNumericTypes) {
   int x{42};
   ASSERT_THAT(x, ::testing::Eq(42));

   double y{12.2};
   ASSERT_THAT(y, ::testing::DoubleEq(12.2));
}

TEST(BraceInitialization, SupportsStrings) {
   std::string s{"Jeff"};
   ASSERT_THAT(s, ::testing::Eq("Jeff"));
}

TEST(BraceInitialization, SupportsCollectionTypes) {
   std::vector<std::string> names {"alpha", "beta", "gamma" };
   ASSERT_THAT(names, ::testing::ElementsAre("alpha", "beta", "gamma"));
}

TEST(BraceInitialization, SupportsArrays) {
   int xs[] {1, 1, 2, 3, 5, 8};
   ASSERT_THAT(xs, ::testing::ElementsAre(1, 1, 2, 3, 5, 8));
}


