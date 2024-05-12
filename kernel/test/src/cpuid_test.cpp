#include "gtest/gtest.h"
#include "cpuid_mock.h"
#include "cpuid.h"

TEST(CpuIdTest, TestGetVendorId) {
    setCpuVendor("GenuineIntel");

    char name[13];
    getVendorIdString(name, 13);
    ASSERT_STREQ(name, "GenuineIntel");

    getVendorIdString(name, 6);
    ASSERT_STREQ(name, "Genu");

    getVendorIdString(name, 9);
    ASSERT_STREQ(name, "GenuineI");
}
