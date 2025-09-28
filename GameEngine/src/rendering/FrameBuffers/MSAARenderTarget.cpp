#include "MSAARenderTarget.h"
#include <iostream>

MSAARenderTarget::MSAARenderTarget(int width,int height,int samples)
{
    init();
    resize(width,height,samples);
}

void MSAARenderTarget::init()
{
    glGenFramebuffers(1, &m_msaa_fbo);
    glGenTextures(1, &m_msaa_m);
    glGenRenderbuffers(1,&m_mssa_rbo);
}
void MSAARenderTarget::bindFrameBuffer() const
{
    glBindFramebuffer(GL_FRAMEBUFFER,m_msaa_fbo);
}
void MSAARenderTarget::unbindFrameBuffer() const
{
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}
void MSAARenderTarget::bindTexture() const
{
    glBindFramebuffer(GL_TEXTURE_2D,m_msaa_m);
}
void MSAARenderTarget::unbindTexture() const
{
    glBindTexture(GL_TEXTURE_2D,0);
}
void MSAARenderTarget::resize(int newWidth,int newHeight,int samples)
{   
    glBindFramebuffer(GL_FRAMEBUFFER,m_msaa_fbo);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msaa_m);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_SRGB,newWidth, newHeight, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,m_msaa_m,0);

	glGenRenderbuffers(1,&m_mssa_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,m_mssa_rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER,4,GL_DEPTH24_STENCIL8,newWidth, newHeight);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,m_mssa_rbo);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	  std::cout<<"Frame buffers not generated"<<std::endl;
	
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
}
void MSAARenderTarget::resize(int newWidth,int newHeight)
{   
    glBindFramebuffer(GL_FRAMEBUFFER,m_msaa_fbo);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msaa_m);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB,newWidth, newHeight, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,m_msaa_m,0);

    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
}

unsigned int MSAARenderTarget::getFramebufferID() const
{
    return m_msaa_fbo;
}