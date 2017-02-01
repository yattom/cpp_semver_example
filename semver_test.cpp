#include "stdafx.h"
#include <gtest/gtest.h>

#include <string>
#include "semver.h"

TEST(semver, 文字列表現) {
	Version ver(1, 4, 2);
	std::string actual = ver.getString();
	EXPECT_EQ("1.4.2", actual);
}

TEST(semver, 等価_等しい場合) {
	Version v1(1, 4, 2);
	Version v2(1, 4, 2);
	EXPECT_TRUE(v1 == v2);
}

TEST(semver, 等価_等しくない場合) {
	Version v1(1, 4, 2);
	Version v2(1, 4, 3);
	EXPECT_TRUE(v1 != v2);
}

TEST(semver, 比較_小さい) {
	EXPECT_TRUE(Version(1, 4, 2) < Version(1, 4, 3));
	EXPECT_TRUE(Version(1, 4, 2) < Version(1, 5, 2));
	EXPECT_TRUE(Version(1, 4, 2) < Version(2, 4, 2));
	EXPECT_FALSE(Version(1, 4, 2) < Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 4, 3) < Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 5, 2) < Version(1, 4, 2));
	EXPECT_FALSE(Version(2, 4, 2) < Version(1, 4, 2));
}

TEST(semver, 比較_小さいか等しい) {
	EXPECT_TRUE(Version(1, 4, 2) <= Version(1, 4, 3));
	EXPECT_TRUE(Version(1, 4, 2) <= Version(1, 5, 2));
	EXPECT_TRUE(Version(1, 4, 2) <= Version(2, 4, 2));
	EXPECT_TRUE(Version(1, 4, 2) <= Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 4, 3) <= Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 5, 2) <= Version(1, 4, 2));
	EXPECT_FALSE(Version(2, 4, 2) <= Version(1, 4, 2));
}

TEST(semver, 比較_大きい) {
	EXPECT_TRUE(Version(1, 4, 3) > Version(1, 4, 2));
	EXPECT_TRUE(Version(1, 5, 2) > Version(1, 4, 2));
	EXPECT_TRUE(Version(2, 4, 2) > Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 4, 2) > Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 4, 2) > Version(1, 4, 3));
	EXPECT_FALSE(Version(1, 4, 2) > Version(1, 5, 2));
	EXPECT_FALSE(Version(1, 4, 2) > Version(2, 4, 2));
}

TEST(semver, 比較_大きいか等しい) {
	EXPECT_TRUE(Version(1, 4, 2) >= Version(1, 4, 2));
	EXPECT_TRUE(Version(1, 4, 3) >= Version(1, 4, 2));
	EXPECT_TRUE(Version(1, 5, 2) >= Version(1, 4, 2));
	EXPECT_TRUE(Version(2, 4, 2) >= Version(1, 4, 2));
	EXPECT_TRUE(Version(1, 4, 2) >= Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 4, 2) >= Version(1, 4, 3));
	EXPECT_FALSE(Version(1, 4, 2) >= Version(1, 5, 2));
	EXPECT_FALSE(Version(1, 4, 2) >= Version(2, 4, 2));
}

TEST(semver, 不正なバージョン_major) {
	try {
		Version v1(-1, 4, 2);
		FAIL();
	}
	catch (std::exception ex) {
	}
}

TEST(semver, 不正なバージョン_minor) {
	try {
		Version v1(1, -1, 2);
		FAIL();
	}
	catch (std::exception ex) {
	}
}

TEST(semver, 不正なバージョン_patch) {
	try {
		Version v1(1, 4, -1);
		FAIL();
	}
	catch (std::exception ex) {
	}
}

TEST(semver, パッチバージョンアップ) {
	Version v(1, 4, 2);
	Version newV = v.rollupPatch();
	EXPECT_EQ("1.4.3", newV.getString());
}

TEST(semver, マイナーバージョンアップ) {
	Version v(1, 4, 2);
	Version newV = v.rollupMinor();
	EXPECT_EQ("1.5.0", newV.getString());
}

TEST(semver, メジャーバージョンアップ) {
	Version v(1, 4, 2);
	Version newV = v.rollupMajor();
	EXPECT_EQ("2.0.0", newV.getString());
}

TEST(semver, 文字列表現2) {
	Version ver(1, 4, 2);
	EXPECT_STREQ("1.4.2", (const char*)ver);
}

TEST(semver, バージョン範囲_デフォルトは上端を含む) {
	VersionRange range = VersionRange(Version(1, 4, 0), Version(1, 5, 0));
	EXPECT_FALSE(range.contains(Version(1, 3, 9)));
	EXPECT_TRUE(range.contains(Version(1, 4, 0)));
	EXPECT_TRUE(range.contains(Version(1, 4, 2)));
	EXPECT_TRUE(range.contains(Version(1, 5, 0)));
	EXPECT_FALSE(range.contains(Version(1, 5, 1)));
}

TEST(semver, バージョン範囲_上端を含まない) {
	VersionRange range = VersionRange(Version(1, 4, 0), Version(1, 5, 0), false);
	EXPECT_FALSE(range.contains(Version(1, 3, 9)));
	EXPECT_TRUE(range.contains(Version(1, 4, 0)));
	EXPECT_TRUE(range.contains(Version(1, 4, 2)));
	EXPECT_FALSE(range.contains(Version(1, 5, 0)));
	EXPECT_FALSE(range.contains(Version(1, 5, 1)));
}

TEST(semver, バージョン範囲_文字列表現) {
	VersionRange range = VersionRange(Version(1, 4, 0), Version(1, 5, 0));
	EXPECT_EQ("1.4.0<=version<=1.5.0", range.getString());
}

TEST(semver, バージョン範囲_上端を含まない_文字列表現) {
	VersionRange range = VersionRange(Version(1, 4, 0), Version(1, 5, 0), false);
	EXPECT_EQ("1.4.0<=version<1.5.0", range.getString());
}

TEST(semver, マイナーバージョン範囲) {
	VersionRange range = buildMinorRange(Version(1, 4, 2));
	EXPECT_EQ("1.4.2<=version<1.5.0", range.getString());
}

TEST(semver, メジャーバージョン範囲) {
	VersionRange range = buildMajorRange(Version(1, 4, 2));
	EXPECT_EQ("1.4.2<=version<2.0.0", range.getString());
}

TEST(semver, マイナーバージョン範囲_development) {
	VersionRange range = buildMinorRange(Version(0, 4, 2));
	EXPECT_EQ("0.4.2<=version<0.5.0", range.getString());
}

TEST(semver, メジャーバージョン範囲_development) {
	VersionRange range = buildMajorRange(Version(0, 4, 2));
	EXPECT_EQ("0.4.2<=version<0.5.0", range.getString());
}