### How to use in an openFrameworks app
 
#### testApp.h
```c++
#include "ofxRingBufferTexture.h"

class testApp
: public ofBaseApp
{
public:
  void setup();
  void draw();
  void audioIn(float* input, int bufferSize, int nChannels);

  ofxRingBufferTexture spectrogram;
};
``` 

#### testApp.cpp
```c++
//--------------------------------------------------------------
void
testApp::audioIn(float* input, int bufferSize, int nChannels)
{
  for (size_t i=0; i<bufferSize; ++i)
    audioChannel.inputBuffer[i] = input[i];

  audioChannel.process();
  spectrogram.addFrame(audioChannel.spectrum);
}

//--------------------------------------------------------------
void
testApp::draw()
{
  spectrogram.uploadToTexture();
  spectrogram.getTextureReference().draw(windowRect);
}
```
