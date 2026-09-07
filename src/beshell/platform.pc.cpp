#include "platform.hpp"

// PC 平台实现：整个文件仅在 BESHELL_PLATFORM_PC 下编译出内容
// （ESP 构建会编译此文件但为空；PC 构建由 pc/CMakeLists.txt 排除 platform.esp32.cpp）
#ifdef BESHELL_PLATFORM_PC

#ifdef _WIN32
#include <windows.h>
#else
#include <thread>
#endif
#include <chrono>

namespace be::platform {

    bool init() {
        return true ;
    }

    bool networkInit() {
        return true ;
    }

    void sleepMs(uint32_t ms) {
#ifdef _WIN32
        Sleep(ms) ;
#else
        std::this_thread::sleep_for(std::chrono::milliseconds(ms)) ;
#endif
    }

    uint64_t uptimeMs() {
        // 注意：MinGW.org 旧运行时的 winbase.h 没有 GetTickCount64，统一用 steady_clock
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count() ;
    }

}
#endif
