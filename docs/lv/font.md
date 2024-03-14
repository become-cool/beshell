## 加载字体文件

用 `lv.loadFont(name, path)` 加载一个 lvgl 格式的字体文件，然后为 label 设置这个字体： `label.font = name`

例子：

```
import * as lv from "lv"

lv.loadFont("source", "/chinese.bin")

let label = new lv.Label(lv.screen())
label.text = "中文"
label.font = "source"
```

## 制作字体文件


https://github.com/lvgl/lv_font_conv

可以使用 `lv_font_conv` 工具将用到的字符制作成 `lvgl` 格式的二进制字体文件在运行时加载。

`lv_font_conv` 支持 TTF/WOFF/OTF 格式。

例如用思源雅黑字体制作一个只有两个中文字的字体文件：
```
lv_font_conv --font SourceHanSansCN-Light.otf --size 20 --bpp 2 --format --no-compress bin -o chinese.bin --symbols "中文"
```

> `--bpp` 是抗锯齿参数，这个参数为1时，有严重的锯齿；这个参数越大，抗锯齿效果越好，但生成的字体文件也越大。

> `--size` 参数是字体的大小，目前 beshell 不支持矢量字体，因此不能设置字号的大小，文本的大小是在制作字体文件时固定的

> `--no-compress` 不压缩

### 字符表文件

通常一个项目需要用到的字符很多，命令行输入不方便，可以将所有字符写在一个文本文件里，然后在命令里用 cat 。

例如在 words.txt 文件里提供所有用到的字符，然后：
```
lv_font_conv --font SourceHanSansCN-Light.otf --size 20 --bpp 2 --format --no-compress bin -o chinese.bin --symbols `cat words.txt`
```


### 安装 lv_font_conv

lv_font_conv 是一个 NodeJS 项目，可以用 pnpm 安装：

```
sudo pnpm i lv_font_conv -g
```
