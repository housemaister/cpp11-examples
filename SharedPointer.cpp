/*---------------------------------------------------------------------------*/
/** \file
 * Source file for SharedPointer.
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

#include <memory>

/* a custom demo class
 */
class CFancyWindow {
public:
    CFancyWindow(int iWidth, int iHeight) {
        // some construction stuff
    }

    void maximize() {
        // some code to maximize the window
    }
};

TEST(SharedPointer, HasAPointerAndCount) {
    auto window = std::make_shared<CFancyWindow>(600, 400);
    //std::shared_ptr<CFancyWindow> window(new CFancyWindow(600, 400));
    ASSERT_THAT(window.get(), ::testing::A<CFancyWindow *>());
    ASSERT_THAT(window.use_count(), ::testing::Eq(1));

    auto second = window;
    ASSERT_THAT(window.use_count(), ::testing::Eq(2));
    ASSERT_THAT(second.use_count(), ::testing::Eq(2));

    window.reset();
    ASSERT_THAT(second.use_count(), ::testing::Eq(1));
}


