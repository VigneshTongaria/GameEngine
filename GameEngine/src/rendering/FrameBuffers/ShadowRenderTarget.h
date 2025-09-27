#pragma once
#include "IRenderTarget.h"

class ShadowRenderTarget : public IRenderTarget
{
private:
    unsigned int m_dm_fbo;
    unsigned int m_dm;
public:
    ShadowRenderTarget(const int width,const int height);

    void init() override;
    void bindTexture() const override;
    void unbindTexture() const override;
    void bindFrameBuffer() const override;
    void unbindFrameBuffer() const override;
    void resize(const int newWidth,const int newHeight) override;

    GLuint getFramebufferID() const override;

    ~ShadowRenderTarget();
};
