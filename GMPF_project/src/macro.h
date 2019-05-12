#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdlib.h>
#include <stdio.h>



/****************************DEBUG ZONE****************************************/
#define GMPF_DEBUG

#ifdef GMPF_DEBUG
    #define D_PRINT(_fmt, ...) \
        fprintf(stdout, "[debug: %s::%d %s] " _fmt "\n", \
            __FILE__, __LINE__, __func__, __VA_ARGS__);
#else
    #define D_PRINT(_fmt, ...)
#endif

#define PRINTERR(_fmt) \
        fprintf(stderr, "[error %s::%d %s] "_fmt "\n", \
            __FILE__, __LINE__, __func__);



/****************************MAGIC ZONE****************************************/

#define GET_UI(_type, _var, _name) \
    _type * _var = (_type *) (gtk_builder_get_object(G_user_data.builder, _name)); \
    if (!_var) \
    { \
        D_PRINT("Unable to get %s\n", _name); \
    }

#ifdef __GNUC__
    #define UNUSED __attribute__((__unused__))
#else
    #define UNUSED
#endif

#define RAD_FROM_DEG(_x) (_x * G_PI / 180)

#define GET_LAYERMNGR(_flowbox) \
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(_flowbox);


// print error

// safe malloc with error gestion - A FINIR
#ifdef GMPF_DEBUG
    #define SAFE_MALLOC(_type, _name, ...) \
            _type * _name = malloc(sizeof(_type)); \
            if (_name == NULL) \
            { \
                PRINTERR ("Unable to malloc"); \
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

#define REFRESH_IMAGE(_layer) \
    if (_layer->image) \
        g_object_unref(_layer->image); \
    _layer->image = gdk_pixbuf_get_from_surface(_layer->surface, 0, 0, \
                    _layer->size.w, _layer->size.h); \
    layer_icon_refresh(_layer);

// debug print


#endif /* DEBUG_H_ */
