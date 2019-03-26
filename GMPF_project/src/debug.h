#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdlib.h>
#include <stdio.h>




//
// DEBUG ZONE
//

// define it for the debug mode
#define GMPF_DEBUG


#define GET_UI(_type, _name) \
    (_type *) (gtk_builder_get_object(data->builder, _name));

// print error
#define PRINTERR fprintf(stderr, "[error] %s::%d %s", __FILE__, __LINE__, __func__);

// safe malloc with error gestion - A FINIR
#ifdef GMPF_DEBUG
    #define SAFE_MALLOC(_type, _name, ...) \
            _type * _name = malloc(sizeof(_type)); \
            if (_name == NULL) \
            { \
                PRINTERR; \
                return __VA_ARGS__; \
            }
#else
    #define SAFE_MALLOC(_type, _name, ...) \
            _type * _name = malloc(sizeof(_type)); \
            if (_name == NULL) \
            { \
                return __VA_ARGS__; \
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
