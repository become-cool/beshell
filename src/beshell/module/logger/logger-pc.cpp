#ifdef BESHELL_PLATFORM_PC

#include "logger-imp.hpp"
#include "../../platform.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <string>

// PC 平台 logger 实现：内存环形缓冲（仅保留最近 bufferSize 字节，用于调试，不落盘）

namespace {
    std::string s_buffer ;
    size_t s_capacity = 0 ;
}

extern "C" {

int log_setup(const char * name, size_t bufferSize, size_t start, size_t end, int core_id) {
    (void)name ; (void)start ; (void)end ; (void)core_id ;
    s_buffer.clear() ;
    s_buffer.reserve(bufferSize) ;
    s_capacity = bufferSize ;
    return LOG_SUCCESS ;
}

int log_flush() {
    return LOG_SUCCESS ;
}

size_t log_write(const void *buf, size_t length, bool timestampPrefix) {
    if(!buf || !s_capacity) {
        return 0 ;
    }
    if(timestampPrefix) {
        char ts[32] ;
        snprintf(ts, sizeof(ts), "[%llu] ", (unsigned long long)be::platform::uptimeMs()) ;
        s_buffer.append(ts) ;
    }
    s_buffer.append((const char *)buf, length) ;
    if(s_buffer.size() > s_capacity) {
        s_buffer.erase(0, s_buffer.size() - s_capacity) ;
    }
    return length ;
}

size_t log_writef(const char *format, va_list args) {
    char tmp[1024] ;
    int n = vsnprintf(tmp, sizeof(tmp), format, args) ;
    if(n<=0) {
        return 0 ;
    }
    if((size_t)n >= sizeof(tmp)) {
        n = sizeof(tmp)-1 ;
    }
    return log_write(tmp, (size_t)n, false) ;
}

void log_erase() {
    s_buffer.clear() ;
}

void log_clear() {
    s_buffer.clear() ;
}

size_t log_length() {
    return s_buffer.size() ;
}

size_t log_read(size_t start_pos, size_t read_length, uint8_t* buffer) {
    if(!buffer || start_pos >= s_buffer.size()) {
        return 0 ;
    }
    size_t n = s_buffer.size() - start_pos ;
    if(n > read_length) {
        n = read_length ;
    }
    memcpy(buffer, s_buffer.data()+start_pos, n) ;
    return n ;
}

void log_set_write_offset(size_t addr) {
    (void)addr ;
}

void log_set_reade_offset(size_t addr) {
    (void)addr ;
}

void log_fill(const char data) {
    s_buffer.assign(s_capacity? s_capacity: 1024, data) ;
}

}

#endif
