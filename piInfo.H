#ifndef __PI_INFO_H
#define __PI_INFO_H

template <typename T>
consteval T GPIO_LAYOUT()
{
    return static_cast<T>(2);
}

template <typename T>
consteval T PI_MODEL()
{
    return static_cast<T>(17);
}

template <typename T>
consteval T PI_REV()
{
    return static_cast<T>(5);
}

template <typename T>
consteval T PI_MEM()
{
    return static_cast<T>(5);
}

template <typename T>
consteval T PI_MAKER()
{
    return static_cast<T>(0);
}

template <typename T>
consteval T PI_WARRANTY()
{
    return static_cast<T>(0);
}

#endif
