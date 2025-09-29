#include "ShadowRenderTarget.h"
#include <iostream>

ShadowRenderTarget::ShadowRenderTarget(int width, int height)
{
    init();
    resize(width,height);
}

void ShadowRenderTarget::init()
{
    glGenFramebuffers(1, &m_dm_fbo);
    glGenTextures(1, &m_dm);
}
void ShadowRenderTarget::bindFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER,m_dm_fbo);
}
void ShadowRenderTarget::unbindFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}
void ShadowRenderTarget::bindTexture() const
{
	glBindTexture(GL_TEXTURE_2D,m_dm);
}
void ShadowRenderTarget::unbindTexture() const
{
	glBindTexture(GL_TEXTURE_2D,0);
}
void ShadowRenderTarget::resize(int newWidth,int newHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER,m_dm_fbo);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glBindTexture(GL_TEXTURE_2D, m_dm);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,newWidth,newHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,m_dm,0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Depth framebuffer is not complete!" << std::endl;
}

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
unsigned int ShadowRenderTarget::getFramebufferID() const
{
	return m_dm_fbo;
}