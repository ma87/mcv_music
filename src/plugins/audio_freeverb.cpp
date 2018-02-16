#include "audio_freeverb.h"

FreeverbEffect::FreeverbEffect(int nchannels_input, int nchannels_output, float sampling_rate)
 : AudioEffect(nchannels_input, nchannels_output), m_model()
{
  m_model.setSampleRate(sampling_rate);
  m_model.setMuteOnChange(true);
  m_model.printconfig();
}


FreeverbEffect::~FreeverbEffect()
{

}

void FreeverbEffect::setreverbtype(int value)
{
  std::cout << "set reverb type " << value << std::endl;
  m_model.setReverbType(value);
}

void FreeverbEffect::setroomsize(float value)
{
  m_model.setroomsize(value);
}

void FreeverbEffect::setdamp(float value)
{
  m_model.setdamp(value);
}

void FreeverbEffect::setwet(float value)
{
  m_model.setwet(value);
}

void FreeverbEffect::setdry(float value)
{
  m_model.setdry(value);
}

void FreeverbEffect::setwidth(float value)
{
  m_model.setwidth(value);
  m_model.printconfig();
}

void FreeverbEffect::setProperties(AudioProperties * audioProperties)
{
  using std::placeholders::_1;

  audioProperties->addIntProperty(std::string("REVERB_ENABLED"), std::bind( &AudioEffect::setEnabled, this, _1 ));
  audioProperties->setIntProperty(std::string("REVERB_ENABLED"), 1);

  audioProperties->addIntProperty(std::string("REVERB_TYPE"), std::bind( &FreeverbEffect::setreverbtype, this, _1 ));
  audioProperties->setIntProperty(std::string("REVERB_TYPE"), 0);

  audioProperties->addFloatProperty(std::string("ROOM_SIZE"), std::bind( &FreeverbEffect::setroomsize, this, _1 ));
  float value = m_model.getroomsize();
  audioProperties->setFloatProperty(std::string("ROOM_SIZE"), value);

  audioProperties->addFloatProperty(std::string("DAMP"), std::bind( &FreeverbEffect::setdamp, this, _1 ));
  value = m_model.getdamp();
  audioProperties->setFloatProperty(std::string("DAMP"), value);

  audioProperties->addFloatProperty(std::string("WET"), std::bind( &FreeverbEffect::setwet, this, _1 ));
  value = m_model.getwet();
  audioProperties->setFloatProperty(std::string("WET"), value);

  audioProperties->addFloatProperty(std::string("DRY"), std::bind( &FreeverbEffect::setdry, this, _1 ));
  value = m_model.getdry();
  audioProperties->setFloatProperty(std::string("DRY"), value);

  audioProperties->addFloatProperty(std::string("WIDTH"), std::bind( &FreeverbEffect::setwidth, this, _1 ));
  value = m_model.getwidth();
  audioProperties->setFloatProperty(std::string("WIDTH"), value);

  audioProperties->addFloatProperty(std::string("REVERB_VOLUME"), std::bind( &AudioEffect::setVolume, this, _1 ));
  value = getVolume();
  audioProperties->setFloatProperty(std::string("REVERB_VOLUME"), value);
}

int FreeverbEffect::process(float ** input, float ** output, int nsamples)
{
  if (m_enabled)
  {
    m_model.processreplace(input[0],input[1],output[0],output[1],nsamples);
  }
  else
  {
    memcpy(output[0], input[0], nsamples * sizeof(float));
    memcpy(output[1], input[1], nsamples * sizeof(float));
  }
}
