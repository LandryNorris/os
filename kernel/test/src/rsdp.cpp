#include "gtest/gtest.h"

extern "C" {
#include "acpi/rsdp.h"
}

TEST(RSDPTest, CheckValidRSDP) {
    char* signatureText = "RSD PTR ";
    char* oem = "Test   ";
    RSDP validRsdp = {
            .checksum = 101,
            .revision = 0,
            .rsdtAddress = 0x123456
    };

    memcpy(validRsdp.signature, signatureText, 8);
    memcpy(validRsdp.oemid, oem, 6);

    ASSERT_TRUE(validateRSDP(&validRsdp));
}

TEST(RSDPTest, CheckIncorrectSignature) {
    char* signatureText = "Wrong Si";
    char* oem = "Test   ";
    RSDP validRsdp = {
            .checksum = 155,
            .revision = 0,
            .rsdtAddress = 0x123456
    };

    memcpy(validRsdp.signature, signatureText, 8);
    memcpy(validRsdp.oemid, oem, 6);

    ASSERT_FALSE(validateRSDP(&validRsdp));
}

TEST(RSDPTest, CheckIncorrectRevision) {
    char* signatureText = "RSD PTR ";
    char* oem = "Test   ";
    RSDP validRsdp = {
            .checksum = 97,
            .revision = 4,
            .rsdtAddress = 0x123456
    };

    memcpy(validRsdp.signature, signatureText, 8);
    memcpy(validRsdp.oemid, oem, 6);

    ASSERT_FALSE(validateRSDP(&validRsdp));
}

TEST(RSDPTest, CheckIncorrectChecksum) {
    char* signatureText = "RSD PTR ";
    char* oem = "Test   ";
    RSDP validRsdp = {
            .checksum = 123,
            .revision = 0,
            .rsdtAddress = 0x123456
    };

    memcpy(validRsdp.signature, signatureText, 8);
    memcpy(validRsdp.oemid, oem, 6);

    ASSERT_FALSE(validateRSDP(&validRsdp));
}
