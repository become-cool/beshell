// BeShell PC 平台入口示例
#include <stdio.h>
#include <beshell/BeShell.hpp>
#include <beshell/fs/FS.hpp>
#include <beshell/module/animator/Animator.hpp>

#ifdef BESHELL_PLATFORM_PC_WASM
#include <emscripten.h>
#endif

using namespace be ;

#ifdef BESHELL_PLATFORM_PC_WASM

// WASM 入口：浏览器/Node 环境中由 emscripten 主循环驱动，
// JS 侧通过 beshell_eval(code) 执行脚本（标准输出自动映射到 console.log）

static BeShell beshell ;

extern "C" {

// 供 JS 侧 ccall('beshell_eval', ...) 调用
EMSCRIPTEN_KEEPALIVE
void beshell_eval(const char * code) {
    beshell.engine->eval(code) ;
}

}

static void wasm_loop() {
    beshell.loop() ;
}

int main() {

    beshell.use<FS>() ;

    // WASM 虚拟文件系统（MEMFS）以 "/" 为根，main.js 由 --preload-file 打包进去
    FS::setPrefix("/") ;

    beshell.setup() ;
    beshell.evalMain("/main.js") ;

    // while(1) 主循环交给 emscripten 逐帧驱动（浏览器中不能阻塞主线程）
    emscripten_set_main_loop(wasm_loop, 0, 0) ;

    return 0 ;
}

#else

int main(int argc, char * argv[]) {

    // 重定向到管道/文件时默认全缓冲，强制无缓冲便于日志实时输出
    setvbuf(stdout, NULL, _IONBF, 0) ;

    BeShell beshell ;

    // 启用 FS 模块（PC 侧经由标准 C 文件 API 访问本地文件）
    beshell.use<FS>() ;

    // 启用 animator 模块（Timeline/Tween，无硬件依赖，可供 PC 侧测试与开发）
    beshell.use<animator::Animator>() ;

    // PC 上使用本地目录作为 VFS 根："/xxx" 映射到 "./fs/xxx"
    FS::setPrefix("./fs") ;

    // 启动 BeShell：先执行 main 脚本，再进入主循环（REPL 走标准输入输出）
    beshell.main(argc>1 ? argv[1] : "/main.js") ;

    return 0 ;
}

#endif
