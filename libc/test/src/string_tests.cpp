#include "gtest/gtest.h"
#include "libc_modified.h"

TEST(StringTests, StringLengthTest) {
    ASSERT_EQ(9, os_libc_strlen("some text"));
    ASSERT_EQ(10, os_libc_strlen("some text\n"));
    ASSERT_EQ(0, os_libc_strlen(""));
}

TEST(StringTests, StringCopyTest) {
    char* str1 = (char*)"some text to copy";
    char str2[50] = {0};

    ASSERT_STRNE(str1, str2);

    os_libc_strcpy(str2, str1);

    ASSERT_STREQ(str1, str2);
}

TEST(StringTests, StringConcatenateTest) {
    char str1[50] = {0};
    char* str2 = (char*) "World";

    os_libc_strcat(str1, (char*) "Hello, ");
    ASSERT_STREQ(str1, "Hello, ");

    os_libc_strcat(str1, str2);
    ASSERT_STREQ(str1, "Hello, World");
}

TEST(StringTests, StringCompareTest) {
    char* str1 = (char*)"some text";
    char* str2 = (char*)"some text";
    char* str3 = (char*)"some other test";
    char* str4 = (char*)"z";

    ASSERT_EQ(0, os_libc_strcmp(str1, str2));
    ASSERT_GT(os_libc_strcmp(str1, str3), 0);
    ASSERT_LT(os_libc_strcmp(str1, str4), 0);
}

TEST(StringTests, StringSearchTest) {
    char* str = (char*)"some text to search";

    ASSERT_STREQ(os_libc_strchr(str, 't'), "text to search");
    ASSERT_FALSE(os_libc_strchr(str, '\a'));
}
