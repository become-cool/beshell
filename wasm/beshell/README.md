## 参考文档

中文：
https://www.cntofu.com/book/150/index.html

文件系统：
https://www.cntofu.com/book/150/zh/ch3-runtime/ch3-03-fs.md

## 日常操作

初始化 emsdk 环境 (bash)：
```
. ../emsdk/emsdk_env.sh
```
或 (fish)
```
. ../emsdk/emsdk_env.fish
```


编译：
```
make cmake
make
```


修改过 `CMakeLists.txt` 需要 `make cmake`


## Install Emscripten SDK
Download the [Emscripten SDK](https://kripken.github.io/emscripten-site/) and make sure it is in your PATH.

1. `git clone https://github.com/emscripten-core/emsdk.git`
2. `cd <path-to-emsdk>`
3. `git pull`
4. `./emsdk install latest`
5. `./emsdk activate latest`
6. `source ./emsdk_env.sh`

More info here: https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html




## Get the Emscripten-LVGL project
* `mkdir build`
* `cd build`
* `emcmake cmake ..`
* `emmake make -j8`
* A file called `index.html` will be generated. Run this in your browser.
* `emrun --port 8080 --serve_root / .`


## 编译输出

在 `CMakeList` 中：
```
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -s USE_SDL=2 -D ASMJS")
```

`-O0` 输出的 js/html 代码不做 ugly（丑化），文件很大，适合 debug;  `-O2` 输出 ugly 代码，可用于 release;



```
set_target_properties(be_lvgl PROPERTIES LINK_FLAGS "--shell-file ${PROJECT_SOURCE_DIR}/src/lvgl_shell.html -s WASM=1")
```

当 `WASM=0` 时，不生产 bin 格式的 .wasm 文件, 而是 asm.js 文件，便于调式

`-gsource-map` 可以生产 c/c++ sourcemap 便于调试，但和 `-s WASM=0`冲突

使用 `emrun --port 8080 --serve_root / .` 命令做为服务器，可以不用 `--source-map-map` 参数