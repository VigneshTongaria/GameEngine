#include "ShadowRenderTarget.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>

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
void ShadowRenderTarget::resize(int newWidth,int newHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER,m_dm_fbo);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glBindTexture(GL_TEXTURE_2D, m_dm);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,newWidth,newWidth,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,m_dm,0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}