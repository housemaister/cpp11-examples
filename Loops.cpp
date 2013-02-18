/*---------------------------------------------------------------------------*/
/** \file
 * Source file for Loops.
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

/* Loops can be done with:
 *  - loop with indices
 *  - iterators (use auto for not writing blabla::iterator)
 *  - for_each (see Lambda.cpp) and extended example below
 *  (- Boost library with lambda)
 *  (- Boost macros)
 *  - Range based loops
 */


TEST(Loops, basicRangeBasedLoop) {
    std::vector<int> vec;
    vec.push_back( 1 );
    vec.push_back( 2 );
    for(auto val : vec ) {
        ASSERT_THAT(val, ::testing::Gt(0));
    }
}

/* extended example */

class CItem {
public:
   CItem(int cost) : cost_{cost} {}
   int Cost() { return cost_; }
private:
   int cost_;
};


int TotalCost(std::vector<CItem>&& items) {
   int total{0};
   for (auto item: items) {
       total += item.Cost();
   }
   return total;
}

TEST(Loops, TotalCost) {
    ASSERT_THAT(
            TotalCost({CItem(5), CItem(10), CItem(15)}),
            ::testing::Eq(5 + 10 + 15));
}

int LambdaTotalCost(std::vector<CItem>&& items) {
   return std::accumulate(items.begin(), items.end(), 0,
     [] (int total, CItem item) { return total + item.Cost(); });
}

TEST(Loops, LambdaTotalCost) {
    ASSERT_THAT(
            LambdaTotalCost({CItem(55), CItem(510), CItem(515)}),
            ::testing::Eq(55 + 510 + 515));
}


/* Range based loop */

TEST(Loops, RangeBased) {
    int total = 0;
    std::vector<int> vec;
    vec.push_back( 1 );
    vec.push_back( 2 );
    for(auto val : vec) {
        total += val;
    }

    ASSERT_THAT(total, ::testing::Eq(3));
}

