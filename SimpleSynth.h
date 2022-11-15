#include "MidiIn.h"
#include <stack>

class SimpleSynth : private MidiIn
{
public:
  SimpleSynth(int devno, int R);
  ~SimpleSynth(void);
  float getValue(void);
  void incrementTime(void);

  virtual void onNoteOn(int channel, int note, int velocity);
  virtual void onNoteOff(int channel, int note);
  virtual void onPitchWheelChange(int channel, float value);

private:
  int samplingRate;
  float multiplier;
  float index;
  float increment;

  float sawtooth(float t);
  std::stack<float> notes;
  float pitchWheelChange;
  float noteVelocity;
  float pitchMultiplier;
};
