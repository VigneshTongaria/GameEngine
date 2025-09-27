#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

class IRenderTarget {
public:
    virtual ~IRenderTarget() = default;

    // Must be called once to allocate GPU resources
    virtual void init() = 0;

    // Bind this render target for rendering
    virtual void bindTexture() const = 0;

    // Unbind (usually bind default framebuffer 0)
    virtual void unbindTexture() const = 0;

    // Bind this render target for rendering
    virtual void bindFrameBuffer() const = 0;

    // Unbind (usually bind default framebuffer 0)
    virtual void unbindFrameBuffer() const = 0;

    // Resize GPU resources when viewport changes
    virtual void resize(int newWidth, int newHeight) = 0;

    // Get the underlying framebuffer ID
    virtual GLuint getFramebufferID() const = 0;
};