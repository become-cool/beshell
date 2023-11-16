#pragma once

#include <stdint.h>
#include <stddef.h>

namespace beprotocal {

    class Parser ;
    class State {
		protected:
			Parser * parser ;
		public:
			State(Parser * parser);
			virtual ~State() ;
        	virtual void parse(uint8_t * bytes, size_t len) = 0 ;
    } ;
    
    class StateLine: public State {
		public:
			StateLine(Parser * parser) ;
			~StateLine() ;
			void parse(uint8_t * bytes, size_t len) ;
    } ;
    
    class StatePkg: public State {
		public:
			StatePkg(Parser * parser) ;
			~StatePkg() ;
			void parse(uint8_t * bytes, size_t len) ;
    } ;

    class StatePkgStream: public StatePkg {
		public:
			void parse(uint8_t * bytes, size_t len) ;
    } ;

    class Parser {
		private:
			State * current ;
			State * stateLine ;
			State * statePkg ;
			State * statePkgStream ;
		public:
			Parser() ;
			~Parser() ;
			void parse(uint8_t * bytes, size_t len) ;

		friend class StateLine ;
		friend class StatePkg ;
    } ;
}