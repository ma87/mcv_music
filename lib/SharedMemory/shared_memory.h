#ifndef __SHARED__MEMORY__H__
#define __SHARED__MEMORY__H__

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include "property.h"


#define ERR_VALUE_INT -200
#define ERR_VALUE_FLOAT -200.0f

enum {
  PAUSE = 0,
  PLAY,
  STOP
};

typedef void * ISharedMemory;

typedef struct {
    int id;
    pthread_mutex_t mutex;
    pthread_mutex_t mutex_control_changed;
    pthread_cond_t cond;
    Properties properties;
} SharedMemory;

//////////////////////////////////////////////////////////////////////////////////////////
// SHARED MEMORY
ISharedMemory create_shared_memory(int shared_key);
ISharedMemory get_shared_memory(int shared_key, int * already_exists);
void free_shared_memory(ISharedMemory i_shared_memory);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// PROPERTIES
void add_shared_property(ISharedMemory i_shared_memory, char * key, int type);

int get_value(ISharedMemory i_shared_memory);
int get_int_shared_property(ISharedMemory i_shared_memory, char * key);
int get_non_blocking_int_shared_property(ISharedMemory i_shared_memory, char * key);
void set_int_shared_property(ISharedMemory i_shared_memory, char * key, int value, int is_wake_up);

float get_float_shared_property(ISharedMemory i_shared_memory, char * key);
float get_non_blocking_float_shared_property(ISharedMemory i_shared_memory, char * key);
void set_float_shared_property(ISharedMemory i_shared_memory, char * key, float value, int is_wake_up);
/////////////////////

///////////////////
// AUDIO CONTROLLER
pthread_mutex_t * get_audio_controller_mutex(ISharedMemory i_shared_memory);
pthread_cond_t * get_audio_controller_cond(ISharedMemory i_shared_memory);
///////////////////

#ifdef __cplusplus
}
#endif

#endif
