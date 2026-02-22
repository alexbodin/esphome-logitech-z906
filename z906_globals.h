#ifndef Z906_GLOBALS_H
#define Z906_GLOBALS_H

#include "z906_component.h"

// Global variable to hold the Z906Component instance
Z906Component *z906_device_global = nullptr;

// Inline helper to initialize on first use
template <typename T>
inline void ensure_z906_initialized(T uart_comp)
{
    if (z906_device_global == nullptr)
    {
        z906_device_global = new Z906Component(uart_comp);
        z906_device_global->setup();
    }
}

#endif
