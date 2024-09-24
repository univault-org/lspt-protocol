#include <gtest/gtest.h>
#include "../../src/core/srpt_package.h"

TEST(SRPTPackageTest, CreatePackage) {
    SRPTPackage package(1024 * 1024);  // 1 MB package
    EXPECT_EQ(1024 * 1024, package.getSize());
}

TEST(SRPTPackageTest, PackageIdentification) {
    SRPTPackage package1(1024 * 1024);
    SRPTPackage package2(1024 * 1024);
    EXPECT_NE(package1.getId(), package2.getId());
}

TEST(SRPTPackageTest, PackageMetadata) {
    SRPTPackage package(1024 * 1024);
    package.setMetadata("content-type", "application/octet-stream");
    EXPECT_EQ("application/octet-stream", package.getMetadata("content-type"));
}