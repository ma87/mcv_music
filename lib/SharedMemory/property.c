#include "property.h"

Properties init_properties()
{
  Properties *properties;
  
  int shmid;
  key_t key = BUFFER_KEY;
  
  // Setup shared memory
  if ((shmid = shmget(key, BUFFER_SIZE, IPC_CREAT | 0666)) < 0)
  {
     printf("Error getting properties shared memory id\n");
     return NULL;
  }
  // Attached shared memory
  if (*properties = (Properties)shmat(shmid, NULL, 0))
  {
     printf("Error attaching properties shared memory id\n");
     return NULL;
  }
  
  int size_int = sizeof(int);
  memcpy((char *)*properties, &size_int, sizeof(int));
  return *properties;
}

int get_size(Property * property)
{
  return (int)(sizeof(int) + sizeof(PROPERTY_TYPE) + 2 * sizeof(int) + property->key_size + property->value_size);
}

void add_property(Properties * properties, Property * property)
{
  int value_size;
  if (look_property(properties, property->key, &value_size) > 0)
  {
    //printf("property %s already exists\n", property->key);
    return;
  }
  // Where is the end ?
  int next;
  memcpy(&next, (char *)properties, sizeof(int));
  
  //printf("end = %d when adding property %s\n", next, property->key);
  if (next + get_size(property) < BUFFER_SIZE)
  {
    memcpy((char *)properties + next, &property->type, sizeof(PROPERTY_TYPE));
    next += sizeof(PROPERTY_TYPE);
    
    memcpy((char *)properties + next, &property->key_size, sizeof(int));
    next += sizeof(int);
    
    memcpy((char *)properties + next, property->key, property->key_size * sizeof(char));
    next += (int)property->key_size;
    
    memcpy((char *)properties + next, &property->value_size, sizeof(int));
    next += sizeof(int);
    
    memcpy((char *)properties + next, property->value, property->value_size);
    next += (int)property->value_size;
    
    // Update end
    memcpy((char *)properties, &next, sizeof(int));
  }
  else
  {
    printf("Can't add new property, BUFFER is FULL !!\n");
  }
  
}

int look_property(Properties * properties, char * wanted_key, int * p_value_size)
{
  void * pbuffer = properties;
  int count = 0;
  
  int key_size;
  int value_size;
  PROPERTY_TYPE type;
  
  char * key;
  
  count = sizeof(int);
  
  while (count < BUFFER_SIZE)
  {
    memcpy(&type, (char *)pbuffer + count, sizeof(PROPERTY_TYPE));
    count += sizeof(PROPERTY_TYPE);
    
    memcpy(&key_size, (char *)pbuffer + count, sizeof(int));
    count += sizeof(int);
    
    key = (char *)malloc(key_size);
    memcpy(key, pbuffer + count, key_size);
    count += key_size;
    
    memcpy(&value_size, (char *)pbuffer + count, sizeof(int));
    count += sizeof(int);
    
    if (!strcmp(wanted_key, key))
    {
      *p_value_size = value_size;
      return count;
    }
    else
    {
      count += value_size;
    }
  }
  
  return -1;
}

void get_property_value(Properties * properties, char * wanted_key, void * value)
{
  int count = 0;
  int value_size;
    
  count = look_property(properties, wanted_key, &value_size);
  if (count > 0)
  {
    memcpy(value, (char *)properties + count, value_size);
    return;
  }
  
  printf("in get_property_value no key found\n");
}

void set_property_value(Properties * properties, char * wanted_key, void * value)
{
  int count;
  int value_size;
  
  count = look_property(properties, wanted_key, &value_size);
  if (count > 0)
  {
    memcpy((char *)properties + count, value, value_size);
    return;
  }
}

Property * create_property(char * key, PROPERTY_TYPE type, int value_size, void * value)
{
  Property * property = (Property *)malloc(sizeof(Property));
  property->type = type;
      
  property->key_size = (strlen(key) + 1) * sizeof(char);
  property->key = (char *)malloc(property->key_size);
  
  strcpy(property->key, key);
    
  property->value_size = value_size;
  property->value = malloc(value_size);
  memcpy(property->value, value, value_size);
  
  return property;
}

void free_property(Property * property)
{
  free(property->key);
  free(property->value);
  free(property);
}


