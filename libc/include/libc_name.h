#ifndef KERNEL_LIBC_NAME_H
#define KERNEL_LIBC_NAME_H

// Tests need a way to add a prefix to the name of methods
// to distinguish from system libc.

#ifdef TESTING
#define LIBC_METHOD_IMPL(name) os_libc_ ## name
#else
#define LIBC_METHOD_IMPL(name) name
#endif

#define LIBC_SYMBOL(name) LIBC_METHOD_IMPL(name)

#endif //KERNEL_LIBC_NAME_H
