#pragma once

#include <stdint.h>
#include <stddef.h>
#include <functional>

#define PKG_LINE_BUFFSIZE 256

namespace be {

	enum PkgDef {
		HEAD1 = 5 ,
		HEAD2 = 18 ,
		HEAD2_V = 19 ,
	} ;
	enum Cmd {
		LINE = 0					// 行数数据
		, RUN = 1					// 执行js代码，无返回值
		, CALL = 2					// 执行js代码，有返回值
		, CALL_ASYNC = 3			// 执行js代码，有异步返回值
		, RSPN = 4					// CMD_CALL/CALL_ASYNC 的返回
		, EXCEPTION = 5				// CMD_RUN/CMD_CALL/CALL_ASYNC 的异常
		, CALLBACK = 6				// 执行本地js代码(远程发起)
		, OUTPUT = 7				// 远程输出
		, DATA = 8					// 数据包(用于 CMD_FILE_PULL_REQ 的回应)
		, MSG = 9					// 主动向客户端推送的事件

		, FILE_OPEN_REQ = 10		// 打开文件，等待后续接受文件后写入. DATA区为文件内容
		, FILE_OFFSET_REQ = 11		// 偏移
		, FILE_PUSH_REQ = 12		// 发送文件
		, FILE_CLOSE_REQ = 13		// 关闭文件
		, FILE_PULL_REQ = 20		// 拉取文件. DATA区格式：路径(0结尾字符串) + Offset(uint32) + MaxSize(uint16)
		, RESET = 30				// 重置命令. DATA区格式：boot_level (uint8)
		, READY = 31				// 系统准备就绪事件
	} ;

	class Package {
	public:

		union {
			uint8_t raw[8] ;
			struct {
				uint8_t h1;
				uint8_t h2;
				uint8_t pkgid;
				uint8_t cmd;
				uint8_t len1;
				uint8_t len2;
				uint8_t len3;
				uint8_t len4;
			} fields ;
		} head ;
		uint8_t head_len ;

		uint8_t * body = nullptr;
		size_t body_len ;

		uint8_t verifysum ;
		
		Package(uint8_t pkgid=0, uint8_t cmd=0, uint8_t * body=nullptr, size_t body_len=0, uint8_t h1=HEAD1, uint8_t h2=HEAD2) ;
		~Package() ;
		
		size_t decodeBodyLength() ;
		void encodeBodyLength() ;

		uint8_t calculateVerifysum() ;
		
		size_t calculateSize() ;
		void pack() ;
		void reset(uint8_t pkgid=0, uint8_t cmd=0,size_t bodylen=0);
		void mallocBody(bool endZero=false) ;
		void freeBody() ;
	} ;
	
    class Parser ;
    class State {
	protected:
		Parser * parser ;

		uint8_t received = 0 ;
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
		// void saveToBuff(uint8_t * data, size_t len) ;
	public:
		using State::State ;
		void parse(uint8_t * bytes, size_t * len) ;
    } ;
    
	// 包头:固定长度区(4字节)
    class StatePkgHeadFixed: public State {
	private:
		// uint8_t received= 0 ;

		bool receivePkgLen(uint8_t ** bytes, size_t * len) ;
	public:
		using State::State ;
		void parse(uint8_t * bytes, size_t * len) ;
		void enter() ;
    } ;

	// 包头:包长部分(1-4字节)
    class StatePkgHeadLength: public State {
	private:
		// uint8_t buff[4] ;
		// uint8_t received = 0 ;
	public:
		using State::State ;
		void parse(uint8_t * bytes, size_t * len) ;
		void enter() ;
	} ;

	// 包身
    class StatePkgBody: public State {
	private:
		// size_t received = 0 ;
		// bool verifysum_received = false ;
	public:
		using State::State ;
		void parse(uint8_t * bytes, size_t * len) ;
		void enter() ;
		bool checkVerifysum() ;
	} ;

	// 上下文类
	typedef std::function<void(Package & pkg)> PackageProcFunc;
	void defaultPkgProcFunc(Package & pkg) ;

    class Parser {
		private:
			Package pkg ;
			State * current = nullptr ;

			StateLine * stateLine ;
			StatePkgHeadFixed * statePkgHeadFixed ;
			StatePkgHeadLength * statePkgHeadLength ;
			StatePkgBody * statePkgBody ;

			PackageProcFunc handler = nullptr ;

			Package * newPackage(uint8_t _cmd=0, uint8_t _pkgid=0, size_t _data_len=0) ;
			void commitPackage() ;

			void changeState(State * state, uint8_t * bytes, size_t * len) ;

		public:
			uint8_t H1 = HEAD1 ;
			uint8_t H2 = HEAD2 ;
			uint8_t H2V = HEAD2 ;
			Parser(PackageProcFunc handler=defaultPkgProcFunc,uint8_t H1=HEAD1,uint8_t H2=HEAD2,uint8_t H2V=HEAD2) ;
			~Parser() ;
			void parse(uint8_t * bytes, size_t len) ;
			void setPkgHead(uint8_t H1=HEAD1,uint8_t H2=HEAD2) ;
			void setProcessHandler(PackageProcFunc handler) ;
			void test() {}

		friend class StateLine ;
		friend class StatePkgHeadFixed ;
		friend class StatePkgHeadLength ;
		friend class StatePkgBody ;
    } ;
}