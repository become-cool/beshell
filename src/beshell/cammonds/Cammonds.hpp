#pragma once
#include <vector>
#include <set>
#include <memory>
#include "../repl/Protocal.hpp"
#include "../deps/Options.hpp"


namespace be {
    class BeShell ;
    class REPLChannel ;


	typedef std::function<void(BeShell * , REPLChannel *, int rspnId, Options & args)> CammondsCommandHandler;
    typedef struct {
        CammondsCommandHandler handler = nullptr ;
        std::unique_ptr<Options> args ;
        std::set<std::string> alias ;
        bool ignoreLogin = false ;
    } CammondsCommand ;

    class Cammonds {
    private:
        BeShell * beshell ;
        std::map<std::string, std::shared_ptr<CammondsCommand> > commands ;
        std::map<std::string, std::shared_ptr<CammondsCommand> > commandAlias ;
        std::string password = "" ;
        bool logined = true ; // setPassword() 时设置为 false
    public:
        Cammonds(BeShell * beshell) ;
        void input(Package & pkg, REPLChannel * ch) ;

        bool execCommand(REPLChannel *, int rspnId, const char * input, int iptLen=-1) ;
        std::shared_ptr<CammondsCommand> registerCommand(const char * name, const char * usage, CammondsCommandHandler handler) ;
        void alias(const char * alias, const char * origin) ;
        void setPassword(const std::string & pwd) ;
    } ;
}