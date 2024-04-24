#include "REPL.hpp"
#include "string_utils.hpp"
#include "BeShell.hpp"
#include "telnet/Telnet.hpp"
#include "debug.h"
#include "mallocf.h"
#include <cassert>
#include <iomanip>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <utime.h>
#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#endif

using namespace std ;


#define CHECK_FS_USED                           \
    if(!beshell->fs) {                          \
        ch->send("call useFS() first\n") ;      \
        return ;                                \
    }

#define ARGS_TO_DIR(i, var)                     \
    string var = beshell->fs->resolve(args[i]) ;\
    beshell->fs->toVFSPath(var) ;               \
    if( !beshell->fs->isDir(var.c_str()) ){     \
        ch->send("is not a exists dir\n") ;     \
        return ;                                \
    }

#define ARGS_TO_FILE(i, var)                    \
    string var = beshell->fs->resolve(args[i]) ;\
    beshell->fs->toVFSPath(var) ;               \
    if( !beshell->fs->isFile(var.c_str()) ){    \
        ch->send("is not a exists file\n") ;    \
        return ;                                \
    }

namespace be {


    static void printFileSize(size_t size, ostringstream & output) {
        if(size>1024*1024*1024) {
            output << std::fixed << std::setprecision(1) << (double)size/(1024*1024*1024) << "G" ;
        }
        else if(size>1024*1024) {
            output << std::fixed << std::setprecision(1) << (double)size/(1024*1024) << "M" ;
        }
        else if(size>1024) {
            output << std::fixed << std::setprecision(1) << (double)size/1024 << "K" ;
        }
        else if(size>1024) {
            output << size ;
        }
    }

    REPL::REPL(BeShell * _beshell)
        : beshell(_beshell)
    {
        registerCommand("ls",
                "Usage: ls [options...] [<path>]\n"
                "Options:\n"
                "  -(l|-list)                   output list\n"
                "  -(h)                         with -l, print human readable sizes\n"
                "  -(?|-help)                   print this message and exit\n"
            , [](BeShell * beshell, TelnetChannel * ch, Options & args){
                CHECK_FS_USED
                
                string path = beshell->fs->resolve(args[0]) ;
                beshell->fs->toVFSPath(path) ;

                if( !beshell->fs->exist(path.c_str()) ){
                    ch->send("path is not a exists\n") ;
                    return ;
                }

                bool bList = args.has("l") ;
                bool bReadable = args.has("h") ;
                
                struct dirent *dirEnt ;
                struct stat statbuf ;
                bool fisrt = true ;

                std::ostringstream dirsBuff, filesBuff;

                if( beshell->fs->isFile(path.c_str()) ){
                    stat(path.c_str(),&statbuf) ;
                    if(bReadable) {
                        printFileSize(statbuf.st_size, filesBuff) ;
                    } else {
                        filesBuff << statbuf.st_size ;
                    }

                    ch->send(filesBuff.str()) ;
                    return ;
                }

                DIR* dir = opendir(path.c_str());
                if(!dir) {
                    ch->send("Count not open dir") ;
                }

                while((dirEnt = readdir(dir))) {
                    if( strcmp(dirEnt->d_name,"..")==0 || strcmp(dirEnt->d_name,".")==0 ) {
                        continue;
                    }
                    if(bList) {
                        string childpath = path + "/" + dirEnt->d_name ;
                        // file
                        if(stat(childpath.c_str(),&statbuf)==0 && !S_ISDIR(statbuf.st_mode)) {
                            filesBuff << std::setw(8) ;
                            if(bReadable) {
                                printFileSize(statbuf.st_size, filesBuff) ;
                            } else {
                                filesBuff << statbuf.st_size ;
                            }
                            filesBuff << "   " << dirEnt->d_name << endl ;
                        }
                        // dir
                        else {
                            dirsBuff << std::setw(8) << "<dir>" << "   " << dirEnt->d_name << "/" << endl ;
                        }
                    } else {
                        if(!fisrt) {
                            filesBuff << ", " ;
                        }
                        filesBuff << dirEnt->d_name ;
                    }
                    fisrt = false ;
                }
                
                closedir(dir);

                if(!bList) {
                    filesBuff << endl ;
                }

                ch->send(dirsBuff.str()) ;
                ch->send(filesBuff.str()) ;
            }
        ) ;
        
        registerCommand("pwd", nullptr, [](BeShell * beshell, TelnetChannel * ch, Options & args){
                ch->send(beshell->fs->cwd()+"\n") ;
            }
        ) ;

        registerCommand("cd",
            "Usage: cd <path>"
        , [](BeShell * beshell, TelnetChannel * ch, Options & args){
                CHECK_FS_USED
                ARGS_TO_DIR(0, path)
                beshell->fs->setCwd(path) ;
            }
        ) ;
        
        registerCommand("cat",
            "Usage: cat <filepath>"
        , [](BeShell * beshell, TelnetChannel * ch, Options & args){
                CHECK_FS_USED
                ARGS_TO_FILE(0, path)
                int readed = 0 ;
                unique_ptr<char> content = beshell->fs->readFileSync(path.c_str(), &readed) ;
                ch->send(content.get(), readed) ;
            }
        ) ;

        registerCommand("source", 
            "Usage: source <filepath>"
        , [](BeShell * beshell, TelnetChannel * ch, Options & args){
                CHECK_FS_USED
                ARGS_TO_FILE(0, path)
                JSValue ret = beshell->engine->evalScript(path.c_str()) ;
                if(JS_IsException(ret)) {
                    beshell->engine->dumpError(-1,ch) ;
                }
                JS_FreeValue(beshell->engine->ctx,ret) ;
            }
        ) ;
        alias(".","source") ;


        registerCommand("touch", 
            "Usage: touch <filepath>"
        , [](BeShell * beshell, TelnetChannel * ch, Options & args){
            if(args.length()<1){
                ch->send("Miss filename") ;
                return ;
            }
            CHECK_FS_USED
            if(!beshell->fs->touch(beshell->fs->resolve(args[0]).c_str())) {
                ch->send("Error creating file\n");
            }
        }) ;

        registerCommand("rm", 
            "Usage: rm <filepath>"
            "Options:\n"
            "  -(r)                   recursive\n"
        , [](BeShell * beshell, TelnetChannel * ch, Options & args){
            if(args.length()<1) {
                ch->send("missing path") ;
            }
            CHECK_FS_USED
            string path = beshell->fs->resolve(args[0]) ;
            if( !beshell->fs->rm(path.c_str(), args.has("-r")) ){
                ch->send("rm faild") ;
            }
        }) ;

        registerCommand("mkdir", 
            "Usage: mkdir <filepath>"
            "Options:\n"
            "  -(r)                   recursive\n"
        , [](BeShell * beshell, TelnetChannel * ch, Options & args){
            if(args.length()<1) {
                ch->send("missing path") ;
            }
            CHECK_FS_USED
            string path = beshell->fs->resolve(args[0]) ;
            if( !beshell->fs->mkdir(path.c_str(), args.has("-r")) ){
                ch->send("mkdir falid") ;
            }
        }) ;

#ifdef ESP_PLATFORM
        registerCommand("reboot", 
            "Usage: reboot"
        , [this](BeShell * beshell, TelnetChannel * ch, Options & args){
            esp_restart() ;
        }) ;
#endif

        registerCommand("cp", 
            "Usage: cp <source> <dest>"
            "Options:\n"
            "  -r                   recursive\n"
            "  -f                   force copy (override exsits file)\n"
        , [](BeShell * beshell, TelnetChannel * ch, Options & args){
            if(args.length()<2) {
                ch->send("missing source or dest path") ;
            }
            CHECK_FS_USED
            string source = beshell->fs->resolve(args[0]) ;
            string dest = beshell->fs->resolve(args[1]) ;
            ch->send("command not implements.") ;
        }) ;

        registerCommand("mv", 
            "Usage: mv <from> <to>"
        , [](BeShell * beshell, TelnetChannel * ch, Options & args){
            if(args.length()<2) {
                ch->send("missing from or to path") ;
            }
            CHECK_FS_USED
            string source = beshell->fs->resolve(args[0]) ;
            string dest = beshell->fs->resolve(args[1]) ;
            ch->send("command not implements.") ;
        }) ;

        registerCommand("free", 
            "Usage: free"
        , [](BeShell * beshell, TelnetChannel * ch, Options & args){

            int heap_total = 0 ;
            int heap_used = 0 ;
            int heap_free = 0 ;
            int psram_total = 0 ;
            int psram_used = 0 ;
            int psram_free = 0 ;
            int dma_total = 0 ;
            int dma_used = 0 ;
            int dma_free = 0 ;

#ifdef ESP_PLATFORM
            multi_heap_info_t info;

            heap_caps_get_info(&info, MALLOC_CAP_DMA);
            dma_total = info.total_free_bytes + info.total_allocated_bytes;
            dma_used = info.total_allocated_bytes;
            dma_free = info.total_free_bytes;

            heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);
            heap_total = info.total_free_bytes + info.total_allocated_bytes;
            heap_used = info.total_allocated_bytes;
            heap_free = info.total_free_bytes;

            heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
            psram_total = info.total_free_bytes + info.total_allocated_bytes;
            psram_used = info.total_allocated_bytes;
            psram_free = info.total_free_bytes;
#endif
            string buff = "heap: total=" + to_string(heap_total) + " used=" + to_string(heap_used) + " free=" + to_string(heap_free) + "\n" ;
            buff+= "psram: total=" + to_string(psram_total) + " used=" + to_string(psram_used) + " free=" + to_string(psram_free) + "\n" ;
            buff+= "dma: total=" + to_string(dma_total) + " used=" + to_string(dma_used) + " free=" + to_string(dma_free) + "\n" ;
            ch->send(buff) ;
        }) ;

        
        registerCommand("top", 
            "Usage: top"
        , [](BeShell * beshell, TelnetChannel * ch, Options & args){

            string buff ;
#ifdef ESP_PLATFORM
            uint8_t CPU_RunInfo[400];
            memset(CPU_RunInfo, 0, 400); /* 信息缓冲区清零 */
    
            vTaskList((char *)&CPU_RunInfo); //获取任务运行时间信息

            buff = "----------------------------------------------------\r\n";
            buff+= "task_name     task_status     priority stack task_id\r\n";
            buff+= (char *)CPU_RunInfo;
            buff+= "----------------------------------------------------\r\n";

            memset(CPU_RunInfo, 0, 400); /* 信息缓冲区清零 */

            vTaskGetRunTimeStats((char *)&CPU_RunInfo);

            buff+= "task_name      run_cnt                 usage_rate   \r\n";
            buff+= (char *)CPU_RunInfo;
            buff+= "----------------------------------------------------\r\n";
#endif
            ch->send(buff) ;
        });


        registerCommand("import", 
            "Usage: import <module>"
        , [](BeShell * beshell, TelnetChannel * ch, Options & args){
            if(args.length()<1) {
                ch->send("missing module name") ;
            }
            const char * moduleName = args[0].c_str() ;
            const char * varName = moduleName ;
            char * code = (char *)mallocf("import * as %s from \"%s\";\n global.%s = %s;", varName, moduleName, varName, varName) ;
            JSValue ret = beshell->engine->eval(code, -1, "repl.import" , JS_EVAL_TYPE_MODULE) ;
            free(code) ;
            if(JS_IsException(ret)) {
                beshell->engine->dumpError(-1,ch) ;
            }
            JS_FreeValue(beshell->engine->ctx,ret) ;
        }) ;


        registerCommand("help", 
            "Usage: help"
        , [this](BeShell * beshell, TelnetChannel * ch, Options & args){
            string buff ;
            bool first = true ;
            for(const auto & pair: commands) {
                if(!first) {
                    buff+= ", " ;
                }
                first = false ;

                buff+= pair.first ;
                if(pair.second->alias.size()) {
                    buff+= "(" ;
                    bool firstAlias = true ;
                    for(const auto & a:pair.second->alias) {
                        if(!firstAlias) {
                            buff+= "," ;
                        }
                        firstAlias = false ;

                        buff+= a ;
                    }
                    buff+= ")" ;
                }
            }
            buff+= "\n" ;
            ch->send(buff) ;
        }) ;
        alias("?","help") ;
    // cd, pwd, ls, cp, rm, mv, touch, cat, stat, mkdir
    // , free, reset, reboot, onboot
    // , compile, help, "?": help
    }

    void REPL::input(Package & pkg, TelnetChannel * ch) {

        assert(beshell) ;

        int rspnId = pkg.head.fields.cmd==LINE? -1: pkg.head.fields.pkgid ;
        bool echo = pkg.head.fields.cmd==LINE && beshell->telnet ;
        if(echo && !ch->disableEcho) {
            ch->send((char *)pkg.body(), pkg.body_len) ;
        }
            
        if(pkg.body_len<1) {
            return ;
        }

        if(execCommand(ch, (const char *)pkg.body(), pkg.body_len)) {            
            ch->send(nullptr,0, rspnId, RSPN) ;
        }

        else {

            size_t content_len = pkg.body_len ;
            // 不含字符串末尾的 0
            if(pkg.body()[pkg.body_len-1] == 0){
                content_len -- ;
            } ;

            JSValue ret = beshell->engine->eval((char *)pkg.body(), content_len,"eval") ;

            if(JS_IsException(ret)) {
                beshell->engine->dumpError(rspnId) ;
            } else {
                beshell->engine->print(ret, rspnId, RSPN, ch) ;
            }
            JS_FreeValue(beshell->engine->ctx, ret) ;
            
            if(echo) {
                ch->send("\n", 1) ;
            }
        }
    }

    int parse(const char * arg, int argLen, std::vector<std::string> & args){
        std::string buf;
        bool in_quote=false;
        for (int i=0; i<argLen; i++){
            if (arg[i]=='\"'){
            in_quote=!in_quote;
                continue;
            }

            if (arg[i]==' ' && !in_quote){
                args.push_back(trim(buf));
                buf="";
                continue;
            }

            if (arg[i]=='\\'){
                i++;
                if (i>=argLen){
                    // errors.push_back("unexpected occurrence of '\\' at end of string");
                    return -1;
                }
            }

            buf+=arg[i];
        }

        if (in_quote){
            // errors.push_back("quote is not closed");
            return -2;
        }

        if (buf.length()>0) {
            args.push_back(trim(buf));
        }

        return 0 ;
    }

    bool REPL::execCommand(TelnetChannel * ch, const char * input, int iptLen) {
        if(iptLen<0) {
            iptLen = strlen(input) ;
        }

        vector<string> argv ;
        int res = parse(input, iptLen, argv) ;

        string & name = argv[0] ;
        shared_ptr<REPLCommand> cmd ;

        if(commands.count(name)>0 ) {
            cmd = commands[name] ;
        }
        else if(commandAlias.count(name)>0)  {
            cmd = commandAlias[name] ;
        }
        else {
            return false ;
        }

        if(res==-1) {
            ch->send("unexpected occurrence of '\\' at end of string\n") ;
            return true ;
        }
        else if(res==-2) {
            ch->send("quote is not closed\n") ;
            return true ;
        }
        
        cmd->args->parse(argv) ;

        if( cmd->args->has("?") ) {
            for(auto line: cmd->args->usage()){
                ch->send(line+"\n") ;
            }
            return true ;
        }

        cmd->handler(beshell, ch, * cmd->args) ;

        return true ;
    }


    // "Usage: test [options...]",
    // "Test program for the header `ParallelRadixSort.h`",
    // "",
    // "Options:",
    // "  -(c|-column) <column>      columns per line when printing",
    // "  -(n|-number) <number>      the number of random numbers",
    // "  -(r|-range) <lb> <ub>      the range of random numbers",
    // "  -(s|-show)                 print sorting result or not",
    // "  -(t|-threads) <threads>    number of threads to be used",
    // "  -(?|-help)                 print this message and exit",
    // "",
    // "Examples:",
    // "  ./test -t=4 --range 100 200",
    // "  ./test -t 2 --range 1 10000 -n=100000000",
    void REPL::registerCommand(const char * name, const char * cusage, REPLCommandHandler handler) {

        std::vector<std::string> usageLines ;
        if(cusage) {
            string usage(cusage) ;
            split(usage, "\n", usageLines) ;
        }
        Options * opts = new Options(usageLines) ;
        
        commands[name] = std::shared_ptr<REPLCommand>(new REPLCommand()) ;
        commands[name]->handler = handler ;
        commands[name]->args.reset(opts) ;
    }

    void REPL::alias(const char * alias, const char * origin) {
        if(commands.count(origin)<1) {
            return ;
        }
        commandAlias[alias] = commands[origin] ;
        commands[origin]->alias.insert(alias) ;
    }
}