#include <stddef.h>

#include "syscall.h"

void uart_write(char* s, size_t length) {
    syscall(0, (uint64_t) s, length, 0, 0, 0, 0);
}

// alloc_page(void* addr, size_t count, int permissions) -> void* addr
void* alloc_page(void* addr, size_t count, int permissions) {
    return (void*) syscall(1, (uint64_t) addr, count, permissions, 0, 0, 0).first;
}

int page_permissions(void* addr, size_t count, int permissions) {
    return syscall(2, (uint64_t) addr, count, permissions, 0, 0, 0).first;
}

int dealloc_page(void* addr, size_t count) {
    return syscall(3, (uint64_t) addr, count, 0, 0, 0, 0).first;
}

typedef struct {
    uint64_t seconds;
    uint64_t nanos;
} time_t;

time_t sleep(uint64_t seconds, uint64_t nanos) {
    dual_t dual = syscall(7, seconds, nanos, 0, 0, 0, 0);
    return (time_t) {
        .seconds = dual.first,
        .nanos = dual.second,
    };
}

void _start() {
    uart_write("initd started\n", 14);
    volatile char* p = alloc_page(NULL, 2, 0x06);
    uart_write("allocated page\n", 15);
    *p = 0;
    uart_write("successfully wrote\n", 19);
    if (!page_permissions((void*) p, 2, 0x04)) {
        uart_write("successfully changed permissions\n", 33);
    } else {
        uart_write("oh no\n", 6);
    }

    dealloc_page((void*) p, 2);
    while (1) {
        uart_write("a\n", 2);
        sleep(1, 0);
    }
}
