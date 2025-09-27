#include "MSAARenderTarget.h"


MSAARenderTarget::MSAARenderTarget(int width,int height,int samples)
{
    init();
    resize(width,height,samples);
}

void MSAARenderTarget::init()
{
    glGenFramebuffers(1, &m_msaa_fbo);
    glGenTextures(1, &m_msaa_m);
}
void MSAARenderTarget::bindFrameBuffer() const
{
    glBindFramebuffer(GL_FRAMEBUFFER,m_msaa_fbo);
}
void MSAARenderTarget::unbindFrameBuffer() const
{
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}
void MSAARenderTarget::resize(int newWidth,int newHeight,int samples)
{   
    glBindFramebuffer(GL_FRAMEBUFFER,m_msaa_fbo);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msaa_m);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB,newWidth, newHeight, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,m_msaa_m,0);

    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
}