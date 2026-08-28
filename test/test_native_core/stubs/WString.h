#ifndef TEST_STUB_WSTRING_H
#define TEST_STUB_WSTRING_H

#include <string>

class String {
public:
    String() = default;
    String(const char *value) : value_(value == nullptr ? "" : value) {}

    const char *c_str() const { return value_.c_str(); }

private:
    std::string value_;
};

#endif
