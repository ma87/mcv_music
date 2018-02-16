#include "audio_properties.h"

char * getCharFromString(std::string key)
{
  char * cstr = (char *)malloc((key.length()+1) * sizeof(char));
  strcpy (cstr, key.c_str());
  
  return cstr;
}

AudioProperties::AudioProperties()
: m_floatProperties(), m_intProperties()
{
  m_sharedMemory = create_shared_memory(123456);
  
  m_isRunning = true;
  m_update = false;
  m_thread = std::thread(&AudioProperties::run, this);
  m_thread.detach();
}


AudioProperties::~AudioProperties()
{
  
}

bool AudioProperties::needUpdate()
{
  return m_update;
}

void AudioProperties::run()
{
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_t * cond = get_audio_controller_cond(m_sharedMemory);
  
  pthread_mutex_lock(&mutex);
  while (m_isRunning)
  {
    pthread_cond_wait(cond, &mutex);
    
    printf("properties thread awake update\n");
    m_update = true;
    
    pthread_mutex_unlock(&mutex);
  }
}

void AudioProperties::update()
{
  printf("enter in AudioProperties::update\n");
  char * cstr;
  bool hasBlocked = false;
  for (std::map<std::string,std::vector<CallBackIntProperty>>::iterator it=m_intProperties.begin(); it!=m_intProperties.end(); ++it)
  {
    cstr = getCharFromString(it->first);
    int value = get_non_blocking_int_shared_property(m_sharedMemory, cstr);
    if (value != ERR_VALUE_INT)
    {
      for (int i = 0 ; i < it->second.size() ; i++)
      {
        it->second[i](value);
      }
    }
    else
    {
      hasBlocked = true;
    }
    
    free(cstr);
  }
  
  for (std::map<std::string,std::vector<CallBackFloatProperty>>::iterator it=m_floatProperties.begin(); it!=m_floatProperties.end(); ++it)
  {
    cstr = getCharFromString(it->first);
    float value = get_non_blocking_float_shared_property(m_sharedMemory, cstr);
    if (value != ERR_VALUE_FLOAT)
    {
      for (int i = 0 ; i < it->second.size() ; i++)
      {
        it->second[i](value);
      }
    }
    else
    {
      hasBlocked = true;
    }
    
    free(cstr);
  }
  
  m_update = hasBlocked;
}


void AudioProperties::addIntProperty(std::string key, CallBackIntProperty callback)
{
  char * cstr = getCharFromString(key);
  
  add_shared_property(m_sharedMemory, cstr, INT);
  m_intProperties[key].push_back(callback);
  
  free(cstr);
}

void AudioProperties::addFloatProperty(std::string key, CallBackFloatProperty callback)
{
  char * cstr = getCharFromString(key);
  
  add_shared_property(m_sharedMemory, cstr, FLOAT);
  
  m_floatProperties[key].push_back(callback);
  free(cstr);
}

void AudioProperties::setIntProperty(std::string key, int value_int)
{
  char * cstr = getCharFromString(key);
  set_int_shared_property(m_sharedMemory, cstr, value_int, 0);
  free(cstr);
}

void AudioProperties::setFloatProperty(std::string key, float value_float)
{
  char * cstr = getCharFromString(key);
  set_float_shared_property(m_sharedMemory, cstr, value_float, 0);
  free(cstr);
}
