#include "shared_array.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

shared_array::shared_array(shared_array&& other) noexcept
    : buffer(other.buffer),
      length(other.length),
      memory_id(other.memory_id) {
    other.buffer = nullptr;
    other.length = 0;
    other.memory_id = nullptr;
}

shared_array::shared_array(const char* name, size_t count)
    : buffer(nullptr), length(count), memory_id(name) {
    if (count == 0 || count > 1'000'000'000)
        throw std::invalid_argument("Invalid array size");

    int shm_fd = shm_open(memory_id, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0)
        throw std::runtime_error("Cannot open shaed memory");

    struct stat info;
    fstat(shm_fd, &info);
    if (info.st_size != 0 && (size_t)info.st_size != length * sizeof(int)) {
        close(shm_fd);
        throw std::runtime_error("Shared memory size mismatch");
    }

    if (ftruncate(shm_fd, length * sizeof(int)) != 0) {
        close(shm_fd);
        throw std::runtime_error("Failed to resize shared memory");
    }

    buffer = static_cast<int*>(
        mmap(nullptr, length * sizeof(int),
             PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)
    );

    if (buffer == MAP_FAILED) {
        close(shm_fd);
        throw std::runtime_error("mmap failed");
    }

    close(shm_fd);
}

shared_array::~shared_array() {
    if (buffer)
        munmap(buffer, length * sizeof(int));
}

int& shared_array::operator[](size_t index) {
    if (!buffer || index >= length)
        throw std::out_of_range("Index out of range");
    return buffer[index];
}

void shared_array::destroy() {
    shm_unlink(memory_id);
}

size_t shared_array::size() const { return length;}
