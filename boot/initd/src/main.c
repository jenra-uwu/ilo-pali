#include <stddef.h>

#include "syscalls.h"
#include "fat16.h"

void thread(void* data, size_t size) {
    while (1) {
        uart_write(data, size);
        sleep(1, 0);
    }
}

void _start(void* initrd, size_t size) {
    uart_write("initd started\n", 14);

    fat16_fs_t fat = verify_initrd(initrd, initrd + size);
    if (fat.data == NULL) {
        uart_write("initrd is wrong :(\n", 19);
        while(1);
    }

    /*
    size_t test_size;
    void* test = read_file_full(&fat, "test", &test_size);

    uint64_t pid = spawn_process(test, test_size, "uwu?\n", 5);

    sleep(1, 0);
    uint8_t waiting = 0;
    send(true, pid, 2, (uint64_t) &waiting, 5);
    lock(&waiting, 1, 4);
    */

    uint64_t pid = spawn_thread(thread, "nya\n", 4);
    sleep(1, 0);
    kill(pid);

    while (1) {
        uart_write("a\n", 2);
        sleep(1, 0);
    }
}

