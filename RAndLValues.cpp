/*---------------------------------------------------------------------------*/
/** \file
 * Header file for RAndLValues.
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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

vector<int> doubleValues (const vector<int>& v)
{
    vector<int> new_values( v.size() );
    for (auto itr = new_values.begin(), end_itr = new_values.end(); itr != end_itr; ++itr )
    {
        new_values.push_back( 2 * *itr );
    }
    return new_values; // copy (may be optimized by compiler)
}

/* where are the copies? */
TEST(ManyCopies, copyValues) {
    vector<int> v;
    for ( int i = 0; i < 100; i++ )
    {
        v.push_back( i );
    }
    v = doubleValues( v ); // copy
}

