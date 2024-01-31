# 关于FS模块
FS模块支持跨平台的文件系统操作，提供了一些常用的文件系统操作API，如文件的读写、打开、关闭、更名、删除等等。如果你在[esp-idf](https://github.com/espressif/esp-idf)上使用fs模块，你可能需要先了解[esp-idf分区表](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-guides/partition-tables.html)、[esp-idf vsf](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-reference/storage/vfs.html)、[esp-idf嵌入二进制数据](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32c3/api-guides/build-system.html?highlight=embed_files#cmake-embed-data)
# FS模块的使用
## esp-idf
在idf上,fs提供了两种实现，分别是littlefs和原生VFS实现，其中原生的VFS仅支持只读操作，但性能比其它文件系统快5-10倍；而littlefs支持读写操作，但是littlefs的实现比较复杂，需要在分区表中配置，而且需要在menuconfig中配置，这里不做介绍，有兴趣的可以自行查阅[esp-idf分区表](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-guides/partition-tables.html)。
### 初始化

```c
BeShell beShell;
beShell.useBasic();

//RawFs
//需要idf_component_register(EMBED_FILES "binary.data"）
//详细使用方式参考 https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32c3/api-guides/build-system.html?highlight=embed_files#cmake-embed-data
extern const unsigned char binary_data_start[] asm("_binary_binary_data_start");
extern const unsigned char binary_data_end[]   asm("_binary_binary_data_end");
const size_t binary_data_size = (binary_data_end - binary_data_start);
beShell.useFS("/rawFS",new RawFS((const char *)binary_data_start,binary_data_size+1));

//LittleFs
// littleFS:vfs路径根目录
// spiffs:分区表中的分区名
// true:是否格式化
beShell.useFS("/littleFS",new LittleFS("/spiffs",true));
```
### js api
```js
//同步创建目录
//path:目录路径
let path = "/littleFS/test";
//recursive:是否递归创建
let recursive = true;
//mkResult:创建结果
let mkResult = fs.mkdirSync(path, recursive);

//同步读取文件
fs.readFileSync(path + "/test.txt");
//同步写入文件
//append:是否追加
let append = true;
fs.writeFileSync(path + "/test.txt", "hello world", append);
//同步重命名文件
fs.renameSync(path + "/test.txt", path + "/new_test.txt");
// 文件分区的信息， 返回的对象包括了分区的总大小，和已用大小
let info = fs.info(path + "/test.txt");
console.log(info);
//{total: 1024, used: 0}
//同步获取文件状态,如果文件不存在,返回 null; 否则返回包含详细信息的对象
let fileInfo = fs.statSync(path + "/test.txt");
console.log(fileInfo);
//同步返回目录是否存在,bool值
let exsits = fs.exsitsSync(path);
//同步判断是否是文件
let isFile = fs.isFileSync(path + "/test.txt");
//同步判断是否是目录
let isDir = fs.isDirSync(path);
//同步删除目录，如果目录为空，返回false
fs.rmdirSync(path, recursive);
//同步删除文件
fs.unlinkSync(path + "/test.txt");
//同步删除文件或目录，如果是目录，recursive为true时，递归删除
fs.rmSync(path, recursive);
```