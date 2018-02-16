#ifndef __MCV_AUDIO_PROPERTIES_H__
#define __MCV_AUDIO_PROPERTIES_H__

#include <map>
#include <vector>
#include <thread>
#include <atomic>
#include <string>
#include <functional>
#include "shared_memory.h"

/*
typedef void (*CallBackIntProperty)(int value);
typedef void (*CallBackFloatProperty)(float value);
*/

typedef std::function<void(int)> CallBackIntProperty;
typedef std::function<void(float)> CallBackFloatProperty;

class AudioProperties
{
  public:
    AudioProperties();
    virtual ~AudioProperties();
    void addIntProperty(std::string key, CallBackIntProperty callback);
    void addFloatProperty(std::string key, CallBackFloatProperty callback);
    
    void setIntProperty(std::string key, int value);
    void setFloatProperty(std::string key, float value);
    
    bool needUpdate();
    void update();
    
  private:
    void run();
  
    std::map<std::string, std::vector<CallBackFloatProperty>> m_floatProperties;
    std::map<std::string, std::vector<CallBackIntProperty>> m_intProperties;
    
    std::thread m_thread;
    ISharedMemory m_sharedMemory;
    bool m_isRunning;
    std::atomic<bool> m_update;
};


#endif
