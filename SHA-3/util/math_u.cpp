#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <stdexcept>


void divMod(uint32_t num, uint32_t divisor, uint32_t *quotient, uint32_t *remainder) {
    assert(divisor != 0); // Ensure divisor is non-zero
    *quotient = num / divisor;
    *remainder = num % divisor;
}


void divMod(uint64_t num, uint64_t divisor, uint64_t *quotient, uint64_t *remainder) {
    assert(divisor != 0); // Ensure divisor is non-zero
    *quotient = num / divisor;
    *remainder = num % divisor;
}


size_t numDigits(uint32_t val, uint32_t base) {
    assert(base > 1); // Ensure base is valid
    size_t ret = 0;
    while (val) {
        ret++;
        val /= base;
    }
    return ret;
}


size_t numDigits(uint64_t val, uint64_t base) {
    assert(base > 1); // Ensure base is valid
    size_t ret = 0;
    while (val) {
        ret++;
        val /= base;
    }
    return ret;
}

template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}


template<typename T>
T min(T a, T b) {
    return (a < b) ? a : b;
}
