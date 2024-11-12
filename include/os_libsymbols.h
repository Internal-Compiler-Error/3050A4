#ifndef __OS_DETECTION_LIBRARY_SYMBOL_ACCESSHEADER__
#define __OS_DETECTION_LIBRARY_SYMBOL_ACCESSHEADER__

/** ------------------------------------------------------------
 ** OS-dependent library symbol management
 ** ------------------------------------------------------------
 **/

#include "os_defs.h"

#if defined( OS_BSD )
# define OS_EXPORT
# define OS_C_DECL

#elif defined( OS_LINUX )
# define OS_EXPORT
# define OS_C_DECL

#elif defined( OS_DARWIN )
# define OS_EXPORT
# define OS_C_DECL

#elif defined( OS_WINDOWS )
# define OS_C_DECL __cdecl
# if defined( OS_COMPILE_DYNAMIC )
#  define OS_EXPORT __declspec(dllexport)
# elif defined( OS_USE_DYNAMIC )
#  define OS_EXPORT __declspec(dllimport)
# elif defined( OS_STATIC )
#  define OS_EXPORT
# else
#  warning "Undeclared WINDOWS setup - assuming dynamic"
#  define OS_EXPORT __declspec(dllexport)
# endif

#else
#  warning "Unknown OS -- assuming no shared object symbol decoration required"
#  define OS_EXPORT

#endif /* end of cascaded if */

#endif /* __OS_DETECTION_LIBRARY_SYMBOL_ACCESSHEADER__ */
