# 关于FS模块
FS模块支持跨平台的文件系统操作，提供了一些常用的文件系统操作API，如文件的读写、打开、关闭、更名、删除等等。如果你在[esp-idf](https://github.com/espressif/esp-idf)上使用fs模块，你可能需要先了解[esp-idf分区表](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-guides/partition-tables.html)、[esp-idf vsf](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-reference/storage/vfs.html)
# FS模块的使用
## esp-idf
在idf上,fs提供了两种实现，分别是littlefs和原生VFS实现，其中原生的VFS仅支持只读操作，而littlefs支持读写操作，但是littlefs的实现比较复杂，需要在分区表中配置，而且需要在menuconfig中配置，这里不做介绍，有兴趣的可以自行查阅[esp-idf分区表](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-guides/partition-tables.html)。
### 初始化

```c
BeShell beShell;
beShell.useBasic();
//RawFs
beShell.useFS("/rawFS",new RawFS("/spiffs"));
//LittleFs
beShell.useFS("/littleFS",new LittleFS("/spiffs"));
```