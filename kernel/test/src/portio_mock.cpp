#include "portio_mock.h"
#include <map>
#include <queue>

extern "C" {

/**
 * data that the kernel sends out to the tests
 */
static std::map<uint16_t, std::queue<uint32_t>> portDataOutMap;

/**
 * data that tests send into the kernel
 */
static std::map<uint16_t, std::queue<uint32_t>> portDataInMap;

void putPortData(uint16_t port, uint32_t data) {
    portDataInMap[port].push(data);
}

uint32_t getPortData(uint16_t port) {
    uint16_t result = portDataOutMap[port].front();
    portDataOutMap[port].pop();

    return result;
}

void drainPort(uint16_t port) {
    while(!portDataOutMap[port].empty()) {
        portDataOutMap[port].pop();
    }
}

void kernelPutPortData(uint16_t port, uint32_t data) {
    portDataOutMap[port].push(data);
}

uint32_t kernelGetPortData(uint16_t port) {
    uint16_t result = portDataInMap[port].front();
    portDataInMap[port].pop();

    return result;
}

uint8_t inportByte(uint16_t port) {
    return kernelGetPortData(port);
}

void outportByte(uint16_t port, uint8_t data) {
    kernelPutPortData(port, data);
}

uint16_t inportShort(uint16_t port) {
    return kernelGetPortData(port);
}

void outportShort(uint16_t port, uint16_t data) {
    kernelPutPortData(port, data);
}

uint32_t inportInt(uint16_t port) {
    return kernelGetPortData(port);
}

void outportInt(uint16_t port, uint32_t data) {
    kernelPutPortData(port, data);
}

}
