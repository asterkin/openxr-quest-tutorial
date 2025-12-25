// Copyright 2023, The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0

// OpenXR Tutorial for Khronos Group

#pragma once
#include <android/log.h>
#include <cstring>

#include <iostream>
class AndroidStreambuf : public std::streambuf {
public:
    enum {
        bufsize = 128
    };  // ... or some other suitable buffer size
    android_LogPriority logPriority;
    AndroidStreambuf(android_LogPriority p = ANDROID_LOG_DEBUG) {
        logPriority = p;
        this->setp(buffer, buffer + bufsize - 1);
    }

private:
    int overflow(int c) {
        if (c == traits_type::eof()) {
            *this->pptr() = traits_type::to_char_type(c);
            this->sbumpc();
        }
        return this->sync() ? traits_type::eof() : traits_type::not_eof(c);
    }
    std::string str;
    int sync() {
        int rc = 0;
        if (this->pbase() != this->pptr()) {
            char writebuf[bufsize + 1];
            memcpy(writebuf, this->pbase(), this->pptr() - this->pbase());
            writebuf[this->pptr() - this->pbase()] = '\0';
            str += writebuf;
            for (size_t pos = 0; pos < str.length(); pos++) {
                if (str[pos] == '\n') {
                    str[pos] = 0;
                    rc = __android_log_write(logPriority, "openxr_tutorial", str.c_str()) > 0;
                    if (str.length() > pos + 1)
                        str = str.substr(pos + 1, str.length() - pos - 1);
                    else
                        str.clear();
                }
            }
            this->setp(buffer, buffer + bufsize - 1);
        }
        return rc;
    }

    char buffer[bufsize];
};
class DebugOutput {
public:
    AndroidStreambuf androidCout;
    AndroidStreambuf androidCerr;

    DebugOutput()
        : androidCout(), androidCerr(ANDROID_LOG_ERROR) {
        auto *oldout = std::cout.rdbuf(&androidCout);
        auto *olderr = std::cerr.rdbuf(&androidCerr);
        if (oldout != &androidCout) {
            __android_log_write(ANDROID_LOG_DEBUG, "openxr_tutorial", "redirected cout");
        }
        std::cout << "Testing cout redirect." << std::endl;
        if (olderr != &androidCerr) {
            __android_log_write(ANDROID_LOG_WARN, "openxr_tutorial", "redirected cerr");
        }
        std::cerr << "Testing cerr redirect." << std::endl;
    }
};

#include <sstream>

#define XR_TUT_LOG_TAG "openxr_tutorial"
#define XR_TUT_LOG(...) {                                                           \
        std::ostringstream ostr;                                                    \
        ostr<<__VA_ARGS__;                                                          \
        __android_log_write(ANDROID_LOG_DEBUG, XR_TUT_LOG_TAG, ostr.str().c_str()); \
    }
#define XR_TUT_LOG_ERROR(...) {                                                     \
        std::ostringstream ostr;                                                    \
        ostr<<__VA_ARGS__;                                                          \
        __android_log_write(ANDROID_LOG_ERROR, XR_TUT_LOG_TAG, ostr.str().c_str()); \
    }
