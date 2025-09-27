#pragma once

class ILight
{
private:
    /* data */
public:
    virtual ~ILight() = default;

    float intensity;
    bool renderShadows;
};
