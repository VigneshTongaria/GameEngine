#pragma once
#include <glad/glad.h>

class IRenderTarget {
public:
    virtual ~IRenderTarget() = default;

    // Must be called once to allocate GPU resources
    virtual void init() = 0;

    // Bind this render target for rendering
    virtual void bind() = 0;

    // Unbind (usually bind default framebuffer 0)
    virtual void unbind() = 0;

    // Resize GPU resources when viewport changes
    virtual void resize(int newWidth, int newHeight) = 0;

    // Get the underlying framebuffer ID
    virtual GLuint getFramebufferID() const = 0;
};