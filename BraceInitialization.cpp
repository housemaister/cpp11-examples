/*---------------------------------------------------------------------------*/
/** \file
 * Source file for BraceInitialization.
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
#include <initializer_list>
#include <string>
#include <map>

/* The Basics */

TEST(BraceInitialization, SupportsNumericTypes) {
    int iValue{42};
    ASSERT_THAT(iValue, ::testing::Eq(42));

    double dValue{12.2};
    ASSERT_THAT(dValue, ::testing::DoubleEq(12.2));
}

TEST(BraceInitialization, SupportsStrings) {
    std::string strValue{"Stefan"};
    ASSERT_THAT(strValue, ::testing::Eq("Stefan"));
}

TEST(BraceInitialization, SupportsCollectionTypes) {
    std::vector<std::string> strNames {"alpha", "beta", "gamma" };
    ASSERT_THAT(strNames, ::testing::ElementsAre("alpha", "beta", "gamma"));
}

TEST(BraceInitialization, SupportsArrays) {
    int iArray[] {1, 1, 2, 3, 5, 8};
    ASSERT_THAT(iArray, ::testing::ElementsAre(1, 1, 2, 3, 5, 8));
}


/* also maps: */

TEST(BraceInitialization, SupportsMaps) {
    std::map<std::string,unsigned int> mapSizes {
        {"Stefan", 180}, {"Tom", 185}
    };

    ASSERT_THAT(mapSizes["Stefan"], ::testing::Eq(180));
    ASSERT_THAT(mapSizes["Tom"], ::testing::Eq(185));
}


/* compare to old school initialisation */

TEST(OldSchoolCollectionInitialization, SignificantlyCluttersTests) {
    std::vector<std::string> strNames;

    strNames.push_back("alpha");
    strNames.push_back("beta");
    strNames.push_back("gamma");

    ASSERT_THAT(strNames, ::testing::ElementsAre("alpha", "beta", "gamma"));
}


/* various other usages */

TEST(BraceInitialization, CanBeUsedOnConstructionInitializationList) {
    struct ReportCard {
        std::string strArrayGrades[5];
        ReportCard() : strArrayGrades{"A", "B", "C", "D", "F"} {}
    } card;

    ASSERT_THAT(card.strArrayGrades, ::testing::ElementsAre("A", "B", "C", "D", "F"));
}

TEST(BraceInitialization, CanBeUsedForReturnValues) {
    struct ReportCard {
        std::vector<std::string> gradesForAllClasses() {
            std::string strScience{"A"};
            std::string strMath{"B"};
            std::string strEnglish{"B"};
            std::string strHistory{"A"};
            return {strScience, strMath, strEnglish, strHistory};
        }
    } card;

    ASSERT_THAT(card.gradesForAllClasses(), ::testing::ElementsAre("A", "B", "B", "A"));
}

TEST(BraceInitialization, CanBeUsedForArguments) {
    struct ReportCard {
        std::vector<std::string> m_vecSubjects_;

        void addSubjects(std::vector<std::string> subjects) {
            m_vecSubjects_ = subjects;
        }
    } card;

    card.addSubjects({"social studies", "art"});

    ASSERT_THAT(card.m_vecSubjects_, ::testing::ElementsAre("social studies", "art"));
}


/* Direct initialisation at member level!! */
/* Does not work with gcc 4.6 */
/* "Non-static data member initializers" in gcc 4.7 */
TEST(BraceInitialization, CanBeUsedToDirectlyInitializeMemberVariables) {
    struct ReportCard {
        std::string strArrayGrades[5] {"A", "B", "C", "D", "F"};
    } card;

    ASSERT_THAT(card.strArrayGrades, ::testing::ElementsAre("A", "B", "C", "D", "F"));
}

TEST(MemInit, OverridesMemberVariableInitialization) {
    struct ReportCard {
        std::string strSchoolName{"Trailblazer Elementary"};
        ReportCard() : strSchoolName{"Chipeta Elementary"} {}
    } card;

    ASSERT_THAT(card.strSchoolName, ::testing::Eq("Chipeta Elementary"));
}
/**/
/* Temporary Type Names */

TEST(BraceInitialization, EliminatesNeedToSpecifyTempTypeName) {
    struct StudentScore {
        StudentScore(std::string strName, int iScore)
        : m_strName(strName), m_iScore(iScore) {}
        std::string m_strName;
        int m_iScore;
    };
    struct ReportCard {
        std::vector<StudentScore> m_vecScores;
        void AddStudentScore(StudentScore score) {
            m_vecScores.push_back(score);
        }
    } card;

    // old school: cardAddStudentScore(StudentScore("Jane", 93));
    card.AddStudentScore({"Jane", 93});

    auto studentScore = card.m_vecScores[0];
    ASSERT_THAT(studentScore.m_strName, ::testing::Eq("Jane"));
    ASSERT_THAT(studentScore.m_iScore, ::testing::Eq(93));
}


/* Defaults */

TEST(BraceInitialization, WillDefaultUnspecifiedElements) {
   int iValue{};
   ASSERT_THAT(iValue, ::testing::Eq(0));

   double dValue{};
   ASSERT_THAT(dValue, ::testing::Eq(0.0));

   bool bValue{};
   ASSERT_THAT(bValue, ::testing::Eq(false));

   std::string strValue{};
   ASSERT_THAT(strValue, ::testing::Eq(""));
}

TEST(BraceInitialization, WillDefaultUnspecifiedArrayElements) {
   int iArray[3]{};
   ASSERT_THAT(iArray, ::testing::ElementsAre(0, 0, 0));

   int iArraySecond[3]{100, 101};
   ASSERT_THAT(iArraySecond, ::testing::ElementsAre(100, 101, 0));
}

TEST(BraceInitialization, UsesDefaultConstructorToDeriveDefaultValue) {
   struct ReportCard {
      std::string m_strSchool;
      ReportCard() : m_strSchool("Trailblazer") {}
      ReportCard(std::string school) : m_strSchool(school) {}
   };

   ReportCard card{};

   ASSERT_THAT(card.m_strSchool, ::testing::Eq("Trailblazer"));
}

TEST(BraceInitialization, CanIncludeEqualsSign) {
   int i = {99};
   ASSERT_THAT(i, ::testing::Eq(99));
}

/* Warning is not emited in gcc 4.6 */
TEST(BraceInitialization, AvoidsNarrowingConversionProblem) {
    int badPi = 3.1415927;
    ASSERT_THAT(badPi, ::testing::Eq(3));

    int pi{3.1415927}; // emits warning by default
    // warning: narrowing conversion of ‘3.1415926999999999e+0’ from ‘double’ to ‘int’ inside { } [-Wnarrowing]
    // fails: ASSERT_THAT(pi, ::testing::Eq(3.1415927));
    ASSERT_THAT(pi, ::testing::Eq(3));
}
/**/

/* use with auto */

TEST(BraceInitialization, IsProbablyNotWhatYouWantWhenUsingAuto) {
    auto x{9};
    ASSERT_THAT(x, ::testing::A<const std::initializer_list<int>>());
    // in other words, the following assignment passes compilation. Thus x is *not* an int.
    const std::initializer_list<int> y = x;
}

/* tricky bits (as it is c++) */
/* Does not work with gcc 4.6 */
TEST(BraceInitialization, AvoidsTheMostVexingParse) {
   struct IsbnService {
      IsbnService() {}
      std::string m_strAddress{"http://example.com"};
   };

   struct Library {
      IsbnService m_Service;
      Library(IsbnService service) : m_Service{service} {}
      std::string Lookup(const std::string& m_strIsbn) { return "book name"; }
   };

   Library library(IsbnService()); // declares a function(!)
//   auto name = library.Lookup("123"); // does not compile

   Library libraryWithBraceInit{IsbnService()};
   auto name = libraryWithBraceInit.Lookup("123");

   ASSERT_THAT(name, ::testing::Eq("book name"));
}
/**/

