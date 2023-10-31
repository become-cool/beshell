#include <stdio.h>//vsprintf and vsnprintf
#include <stdlib.h>//size_t, malloc
#include "mallocf.h"

char *strdupf( const char *fmt, ... )
{
  char *result;
  va_list args;

  va_start( args, fmt );
  result = vstrdupf( fmt, args );
  va_end( args );

  return result;
}

char *vstrdupf( const char *fmt, va_list args )
{
  char *buf;
  va_list copy;
  int size;

  va_copy( copy, args );

  size = vstrlenf( fmt, copy );

  va_end( copy );

  /*
   * If vstrlenf returns a negative number, that indicates a wide-char
   * encoding error.  Return NULL in that case.
   */
  if ( size < 0 )
    return NULL;

  buf = malloc( size + 1 );

  if ( !buf )
    return NULL;

  vsprintf( buf, fmt, args );

  return buf;
}

int strlenf( const char *fmt, ... )
{
  va_list args;
  int result;
  char buf;

  va_start( args, fmt );
  /*
   * In case the size limit is too small,
   * vsnprintf returns the number of chars that would
   * have been printed, had the size limit been large enough.
   * (Unless there is an encoding error, see below.)
   * So, by specifying a size limit of 0, we provoke
   * vsnprintf to return the total length of the formatted
   * string.
   */
  result = vsnprintf( &buf, 0, fmt, args );
  va_end( args );

  /*
   * Note: In case of an encoding error involving wide-chars,
   * vsnprintf returns a negative number, which is then returned
   * by strlenf.
   */
  return result;
}

int vstrlenf( const char *fmt, va_list args )
{
  char buf;

  /*
   * For explanation of the following line, see the comments
   * in "strlenf" above.
   */
  return vsnprintf( &buf, 0, fmt, args );
}
