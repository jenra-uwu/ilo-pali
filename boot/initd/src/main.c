#include "fat16.h"
#include "syscalls.h"

bool streq(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b)
            return false;
        a++;
        b++;
    }

    return *a == *b;
}

int _start() {
    struct allowed_memory allowed;
    uart_puts("initd started");
    for (size_t i = 0; get_allowed_memory(i, &allowed); i++) {
        if (streq(allowed.name, "initrd")) {
            break;
        }
    }

    void* fat_raw = map_physical_memory(allowed.start, allowed.size, PAGE_PERM_READ | PAGE_PERM_WRITE);
    fat16_fs_t fat = verify_initrd(fat_raw, fat_raw + allowed.size);
    size_t size;
    void* data = read_file_full(&fat, "uwu", &size);
    uart_puts("read file /uwu:");
    uart_puts((char*) data);
    while(1);
}

