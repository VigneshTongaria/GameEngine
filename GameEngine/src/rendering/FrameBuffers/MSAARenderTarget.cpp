#include "MSAARenderTarget.h"
#include <iostream>
#include "../../core/OpenGLHeaders.h"

MSAARenderTarget::MSAARenderTarget(int width,int height,int samples)
{
    init();
    resize(width,height,samples);
}

void MSAARenderTarget::init()
{
    glGenFramebuffers(1, &m_msaa_fbo);
#ifdef __EMSCRIPTEN__
    glGenRenderbuffers(1, &m_msaa_m);
#else
    glGenTextures(1, &m_msaa_m);
#endif
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
void MSAARenderTarget::resize(int newWidth, int newHeight, int samples)
{
    if (newWidth <= 0 || newHeight <= 0 || samples <= 0)
        return;

    GLint maxSamples = 0;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
    samples = std::min(samples, maxSamples);

    glBindFramebuffer(GL_FRAMEBUFFER, m_msaa_fbo);

#ifdef __EMSCRIPTEN__

    glBindRenderbuffer(GL_RENDERBUFFER, m_msaa_m);

    glRenderbufferStorageMultisample(
        GL_RENDERBUFFER,
        samples,
        GL_RGBA8,
        newWidth,
        newHeight
    );

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_RENDERBUFFER,
        m_msaa_m
    );

#else

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msaa_m);

    glTexImage2DMultisample(
        GL_TEXTURE_2D_MULTISAMPLE,
        samples,
        GL_RGBA8,
        newWidth,
        newHeight,
        GL_TRUE
    );

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D_MULTISAMPLE,
        m_msaa_m,
        0
    );

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

#endif

    // Depth/stencil attachment
    glBindRenderbuffer(GL_RENDERBUFFER, m_mssa_rbo);

    glRenderbufferStorageMultisample(
        GL_RENDERBUFFER,
        samples,
        GL_DEPTH24_STENCIL8,
        newWidth,
        newHeight
    );

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        m_mssa_rbo
    );

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "MSAA framebuffer incomplete: 0x"
            << std::hex << status << std::dec << std::endl;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void MSAARenderTarget::resize(int newWidth,int newHeight)
{   
    resize(newWidth, newHeight, 4);
}

unsigned int MSAARenderTarget::getFramebufferID() const
{
    return m_msaa_fbo;
}