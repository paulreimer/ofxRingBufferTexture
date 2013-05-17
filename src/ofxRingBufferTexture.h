/*
 * Copyright Paul Reimer, 2013
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial 3.0 Unported License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/3.0/
 * or send a letter to
 * Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#pragma once

#include "ofTexture.h"
#include "ofFbo.h"
#include <deque>

class ofxRingBufferTexture
{
public:
  ofxRingBufferTexture();
  ~ofxRingBufferTexture();
  
  void setup(const size_t _width, const size_t _numFrames);
  void uploadToTexture();
  void addFrame(const std::vector<float>& frame);
  
  ofTexture& getTextureReference();

  ofTexture texture;
  ofFbo framebuffer;

  float currentFrameOffset;
  size_t framesProcessed;
  std::deque<std::vector<float> > queuedFrames;
  
  size_t width;
  size_t numFrames;
};
