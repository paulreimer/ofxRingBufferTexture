/*
 * Copyright Paul Reimer, 2013
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial 3.0 Unported License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/3.0/
 * or send a letter to
 * Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include "ofxRingBufferTexture.h"

//--------------------------------------------------------------
ofxRingBufferTexture::ofxRingBufferTexture()
: currentFrameOffset(0)
, framesProcessed(0)
, width(0)
, numFrames(0)
{}

//--------------------------------------------------------------
ofxRingBufferTexture::~ofxRingBufferTexture()
{}

//--------------------------------------------------------------
void
ofxRingBufferTexture::setup(const size_t _width, const size_t _numFrames)
{
  width = _width;
  numFrames = _numFrames;

  ofFbo::Settings settings;
  settings.width          = width;
  settings.height         = numFrames;
	settings.useDepth       = false;				// whether to use depth buffer or not
	settings.useStencil     = false;				// whether to use stencil buffer or not
	settings.textureTarget  = GL_TEXTURE_RECTANGLE_ARB;
//	settings.internalformat = GL_LUMINANCE32F_ARB;			// GL_RGBA, GL_RGBA16F_ARB, GL_RGBA32F_ARB, GL_LUMINANCE32F_ARB etc.
  framebuffer.allocate(settings);
//  framebuffer.allocate(width, numFrames, GL_LUMINANCE);

  texture.allocate(width, numFrames, GL_LUMINANCE32F_ARB, true);
  texture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
}

//--------------------------------------------------------------
void
ofxRingBufferTexture::addFrame(const std::vector<float>& frame)
{
  queuedFrames.push_back(frame);
}

//--------------------------------------------------------------
void
ofxRingBufferTexture::uploadToTexture()
{
  if (!queuedFrames.empty())
  {
    texture.bind();

    size_t firstFrameOffset;
    while (!queuedFrames.empty())
    {
      const std::vector<float>& processFrame = queuedFrames.front();
      {
        // write to texture stage 1
        glTexSubImage2D(texture.getTextureData().textureTarget, 0,
                        0, currentFrameOffset,
                        processFrame.size(), 1,
                        GL_LUMINANCE, GL_FLOAT,
                        processFrame.data());
      }
      queuedFrames.pop_front();
      currentFrameOffset++;
      framesProcessed++;

      if (currentFrameOffset >= numFrames)
        currentFrameOffset = 0;

      firstFrameOffset = currentFrameOffset + 1;

      if (framesProcessed < numFrames)
        firstFrameOffset = 0;

      if (firstFrameOffset >= numFrames)
        firstFrameOffset = 0;
    }
    texture.unbind();
    
    // write to fbo stage 2
    // draw 2 rectangle sections from current:max (older) then from 0:current (newer)
    framebuffer.begin();
    {
      float w = width;
      float totalH = MIN(framesProcessed, numFrames);
      float olderH = totalH - firstFrameOffset;
      
      // older data section
      texture.drawSubsection(0, 0, w, olderH,
                             0, firstFrameOffset, w, olderH);
      
      // newer data section
      texture.drawSubsection(0, olderH, w, firstFrameOffset,
                             0, 0, w, firstFrameOffset);
    }
    framebuffer.end();
  }
}

//--------------------------------------------------------------
ofTexture&
ofxRingBufferTexture::getTextureReference()
{
  return framebuffer.getTextureReference();
}
