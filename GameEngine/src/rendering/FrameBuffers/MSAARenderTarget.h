#pragma once
#include "IRenderTarget.h"

class MSAARenderTarget : public IRenderTarget
{
private:
    unsigned int m_msaa_fbo;
    unsigned int m_mssa_rbo;
    unsigned int m_msaa_m;
    unsigned int m_samples;
public:
    MSAARenderTarget(int width,int height,int samples);

    void init() override;
    void bindFrameBuffer() const override;
    void unbindFrameBuffer() const override;
    void bindTexture() const override;
    void unbindTexture() const override;
    void resize(int width,int height) override;
    void resize(int newWidth, int newHeight,int samples);

    unsigned int getFramebufferID() const override;
};
