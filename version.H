#ifndef __WIRING_PI_VERSION_H
#define __WIRING_PI_VERSION_H

[[nodiscard]] consteval const std::string_view VERSION()
{
    return "2.70";
}

// template <typename T>
// consteval T VERSION_MAJOR()
[[nodiscard]] consteval int VERSION_MAJOR()
{
    return 2;
}

// template <typename T>
// consteval T VERSION_MINOR()
[[nodiscard]] consteval int VERSION_MINOR()
{
    return 70;
}

#endif