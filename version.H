#ifndef __WIRING_PI_VERSION_H
#define __WIRING_PI_VERSION_H

consteval const char *VERSION()
{
    return "2.70";
}

template <typename T>
consteval T VERSION_MAJOR()
{
    return 2;
}

template <typename T>
consteval T VERSION_MINOR()
{
    return 70;
}

#endif