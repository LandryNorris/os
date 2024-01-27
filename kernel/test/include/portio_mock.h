//
// Created by landry on 1/27/24.
//

#ifndef KERNEL_PORTIO_H
#define KERNEL_PORTIO_H

#include <cstdint>

extern "C" {
void putPortData(uint16_t port, uint32_t data);
uint32_t getPortData(uint16_t port);
void drainPort(uint16_t port);
};

#endif //KERNEL_PORTIO_H
