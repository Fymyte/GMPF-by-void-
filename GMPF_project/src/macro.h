#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdlib.h>
#include <stdio.h>



/****************************DEBUG ZONE****************************************/
#define GMPF_DEBUG

#ifdef GMPF_DEBUG
    #define D_PRINT(_fmt, ...) \
        fprintf(stderr, "[debug: %s::%d %s] " _fmt, \
            __FILE__, __LINE__, __func__, __VA_ARGS__);
#else
    #define D_PRINT(_fmt, ...)
#endif

#define PRINTERR fprintf(stderr, "[error] %s::%d %s", \
        __FILE__, __LINE__, __func__);



/****************************MAGIC ZONE****************************************/

#define GET_UI(_type, _var, _name) \
    _type * _var = (_type *) (gtk_builder_get_object(data->builder, _name));

#define INIT_UI() \
    SGlobalData *data = (SGlobalData *)user_data;

#ifdef __GNUC__
    #define UNUSED __attribute__((__unused__))
#else
    #define UNUSED
#endif

#define RAD_FROM_DEG(_x) _x = _x * G_PI / 180;


// print error

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


#endif /* DEBUG_H_ */
