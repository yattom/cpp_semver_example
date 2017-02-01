#include "stdafx.h"

#include <string>
#include <malloc.h>
#include "semver.h"

Version::Version(int major, int minor, int patch) {
	if (major < 0 || minor < 0 || patch < 0) {
		throw std::exception();
	}
	this->major = major;
	this->minor = minor;
	this->patch = patch;
}

std::string Version::getString() const {
	return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
}

Version::operator const char*() {
	sprintf_s<10>(strbuf, "%d.%d.%d", major, minor, patch);
	return strbuf;
}

bool Version::operator ==(const Version other) const {
	return major == other.major && minor == other.minor && patch == other.patch;
}

bool Version::operator <(const Version other) const {
	if (major < other.major) {
		return true;
	}
	if (major == other.major && minor < other.minor) {
		return true;
	}
	if (major == other.major && minor == other.minor && patch < other.patch) {
		return true;
	}
	return false;
}

Version Version::rollupPatch() const {
	return Version(major, minor, patch + 1);
}

Version Version::rollupMinor() const {
	return Version(major, minor + 1, 0);
}

Version Version::rollupMajor() const {
	return Version(major + 1, 0, 0);
}

VersionRange::VersionRange(const Version &start, const Version &end) : VersionRange(start, end, true) {
}

VersionRange::VersionRange(const Version &start, const Version &end, bool include_end) : start(start), end(end), include_end(include_end) {
}

bool VersionRange::contains(const Version &point) const {
	if (include_end) {
		return start <= point && point <= end;
	}
	else {
		return start <= point && point < end;
	}
}

std::string VersionRange::getString() const {
	if (include_end) {
		return start.getString() + "<=version<=" + end.getString();
	}
	else {
		return start.getString() + "<=version<" + end.getString();
	}
}

VersionRange buildMinorRange(const Version &base) {
	Version end = base.rollupMinor();
	return VersionRange(base, end, false);
}

VersionRange buildMajorRange(const Version &base) {
	if (base.getMajor() == 0) {
		Version end = base.rollupMinor();
		return VersionRange(base, end, false);
	}
	else {
		Version end = base.rollupMajor();
		return VersionRange(base, end, false);
	}
}