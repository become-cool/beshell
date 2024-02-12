#include "REPL.hpp"
#include "string_utils.hpp"
#include "BeShell.hpp"
#include "telnet/Telnet.hpp"
#include "debug.h"
#include <cassert>
#include <iomanip>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <utime.h>
#ifdef ESP_PLATFORM
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
                ARGS_TO_DIR(0, path)
                bool bList = args.has("l") ;
                bool bReadable = args.has("h") ;

                DIR* dir = opendir(path.c_str());
                if(!dir) {
                    ch->send("Count not open dir") ;
                }

                std::ostringstream dirsBuff, filesBuff;
                
                struct dirent *dirEnt ;
                struct stat statbuf ;
                bool fisrt = true ;

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
                                if(statbuf.st_size>1024*1024*1024) {
                                    filesBuff << std::fixed << std::setprecision(1) << (double)statbuf.st_size/(1024*1024*1024) << "G" ;
                                }
                                else if(statbuf.st_size>1024*1024) {
                                    filesBuff << std::fixed << std::setprecision(1) << (double)statbuf.st_size/(1024*1024) << "M" ;
                                }
                                else if(statbuf.st_size>1024) {
                                    filesBuff << std::fixed << std::setprecision(1) << (double)statbuf.st_size/1024 << "K" ;
                                }
                                else if(statbuf.st_size>1024) {
                                    filesBuff << statbuf.st_size ;
                                }
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
            ch->send("command not implements.") ;
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