#include "cpuid.h"
#include "cpuid_mock.h"
#include "gtest/gtest.h"

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

TEST(CpuIdTest, GetCpuFeatures) {
    CpuFeatures features;
    features.raw1 = 0x01234567;
    features.raw2 = 0xFEDCBA98;

    setFeatures(features);

    CpuFeatures readFeatures;
    getCpuFeatures(&readFeatures);

    ASSERT_EQ(features.raw1, readFeatures.raw1);
    ASSERT_EQ(features.raw2, readFeatures.raw2);

    ASSERT_TRUE(readFeatures.sse3);
    ASSERT_TRUE(readFeatures.pclmulqdq);
    ASSERT_FALSE(readFeatures.monitor);
    ASSERT_FALSE(readFeatures.hypervisor);

    ASSERT_TRUE(readFeatures.pbe);
    ASSERT_TRUE(readFeatures.ia64);
    ASSERT_FALSE(readFeatures.fpu);
    ASSERT_TRUE(readFeatures.ss);
}
