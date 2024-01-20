
function (mangleSymbol symbol)
    target_compile_definitions(libc_test_lib PRIVATE ${symbol}=os_libc_${symbol})
endfunction()

# mangle symbols shared between this libc and system libc

mangleSymbol(printf)
mangleSymbol(putchar)
mangleSymbol(memmove)
mangleSymbol(memset)
mangleSymbol(memcpy)
mangleSymbol(memcmp)
mangleSymbol(memcpy)
mangleSymbol(strcat)
mangleSymbol(strchr)
mangleSymbol(strcpy)
mangleSymbol(strcmp)
mangleSymbol(strlen)
mangleSymbol(stdout)
mangleSymbol(stdin)
