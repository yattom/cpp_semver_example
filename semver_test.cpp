#include "stdafx.h"
#include <gtest/gtest.h>

#include <string>
#include "semver.h"

TEST(semver, ������\��) {
	Version ver(1, 4, 2);
	std::string actual = ver.getString();
	EXPECT_EQ("1.4.2", actual);
}

TEST(semver, ����_�������ꍇ) {
	Version v1(1, 4, 2);
	Version v2(1, 4, 2);
	EXPECT_TRUE(v1 == v2);
}

TEST(semver, ����_�������Ȃ��ꍇ) {
	Version v1(1, 4, 2);
	Version v2(1, 4, 3);
	EXPECT_TRUE(v1 != v2);
}

TEST(semver, ��r_������) {
	EXPECT_TRUE(Version(1, 4, 2) < Version(1, 4, 3));
	EXPECT_TRUE(Version(1, 4, 2) < Version(1, 5, 2));
	EXPECT_TRUE(Version(1, 4, 2) < Version(2, 4, 2));
	EXPECT_FALSE(Version(1, 4, 2) < Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 4, 3) < Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 5, 2) < Version(1, 4, 2));
	EXPECT_FALSE(Version(2, 4, 2) < Version(1, 4, 2));
}

TEST(semver, ��r_��������������) {
	EXPECT_TRUE(Version(1, 4, 2) <= Version(1, 4, 3));
	EXPECT_TRUE(Version(1, 4, 2) <= Version(1, 5, 2));
	EXPECT_TRUE(Version(1, 4, 2) <= Version(2, 4, 2));
	EXPECT_TRUE(Version(1, 4, 2) <= Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 4, 3) <= Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 5, 2) <= Version(1, 4, 2));
	EXPECT_FALSE(Version(2, 4, 2) <= Version(1, 4, 2));
}

TEST(semver, ��r_�傫��) {
	EXPECT_TRUE(Version(1, 4, 3) > Version(1, 4, 2));
	EXPECT_TRUE(Version(1, 5, 2) > Version(1, 4, 2));
	EXPECT_TRUE(Version(2, 4, 2) > Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 4, 2) > Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 4, 2) > Version(1, 4, 3));
	EXPECT_FALSE(Version(1, 4, 2) > Version(1, 5, 2));
	EXPECT_FALSE(Version(1, 4, 2) > Version(2, 4, 2));
}

TEST(semver, ��r_�傫����������) {
	EXPECT_TRUE(Version(1, 4, 2) >= Version(1, 4, 2));
	EXPECT_TRUE(Version(1, 4, 3) >= Version(1, 4, 2));
	EXPECT_TRUE(Version(1, 5, 2) >= Version(1, 4, 2));
	EXPECT_TRUE(Version(2, 4, 2) >= Version(1, 4, 2));
	EXPECT_TRUE(Version(1, 4, 2) >= Version(1, 4, 2));
	EXPECT_FALSE(Version(1, 4, 2) >= Version(1, 4, 3));
	EXPECT_FALSE(Version(1, 4, 2) >= Version(1, 5, 2));
	EXPECT_FALSE(Version(1, 4, 2) >= Version(2, 4, 2));
}

TEST(semver, �s���ȃo�[�W����_major) {
	try {
		Version v1(-1, 4, 2);
		FAIL();
	}
	catch (std::exception ex) {
	}
}

TEST(semver, �s���ȃo�[�W����_minor) {
	try {
		Version v1(1, -1, 2);
		FAIL();
	}
	catch (std::exception ex) {
	}
}

TEST(semver, �s���ȃo�[�W����_patch) {
	try {
		Version v1(1, 4, -1);
		FAIL();
	}
	catch (std::exception ex) {
	}
}

TEST(semver, �p�b�`�o�[�W�����A�b�v) {
	Version v(1, 4, 2);
	Version newV = v.rollupPatch();
	EXPECT_EQ("1.4.3", newV.getString());
}

TEST(semver, �}�C�i�[�o�[�W�����A�b�v) {
	Version v(1, 4, 2);
	Version newV = v.rollupMinor();
	EXPECT_EQ("1.5.0", newV.getString());
}

TEST(semver, ���W���[�o�[�W�����A�b�v) {
	Version v(1, 4, 2);
	Version newV = v.rollupMajor();
	EXPECT_EQ("2.0.0", newV.getString());
}

TEST(semver, ������\��2) {
	Version ver(1, 4, 2);
	EXPECT_STREQ("1.4.2", (const char*)ver);
}

TEST(semver, �o�[�W�����͈�_�f�t�H���g�͏�[���܂�) {
	VersionRange range = VersionRange(Version(1, 4, 0), Version(1, 5, 0));
	EXPECT_FALSE(range.contains(Version(1, 3, 9)));
	EXPECT_TRUE(range.contains(Version(1, 4, 0)));
	EXPECT_TRUE(range.contains(Version(1, 4, 2)));
	EXPECT_TRUE(range.contains(Version(1, 5, 0)));
	EXPECT_FALSE(range.contains(Version(1, 5, 1)));
}

TEST(semver, �o�[�W�����͈�_��[���܂܂Ȃ�) {
	VersionRange range = VersionRange(Version(1, 4, 0), Version(1, 5, 0), false);
	EXPECT_FALSE(range.contains(Version(1, 3, 9)));
	EXPECT_TRUE(range.contains(Version(1, 4, 0)));
	EXPECT_TRUE(range.contains(Version(1, 4, 2)));
	EXPECT_FALSE(range.contains(Version(1, 5, 0)));
	EXPECT_FALSE(range.contains(Version(1, 5, 1)));
}

TEST(semver, �o�[�W�����͈�_������\��) {
	VersionRange range = VersionRange(Version(1, 4, 0), Version(1, 5, 0));
	EXPECT_EQ("1.4.0<=version<=1.5.0", range.getString());
}

TEST(semver, �o�[�W�����͈�_��[���܂܂Ȃ�_������\��) {
	VersionRange range = VersionRange(Version(1, 4, 0), Version(1, 5, 0), false);
	EXPECT_EQ("1.4.0<=version<1.5.0", range.getString());
}

TEST(semver, �}�C�i�[�o�[�W�����͈�) {
	VersionRange range = buildMinorRange(Version(1, 4, 2));
	EXPECT_EQ("1.4.2<=version<1.5.0", range.getString());
}

TEST(semver, ���W���[�o�[�W�����͈�) {
	VersionRange range = buildMajorRange(Version(1, 4, 2));
	EXPECT_EQ("1.4.2<=version<2.0.0", range.getString());
}

TEST(semver, �}�C�i�[�o�[�W�����͈�_development) {
	VersionRange range = buildMinorRange(Version(0, 4, 2));
	EXPECT_EQ("0.4.2<=version<0.5.0", range.getString());
}

TEST(semver, ���W���[�o�[�W�����͈�_development) {
	VersionRange range = buildMajorRange(Version(0, 4, 2));
	EXPECT_EQ("0.4.2<=version<0.5.0", range.getString());
}