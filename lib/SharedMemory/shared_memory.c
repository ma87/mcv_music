#include "shared_memory.h"

ISharedMemory create_shared_memory(int shared_key)
{
   printf("libSharedMemory v0.2.2\n");
   int already_exists = 0;
   SharedMemory * shared_memory = (SharedMemory *)get_shared_memory(shared_key, &already_exists);
   if (!shared_memory)
   {
      printf("error in get shared memory\n");
      return NULL;
   }

   if (already_exists == 1)
   {
      return (ISharedMemory)shared_memory;
   }
   pthread_condattr_t cond_attr;
   pthread_condattr_init(&cond_attr);
   pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);

   pthread_mutexattr_t m_attr;
   pthread_mutexattr_init(&m_attr);
   pthread_mutexattr_setpshared(&m_attr, PTHREAD_PROCESS_SHARED);

   pthread_mutex_init(&shared_memory->mutex, &m_attr);
   pthread_mutex_init(&shared_memory->mutex_control_changed, NULL);
   pthread_cond_init(&shared_memory->cond, &cond_attr);

   pthread_condattr_destroy(&cond_attr);
   pthread_mutexattr_destroy(&m_attr);

   printf("%d\n", (int)sizeof(SharedMemory));
   //shared_memory->properties = init_properties();

   shared_memory->properties = (char *)shared_memory + sizeof(SharedMemory) - 8;
   int next = sizeof(int);
   memcpy((char *)shared_memory->properties, &next, sizeof(int));

   return (ISharedMemory)shared_memory;
}

ISharedMemory get_shared_memory(int shared_key, int * already_exists)
{
   SharedMemory * shared_memory;
   int shmid;
   key_t key = shared_key;
   if ((shmid = shmget(shared_key, sizeof(SharedMemory) + BUFFER_SIZE, 0600)) == -1)
   {
       printf("could not get SHM id, trying to create one now\n");
       if ((shmid = shmget(shared_key, sizeof(SharedMemory) + BUFFER_SIZE, IPC_EXCL | IPC_CREAT | 0600)) == -1)
       {
           printf("could not create or get shared memory segment");
           return NULL;
        }
       else
           printf("created SHM id\n");
   }
   else
   {
       printf("got already existing SHM id\n");
       *already_exists = 1;
   }

   printf("shared memory created\n");
   // Attached shared memory
   if (!(shared_memory = (SharedMemory *)shmat(shmid, NULL, 0)))
   {
      printf("Error attaching shared memory id\n");
      return NULL;
   }

   shared_memory->id = shmid;

   return (ISharedMemory)shared_memory;
}

void free_shared_memory(ISharedMemory i_shared_memory)
{
   SharedMemory * shared_memory = (SharedMemory *)i_shared_memory;

   // shmctl checks if there is still attached nodes before removing shared memory
   shmctl(shared_memory->id, IPC_RMID, NULL);
}

void add_shared_property(ISharedMemory i_shared_memory, char * key, int type)
{
   SharedMemory * shared_memory = (SharedMemory *)i_shared_memory;
   PROPERTY_TYPE property_type = (PROPERTY_TYPE)type;
   size_t size_property;
   void * value;
   switch(property_type)
   {
      case INT:
         value = malloc(sizeof(int));
         int default_value_int = 0;
         memcpy(value, &default_value_int, sizeof(int));
         size_property = sizeof(int);
         break;
      case FLOAT:
         value = malloc(sizeof(float));
         float default_value_float = 0.0f;
         memcpy(value, &default_value_float, sizeof(float));
         size_property = sizeof(float);
         break;
   }
   Property * property = create_property(key, property_type, size_property, &value);

   add_property(&shared_memory->properties, property);
   free_property(property);
}

int get_value(ISharedMemory i_shared_memory)
{
   return get_int_shared_property(i_shared_memory, "TITLE");
}

int get_int_shared_property(ISharedMemory i_shared_memory, char * key)
{
   SharedMemory * shared_memory = (SharedMemory *)i_shared_memory;

   int value;

   pthread_mutex_lock (&shared_memory->mutex);
   get_property_value(&shared_memory->properties, key, &value);
   pthread_mutex_unlock (&shared_memory->mutex);

   return value;
}

int get_non_blocking_int_shared_property(ISharedMemory i_shared_memory, char * key)
{
   SharedMemory * shared_memory = (SharedMemory *)i_shared_memory;

   int value;

   if (pthread_mutex_trylock(&shared_memory->mutex) != 0)
   {
      value = ERR_VALUE_INT;
      printf("blocked int\n");
   }
   else
   {
      get_property_value(&shared_memory->properties, key, &value);
   }
   pthread_mutex_unlock (&shared_memory->mutex);

   return value;
}

void set_int_shared_property(ISharedMemory i_shared_memory, char * key, int value, int wake_up)
{
   SharedMemory * shared_memory = (SharedMemory *)i_shared_memory;
   pthread_mutex_lock (&shared_memory->mutex);
   set_property_value(&shared_memory->properties, key, &value);
   pthread_mutex_unlock (&shared_memory->mutex);

   if (wake_up > 0)
   {
     printf("wake up for key %s\n", key);
      pthread_cond_broadcast(&shared_memory->cond);
   }
}

float get_float_shared_property(ISharedMemory i_shared_memory, char * key)
{
   SharedMemory * shared_memory = (SharedMemory *)i_shared_memory;

   float value;

   pthread_mutex_lock (&shared_memory->mutex);
   get_property_value(&shared_memory->properties, key, &value);
   pthread_mutex_unlock (&shared_memory->mutex);

   return value;
}

float get_non_blocking_float_shared_property(ISharedMemory i_shared_memory, char * key)
{
   SharedMemory * shared_memory = (SharedMemory *)i_shared_memory;

   float value;

   if (pthread_mutex_trylock(&shared_memory->mutex) != 0)
   {
      value = ERR_VALUE_FLOAT;
      printf("blocked float\n");
   }
   else
   {
      get_property_value(&shared_memory->properties, key, &value);
   }
   pthread_mutex_unlock (&shared_memory->mutex);

   return value;
}

void set_float_shared_property(ISharedMemory i_shared_memory, char * key, float value, int is_wake_up)
{
   SharedMemory * shared_memory = (SharedMemory *)i_shared_memory;

   pthread_mutex_lock (&shared_memory->mutex);
   set_property_value(&shared_memory->properties, key, &value);
   pthread_mutex_unlock (&shared_memory->mutex);

   if (is_wake_up > 0)
   {
     printf("wake up for key %s\n", key);
      pthread_cond_broadcast(&shared_memory->cond);
   }
}

pthread_mutex_t * get_audio_controller_mutex(ISharedMemory i_shared_memory)
{
   SharedMemory * shared_memory = (SharedMemory *)i_shared_memory;
   return &shared_memory->mutex_control_changed;
}
pthread_cond_t * get_audio_controller_cond(ISharedMemory i_shared_memory)
{
   SharedMemory * shared_memory = (SharedMemory *)i_shared_memory;
   return &shared_memory->cond;
}
