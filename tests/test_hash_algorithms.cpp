// tests/test_hash_algorithms.cpp
#include <gtest/gtest.h>
#include "hash/MD5Hash.h"
#include "hash/SHA256Hash.h"

TEST(MD5Test, KnownTestVectors) {
    MD5Hash md5;
    
    EXPECT_EQ(md5.hashToString("password"), 
              "5f4dcc3b5aa765d61d8327deb882cf99");
    EXPECT_EQ(md5.hashToString("123456"), 
              "e10adc3949ba59abbe56e057f20f883e");
    EXPECT_EQ(md5.hashToString(""), 
              "d41d8cd98f00b204e9800998ecf8427e");
}

TEST(SHA256Test, KnownTestVectors) {
    SHA256Hash sha256;
    
    EXPECT_EQ(sha256.hashToString("password"),
              "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8");
    EXPECT_EQ(sha256.hashToString("123456"),
              "8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92");
}

TEST(HashVerificationTest, VerifyHashes) {
    MD5Hash md5;
    EXPECT_TRUE(md5.verify("password", "5f4dcc3b5aa765d61d8327deb882cf99"));
    EXPECT_FALSE(md5.verify("wrong", "5f4dcc3b5aa765d61d8327deb882cf99"));
}
