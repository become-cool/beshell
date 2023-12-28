#include "REPL.hpp"
#include "BeShell.hpp"
#include "telnet/Telnet.hpp"
#include "debug.h"
#include <cassert>
#include <iomanip>

using namespace std ;

namespace be {


    REPL::REPL(BeShell * _beshell)
        : beshell(_beshell)
    {}

    void REPL::input(Package & pkg, TelnetChannel * ch) {

        assert(beshell) ;
        if(pkg.body_len<1) {
            return ;
        }

        int rspnId = pkg.head.fields.cmd==LINE? -1: pkg.head.fields.pkgid ;
        bool echo = pkg.head.fields.cmd==LINE && beshell->telnet ;

        size_t content_len = pkg.body_len ;
        // 不含字符串末尾的 0
        if(pkg.body()[pkg.body_len-1] == 0){
            content_len -- ;
        } ;

        JSValue ret = beshell->engine->eval((char *)pkg.body(), content_len,"eval") ;

        if(JS_IsException(ret)) {
            beshell->engine->dumpError(rspnId) ;
        } else {
            if(echo && !ch->disableEcho) {
                ch->send((char *)pkg.body(), pkg.body_len) ;
            }
            beshell->engine->print(ret, rspnId, RSPN, ch) ;
            if(echo) {
                ch->send("\n", 1) ;
            }
        }
        JS_FreeValue(beshell->engine->ctx, ret) ;
    }

    // std::vector<std::string> REPL::resolveCommand(const std::string &arg){
    //     std::vector<std::string> args;
    //     std::string buf;
    //     bool in_quote=false;
    //     for (std::string::size_type i=0; i<arg.length(); i++){
    //         if (arg[i]=='\"'){
    //             in_quote=!in_quote;
    //             continue;
    //         }
    //         if (arg[i]==' ' && !in_quote){
    //             args.push_back(buf);
    //             buf="";
    //             continue;
    //         }
    //         if (arg[i]=='\\'){
    //             i++;
    //             if (i>=arg.length()){
    //                 // unexpected occurrence of '\\' at end of string
    //                 return args;
    //             }
    //         }
    //         buf+=arg[i];
    //     }
    //     if (in_quote){
    //         // quote is not closed
    //         return args;
    //     }
    //     if (buf.length()>0) {
    //         args.push_back(buf);
    //     }
    //     for (size_t i=0; i<args.size(); i++) {
    //         std::cout<<"\""<<args[i]<<"\""<<std::endl;
    //     }
    //     return args;
    // }
}