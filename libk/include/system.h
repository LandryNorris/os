//
// Created by landry on 7/4/23.
//

#ifndef OS_SYSTEM_H
#define OS_SYSTEM_H

// We've set up our kernel to load at 0xC0000000, so any absolute addresses need to
// subtract this value.
#define LOAD_MEMORY_ADDRESS 0xC0000000
#define VGA_ADDRESS 0xC03FF000

#endif //OS_SYSTEM_H
