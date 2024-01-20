
#include "buffer.h"

FileBuffer* stdin = 0;
FileBuffer* stdout = 0;

void initializeBuffers(FileBuffer* in, FileBuffer* out) {
    stdin = in;
    stdout = out;
}
