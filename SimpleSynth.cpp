/**************************************************************/
/*
  Joey Laybourn
  Assignment 6: SimpleSynth
  CS245
  Spring 2019
*/
/**************************************************************/

#include "SimpleSynth.h"
#include <iostream>

SimpleSynth::SimpleSynth(int devno, int R) :
  MidiIn(devno),
  samplingRate(R),
  index(0),
  increment(440.0f / float(R)),
  multiplier(0),
  pitchWheelChange(0)
{
  start();
}

SimpleSynth::~SimpleSynth(void)
{
  stop();
}

float SimpleSynth::getValue(void)
{
  /*if (findex >= audio_data->frames() - 1)
  {
    return 0;
  }*/

  unsigned flooredValue = static_cast<unsigned>(index);
  /*if (isLooping && (flooredValue > iloop_end))
  {
    flooredValue = iloop_end;
  }*/
  float temp1 = sawtooth(index);
  //std::cout << "Floored Value: " << flooredValue << std::endl;

  // + 1 won't fly for looped values
  /*float temp2;
  if (isLooping && (findex >= iloop_end))
  {
    temp2 = audio_data->sample(iloop_bgn, ichannel);
  }
  else
  {
    temp2 = audio_data->sample(flooredValue + 1, ichannel);
  }*/

  float temp2 = sawtooth(index + 1);

  float tempMultiplier = static_cast<float>(index) - static_cast<float>(flooredValue);

  return temp1 + (tempMultiplier * (temp2 - temp1));
}

void SimpleSynth::incrementTime(void)
{
  index += increment * multiplier;

  if (index > 1)
  {
    index = 0;
  }

  /*if (isLooping && (findex > iloop_end))
  {
    double offset = findex - static_cast<double>(iloop_end);
    findex = static_cast<double>(iloop_bgn) + offset;
  }*/
}

void SimpleSynth::onNoteOn(int channel, int note, int velocity)
{
  notes.push(float(note));
  multiplier = 16.0f * (pow(2.0f, (notes.top() + pitchMultiplier) / 12.0f) / 440.0f);
  noteVelocity = float(velocity) / 127.0f;

  //std::cout << "PITCH MULTIPLIER: " << pitchMultiplier << std::endl;
}

void SimpleSynth::onNoteOff(int channel, int note)
{
  //std::cout << "NOTE OFF" << std::endl;
  notes.pop();
  if (notes.empty())
  {
    multiplier = 0;
  }
  else
  {
    multiplier = 16 * (pow(2.0f, notes.top() / 12.0f) / 440.0f);
  }
}

void SimpleSynth::onPitchWheelChange(int channel, float value)
{
  pitchMultiplier = (2.0f * value);

  if (!notes.empty())
  {
    multiplier = 16 * (pow(2.0f, (pitchMultiplier + notes.top()) / 12.0f) / 440.0f);
  }
}

float SimpleSynth::sawtooth(float t)
{
  t -= std::floor(t);
  return (2 * t - 1) * noteVelocity; 
}
