#ifndef __PROPERTY__H__
#define __PROPERTY__H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _SVID_SOURCE
#define _SVID_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h> 
#include <pthread.h> 

#define BUFFER_KEY  78910
#define BUFFER_SIZE 1000

typedef enum {
    VOID,
    INT,
    STR,
    FLOAT    
} PROPERTY_TYPE;

typedef struct {
    PROPERTY_TYPE type;
    
    int key_size;
    char * key;
    
    int value_size;
    void * value;
} Property;

typedef void * Properties;

Properties init_properties();
void add_property(Properties * properties, Property * property);

int get_size(Property * property);
Property * create_property(char * key, PROPERTY_TYPE type, int value_size, void * value);
int look_property(Properties * properties, char * wanted_key, int * p_value_size);

void get_property_value(Properties * properties, char * key, void * value);
void set_property_value(Properties * properties, char * key, void * value);

void free_property(Property * property);

#ifdef __cplusplus
}
#endif

#endif
