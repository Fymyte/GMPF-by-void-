#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdlib.h>
#include <stdio.h>




//
// DEBUG ZONE
//

// define it for the debug mode
//#define GMPF_DEBUG

// print error
#define PRINTERR fprintf(stderr, "[error] %s::%d %s", __FILE__, __LINE__, __func__);

// safe malloc with error gestion - A FINIR
#ifdef GMPF_DEBUG
    #define SAFE_MALLOC(_type, _name, _err_ptr) \
            _type * _name = malloc(sizeof(_type)); \
            if (_name == NULL) \
            { \
                PRINTERR; \
                *_err_ptr == 1; \
                return; \
            }
#else
    #define SAFE_MALLOC(_type, _name, _err_ptr) \
            _type * _name = malloc(sizeof(_type)); \
            if (_name == NULL) \
            { \
                *_err_ptr == 1; \
                return; \
            }
#endif

// debug print
#ifdef GMPF_DEBUG
    #define D_PRINT(_fmt, ...) \
        fprintf(stderr, "[debug: %s::%d %s] " _fmt, \
        __FILE__, __LINE__, __func__, __VA_ARGS__);
#else
    #define D_PRINT(_fmt, ...)
#endif




#endif /* DEBUG_H_ */