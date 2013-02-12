/*---------------------------------------------------------------------------*/
/** \file
 * Source file for AutoAndDecltype.
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

/* old style without auto: */
template <typename BuiltType, typename Builder>
void
oldMakeAndProcessObject (const Builder& builder)
{
    BuiltType val = builder.makeObject();
    // do stuff with val
}

struct MyObj {
};

struct MyObjBuilder {
    MyObj makeObject() const {
        return MyObj();
    }
};

TEST(OldStyle, builderObjectWithType) {
    MyObjBuilder builder;
    oldMakeAndProcessObject<MyObj>( builder );
}

/* new style with auto: */

template <typename Builder>
void
makeAndProcessObject (const Builder& builder)
{
    auto val = builder.makeObject();
    // do stuff with val
}

TEST(NewStyle, builderObjectWithoutType) {
    MyObjBuilder builder;
    makeAndProcessObject( builder );
}



/* The Joy of decltype
auto multiply (int x, int y) -> int;
*/

class Person
{
public:
    enum PersonType { ADULT, CHILD, SENIOR };
    void setPersonType (PersonType person_type);
    PersonType getPersonType ();
private:
    PersonType _person_type;
};

void Person::setPersonType (PersonType person_type)
{
    _person_type = person_type;
}

/* instead of
Person::PersonType Person::getPersonType ()
{
    return _person_type;
}
  write: */

auto Person::getPersonType () -> PersonType
{
    return _person_type;
}


TEST(Decltype, simpleOperations) {
    int x = 3;
    decltype(x) y = x; // same thing as auto y = x;
}

TEST(Decltype, withBuilderObject) {
    MyObjBuilder builder;
    decltype( builder.makeObject() );
}

/* combine to */
template <typename Builder>
auto
evenBettermakeAndProcessObject (const Builder& builder) -> decltype( builder.makeObject() )
{
    auto val = builder.makeObject();
    // do stuff with val
    return val;
}

TEST(NewStyle, evenBetterBuilderObjectWithoutType) {
    MyObjBuilder builder;
    auto object = evenBettermakeAndProcessObject( builder );
    ASSERT_THAT(object, ::testing::A<MyObj>());
}

