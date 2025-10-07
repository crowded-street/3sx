//===========================================================================
// mw_stdarg.h	(c)2001 Metrowerks Inc. All rights reserved.
//===========================================================================

/* This header is for compatibility with GCC's implementation of
 * the standard variable argument lists. The Metrowerks compiler
 * complies with the GCC 2.95.x implementation. Newer versions of GCC
 * have changed their internal implementation so this header remaps
 * the macros to something the Metrowerks compiler can still interpret.
 */

#pragma once

#ifndef __MW_STDARG_H__
#define __MW_STDARG_H__

#define _STDARG_H /* to avoid pulling in gcc's stdarg.h */

#undef __va_rounded_size
#undef __va_reg_size
#undef __va_next_addr
#undef __va_copy

#undef va_start
#undef va_end

typedef char* va_list;

enum {
    __no_type_class = -1,
    __void_type_class,
    __integer_type_class,
    __char_type_class,
    __enumeral_type_class,
    __boolean_type_class,
    __pointer_type_class,
    __reference_type_class,
    __offset_type_class,
    __real_type_class,
    __complex_type_class,
    __function_type_class,
    __method_type_class,
    __record_type_class,
    __union_type_class,
    __array_type_class,
    __string_type_class,
    __set_type_class,
    __file_type_class,
    __lang_type_class
};

#ifdef __mips64
#define __va_rounded_size(__TYPE) (((sizeof(__TYPE) + 8 - 1) / 8) * 8)
#define __va_reg_size 8
#else
#define __va_rounded_size(__TYPE) (((sizeof(__TYPE) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define __va_reg_size 4
#endif

#define va_start(__AP, __LASTARG)                                                                                      \
    (__AP = ((char*)__builtin_next_arg(__LASTARG) -                                                                    \
             (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * __va_reg_size)))

#define va_end(x) ((void)0)

#define __va_next_addr(__AP, __type) ((__AP += __va_reg_size) - __va_reg_size)

#define va_arg(__AP, __type)                                                                                           \
    ((__va_rounded_size(__type) <= __va_reg_size) ? *(__type*)(void*)__va_next_addr(__AP, __type)                      \
                                                  : (__builtin_classify_type(*(__type*)0) >= __record_type_class       \
                                                         ? **(__type**)(void*)__va_next_addr(__AP, __type)             \
                                                         : *(__type*)(void*)__va_next_addr(__AP, __type)))

#define __va_copy(dest, src) (dest) = (src)

#endif /* __MW_STDARG_H__ */

/* mw_stdarg.h */
