#include <gtest/gtest.h>
#include "../../src/core/lspt_package.h"

TEST(LSPTPackageTest, CreatePackage) {
    LSPTPackage package(1024 * 1024);  // 1 MB package
    EXPECT_EQ(1024 * 1024, package.getSize());
}

TEST(LSPTPackageTest, PackageIdentification) {
    LSPTPackage package1(1024 * 1024);
    LSPTPackage package2(1024 * 1024);
    EXPECT_NE(package1.getId(), package2.getId());
}

TEST(LSPTPackageTest, PackageMetadata) {
    LSPTPackage package(1024 * 1024);
    package.setMetadata("content-type", "application/octet-stream");
    EXPECT_EQ("application/octet-stream", package.getMetadata("content-type"));
}