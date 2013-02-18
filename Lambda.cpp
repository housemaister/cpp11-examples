/*---------------------------------------------------------------------------*/
/** \file
 * Source file for Lambda.
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

#include <iostream>

/* Basics */
TEST(Lambda, Basics){
    auto func = [] () { std::cout << "Hello world"; };
    func(); // now call the function
}

/* extended example */
#include <string>
#include <vector>

class CAddressBook {
public:
    // using a template allows us to ignore the differences between functors, function pointers
    // and lambda
    template<typename Func>
    std::vector<std::string> findMatchingAddresses (Func func) const {
        std::vector<std::string> vec_results;
        for ( auto itr = m_strAddresses.begin(), end = m_strAddresses.end(); itr != end; ++itr ) {
            // call the function passed into findMatchingAddresses and see if it matches
            if ( func( *itr ) ) {
                vec_results.push_back( *itr );
            }
        }
        return vec_results;
    }

    void addAddress(std::string strAddress) {
        m_strAddresses.emplace_back(strAddress);
    }

private:
    std::vector<std::string> m_strAddresses;
};

std::vector<std::string> findAddressesFromOrgs(CAddressBook const & addressBook) {
    return addressBook.findMatchingAddresses(
            // we're declaring a lambda here; the [] signals the start
            [] (const std::string& addr) {
        return addr.find( ".org" ) != std::string::npos;
    } ); // <-- remember this syntax
}

TEST(Lambda, AddressbookOrgSearch) {
    CAddressBook addressBook;
    addressBook.addAddress("stefan.weigand@triagnosys.com");
    addressBook.addAddress("somebody@some.org");
    std::vector<std::string> vec_addresses = findAddressesFromOrgs(addressBook);
    ASSERT_THAT(vec_addresses.size(), ::testing::Eq(1UL));
    ASSERT_THAT(vec_addresses[0], ::testing::Eq("somebody@some.org"));
}


/* variable capture */

std::vector<std::string> findAddressesWithName(CAddressBook const & addressBook, std::string strName) {
    return addressBook.findMatchingAddresses(
            // notice that the lambda function uses the the variable 'strName'
            // [&] : Capture any referenced variable by reference
            [&] (const std::string& addr) { return addr.find( strName ) != std::string::npos; }
    );
}

TEST(Lambda, AddressbookNameSearch) {
    CAddressBook addressBook;
    addressBook.addAddress("stefan.weigand@triagnosys.com");
    addressBook.addAddress("somebody@some.org");
    std::vector<std::string> vec_addresses = findAddressesWithName(addressBook, "weigand");
    ASSERT_THAT(vec_addresses.size(), ::testing::Eq(1UL));
    ASSERT_THAT(vec_addresses[0], ::testing::Eq("stefan.weigand@triagnosys.com"));
}

/* With for_each loops */

void normal_loop() {
    std::vector<int> vec;
    vec.push_back( 1 );
    vec.push_back( 2 );
    //...
    for ( auto itr = vec.begin(), end = vec.end(); itr != end; itr++ )
    {
        std::cout << *itr;
    }
}

TEST(Lambda, for_each_loop) {
    std::vector<int> vec;
    vec.push_back( 1 );
    vec.push_back( 2 );
    std::for_each(vec.begin(), vec.end(), [] (int val) {
        ASSERT_THAT(val, ::testing::Gt(0));
    } );
}

/* return values:
   * compiler automatically determines return value (cf auto)
   * specify with new syntax; e.g.:
     [] () -> int { return 1; }

   throw:
     [] () throw () { /* code that you don't expect to throw an exception * / }
 */

/* capture options:
   []          Capture nothing (or, a scorched earth strategy?)
   [&]         Capture any referenced variable by reference
   [=]         Capture any referenced variable by making a copy
   [=, &foo]   Capture any referenced variable by making a copy, but capture variable foo by reference
   [bar]       Capture bar by making a copy; don't copy anything else
   [this]      Capture the this pointer of the enclosing class
 */

// TODO: make examples


// Type of lambda: std::function

#include <functional>

/* Addressbook without template */
class CAnotherAddressBook
{
    public:
    std::vector<std::string> findMatchingAddresses (std::function<bool (const std::string&)> func)
    {
        std::vector<std::string> results;
        for ( auto itr = m_strAddresses.begin(), end = m_strAddresses.end(); itr != end; ++itr )
        {
            // call the function passed into findMatchingAddresses and see if it matches
            if ( func( *itr ) )
            {
                results.push_back( *itr );
            }
        }
        return results;
    }

    void addAddress(std::string strAddress) {
        m_strAddresses.emplace_back(strAddress);
    }

    private:
    std::vector<std::string> m_strAddresses;
};

TEST(Function, doWeHaveAFunction) {
    std::function<int ()> func;
    if(func) {
        FAIL();
    }
    func = [] () -> int { return 2; };
    if(!func) {
        FAIL();
    }
//    ASSERT_TRUE(func); // tests the object but not operator bool
}



