#pragma once

#include <stdio.h>

class Version {
private:
	int major;
	int minor;
	int patch;

	char strbuf[10];

public:
	Version(int major, int minor, int patch);

	inline int getMajor() const {
		return major;
	}

	Version rollupPatch() const;
	Version rollupMinor() const;
	Version rollupMajor() const;

	std::string getString() const;
	operator const char*();
	bool operator ==(const Version other) const;
	bool operator <(const Version other) const;
	inline bool operator >(const Version other) const { return (other < *this); }
	inline bool operator <=(const Version other) const { return (*this == other) || (*this < other); }
	inline bool operator >=(const Version other) const { return (*this == other) || (*this > other); }
	inline bool operator !=(const Version other) const { return !(*this == other); }
};

class VersionRange {
private:
	Version start;
	Version end;
	bool include_end;
public:
	VersionRange(const Version &start, const Version &end);
	VersionRange(const Version &start, const Version &end, bool include_end);
	bool contains(const Version &point) const;
	std::string getString() const;
};

VersionRange buildMinorRange(const Version &base);
VersionRange buildMajorRange(const Version &base);