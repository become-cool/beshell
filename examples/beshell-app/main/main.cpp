#include <stdio.h>
#include <beshell/BeShell.hpp>

using namespace std ;
using namespace be ;


#ifdef __cplusplus
extern "C" {
#endif

void app_main(void)
{
    BeShell beshell;

    // 启用 BeShell 模块
    beshell.use<FS>() ;
    beshell.use<Serial>() ;
    beshell.use<NVS>() ;
    beshell.use<WiFi>() ;
    beshell.use<animator::Animator>() ;
#if CONFIG_BT_BLUEDROID_ENABLED
    beshell.use<BT>() ;
#endif
#if CONFIG_USB_OTG_SUPPORTED
    // beshell.use<CDC>() ;
    beshell.use<REPLCDC>() ;
#endif

    // 挂载 js 分区到文件的根目录
    FS::mount("/", new LittleFS("js", true)) ;

    // 启动 BeShell
    beshell.main("/main.js");
}

#ifdef __cplusplus
}
#endif
