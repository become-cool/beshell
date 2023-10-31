
### 编译

```
mkdir build
cd build
cmake ..
make
```

### 运行
pc simulation 模式下没有实现 repl ，输入命令不方便，可以使用 rlwrap 命令
```
rlwrap build/beshell
```

### windows 下使用 mingw g++ 

```
cd build
cmake -G "MinGW Makefiles" ..
```

### 使用 ninjia 替代 make (编译更快)

> ninjia 在执行时，依赖关系搜索很快

```
cd build
rm CMakeCache.txt
cmake -G Ninja -S .. -B .
ninja
```