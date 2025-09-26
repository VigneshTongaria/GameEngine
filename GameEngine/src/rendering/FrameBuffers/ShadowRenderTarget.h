#pragma once
#include "IRenderTarget.h"

class ShadowRenderTarget : public IRenderTarget
{
private:
    unsigned int m_dm_fbo;
    unsigned int m_dm;
public:
    ShadowRenderTarget(int width, int height);

    void init() override;
    void bind() override;
    void unbind() override;
    void resize(int newWidth, int newHeight) override;

    GLuint getFramebufferID() const override;

    ~ShadowRenderTarget();
};
