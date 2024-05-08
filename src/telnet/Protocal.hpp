#pragma once

#include <stdint.h>
#include <stddef.h>
#include <functional>
#include <iostream>
#include <memory>

#define PKG_LINE_BUFFSIZE 256

namespace be {

	enum PkgDef {
		HEAD1 = 5 ,
		HEAD2 = 18 ,
		HEAD2_V = 19 ,
	} ;
	#undef OUTPUT  // esp-idf define MACRO as same name
	enum Cmd {
		LINE = 0					// 行数数据
		, RUN = 1					// 执行js代码，无返回值
		, CALL = 2					// 执行js代码，有返回值
		, CALL_ASYNC = 3			// 执行js代码，有异步返回值
		, RSPN = 4					// CMD_CALL/CALL_ASYNC 的返回
		, EXCEPTION = 5				// CMD_RUN/CMD_CALL/CALL_ASYNC 的异常
		, CALLBACK = 6				// 执行本地js代码(远程发起)
		, OUTPUT = 7				// 远程输出
		, MSG = 8					// 主动向客户端推送的事件

		, FILE_MIN = 32
		, FILE_OPEN = 32
		, FILE_OPEN_APPEND = 33
		, FILE_OFFSET = 34
		, FILE_PUSH = 35
		, FILE_CLOSE = 36
		, FILE_PULL = 38			// 拉取文件. DATA区格式：路径(0结尾字符串) + Offset(uint32) + Length(uint16)
		, FILE_PULL_DATA = 39		// 拉取文件返回的数据
		, FILE_MAX = 39

		, READY = 60				// 系统准备就绪事件
	} ;

	class State ;
	class StateLine ;
	class StatePkgHeadFields ;
	class StatePkgBodyLength ;
	class StatePkgBody ;

	class Package {
		friend class State ;
		friend class StateLine ;
		friend class StatePkgHeadFields ;
		friend class StatePkgBodyLength ;
		friend class StatePkgBody ;
	private :
		uint8_t * _body = nullptr ;
		bool sharedBody = false ;
		
		void mallocBody(uint16_t len, bool endZero=false) ;
	public:

		union {
			uint8_t raw[6] ;
			struct {
				uint8_t h1;
				uint8_t h2;
				uint8_t pkgid;
				uint8_t cmd;
				uint8_t len1;
				uint8_t len2;
			} fields ;
		} head ;
		uint8_t head_len = 5 ;

		size_t body_len = 0 ;
		size_t chunk_len = 0 ;

		uint8_t verifysum ;
		
		Package(uint8_t pkgid=0, uint8_t cmd=0, uint8_t * body=nullptr, size_t body_len=0, uint8_t h1=HEAD1, uint8_t h2=HEAD2) ;
		Package(const Package *) ;
		~Package() ;

		uint8_t * body() const ;
		
		size_t decodeBodyLength() ;
		void encodeBodyLength() ;

		uint8_t calculateVerifysum() ;
		static uint8_t calculateVerifysum(uint8_t * data, size_t datalen, uint8_t baseval=0) ;
		
		size_t calculateSize() ;
		void pack() ;
		void reset(uint8_t pkgid=0, uint8_t cmd=0,size_t bodylen=0);
		void freeBody() ;
	} ;

	class UniquePackage: public Package {
		std::unique_ptr<uint8_t[]> unique_body ;
	} ;
	
    class Parser ;
    class State {
	protected:
		Parser * parser ;

		uint16_t received = 0 ;
		bool receiveToBuff(uint8_t ** bytes, size_t * len, uint8_t * buff, size_t buffsize) ;
	public:
		uint8_t label ;
		State(Parser * parser);
		virtual ~State() ;
		virtual void parse(uint8_t * bytes, size_t * len) = 0 ;
		virtual void enter() ;
    } ;
    
    class StateLine: public State {
	private:
		uint8_t buff[PKG_LINE_BUFFSIZE] ;
	public:
		using State::State ;
		void parse(uint8_t * bytes, size_t * len) ;
    } ;
    
	// 包头:固定长度区(4字节)
    class StatePkgHeadFields: public State {
	private:
		bool receivePkgLen(uint8_t ** bytes, size_t * len) ;
	public:
		using State::State ;
		void parse(uint8_t * bytes, size_t * len) ;
		void enter() ;
    } ;

	// 包头:包长部分(1-2字节)
    class StatePkgBodyLength: public State {
	private:
	public:
		using State::State ;
		void parse(uint8_t * bytes, size_t * len) ;
		void enter() ;
	} ;

	// 包身
    class StatePkgBody: public State {
	private:
		uint8_t verifysum ;
	public:
		using State::State ;
		void parse(uint8_t * bytes, size_t * len) ;
		void enter() ;
	} ;

	typedef std::function<void(std::unique_ptr<Package> pkg)> PackageProcFunc;
	void defaultPkgProcFunc(std::unique_ptr<Package> pkg) ;

	// 上下文类
    class Parser {
		private:
			Package * pkg = nullptr ;
			State * current = nullptr ;

			StateLine * stateLine ;
			StatePkgHeadFields * statePkgHeadFields ;
			StatePkgBodyLength * statePkgBodyLength ;
			StatePkgBody * statePkgBody ;

			PackageProcFunc handler = nullptr ;

			Package * newPackage(uint8_t _cmd=0, uint8_t _pkgid=0, size_t _data_len=0) ;
			void commitPackage() ;

			void changeState(State * state, uint8_t * bytes, size_t * len) ;

		public:
			uint8_t H1 = HEAD1 ;
			uint8_t H2 = HEAD2 ;
			Parser(PackageProcFunc handler=defaultPkgProcFunc,uint8_t H1=HEAD1,uint8_t H2=HEAD2) ;
			~Parser() ;
			void parse(uint8_t * bytes, size_t len) ;
			void setPkgHead(uint8_t H1=HEAD1,uint8_t H2=HEAD2) ;
			void setProcessHandler(PackageProcFunc handler) ;
			

		friend class StateLine ;
		friend class StatePkgHeadFields ;
		friend class StatePkgBodyLength ;
		friend class StatePkgBody ;
    } ;
}