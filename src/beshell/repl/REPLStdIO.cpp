#include "REPLStdIO.hpp"
#include "REPL.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <sys/select.h>
#endif

#ifdef BESHELL_PLATFORM_PC

using namespace std ;

namespace be {
    
    REPLStdIO::REPLStdIO(REPL * repl)
        : REPLChannel(repl)
    {
        disableEcho = true ;
    }

    void REPLStdIO::setup () {
    }
    void REPLStdIO::loop () {

#ifdef _WIN32
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE) ;
        if(hStdin==NULL || hStdin==INVALID_HANDLE_VALUE) {
            return ;
        }
        if(GetFileType(hStdin)==FILE_TYPE_CHAR) {
            // 控制台：select() 不支持控制台句柄，用 _kbhit/_getch 轮询，按行组包
            static size_t line_len = 0 ;
            while(_kbhit()) {
                int c = _getch() ;
                if(c=='\r' || c=='\n') {
                    if(line_len>0) {
                        repl->onReceived(this, std::make_unique<Package>(0,LINE, buf, line_len)) ;
                        line_len = 0 ;
                    }
                } else if(line_len<sizeof(buf)-1) {
                    buf[line_len++] = (uint8_t)c ;
                }
            }
        } else {
            // 管道：PeekNamedPipe 查询可读字节，不阻塞主循环（磁盘文件重定向不支持）
            DWORD avail = 0 ;
            if(!PeekNamedPipe(hStdin, NULL, 0, NULL, &avail, NULL) || avail==0) {
                return ;
            }
            if(avail>sizeof(buf)-1) {
                avail = sizeof(buf)-1 ;
            }
            DWORD nread = 0 ;
            if(!ReadFile(hStdin, buf, avail, &nread, NULL) || nread==0) {
                return ;
            }
            repl->onReceived(this, std::make_unique<Package>(0,LINE, buf, nread)) ;
        }
        return ;
#else

        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        size_t num_readable = select(STDIN_FILENO  + 1, &readfds, NULL, NULL, &tv);
        if(errno==EINTR) {
            num_readable = select(STDIN_FILENO  + 1, &readfds, NULL, NULL, &tv);
        }
        if (num_readable == 0) {
            return ;
        } else {
            size_t num_bytes = read(STDIN_FILENO, buf, 10240-1);
            if (num_bytes < 0) {
                fprintf(stderr, "\nError on read : %s\n", strerror(errno));
                exit(1);
            }

            buf[num_bytes] = 0 ;

            repl->onReceived(this, std::make_unique<Package>(0,LINE, buf, num_bytes)) ;
        }
#endif
    }

    void REPLStdIO::sendData (const char * data, size_t datalen) {
        cout << std::setw(datalen) << data << flush ;
    }
}

#endif