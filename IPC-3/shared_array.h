#pragma once

#include <cstddef>

class shared_array {
private:
    int* buffer;
    size_t length;
    const char* memory_id;

public:
    shared_array(const shared_array&) = delete;
    shared_array& operator=(const shared_array&) = delete;

    shared_array(shared_array&& other) noexcept;

    shared_array(const char* name, size_t count);

    ~shared_array();

    int& operator[](size_t index);

    void destroy();

    size_t size() const;
};
