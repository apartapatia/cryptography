#include <cstdlib>
#include <ctime>
#include "arrays_l.h"

char arrContains(void **arr, int n, void *target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return 1;
        }
    }
    return 0;
}

// random array generation
char *newRandomBytes(int n) {
    char *ret = static_cast<char *>(malloc(n));
    if (!ret) {
        return nullptr;
    }
    randomBytes(ret, n);
    return ret;
}

void randomBytes(char *out, int n) {
    if (!out || !n || n < 0) {
        return;
    }
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < n; i++) {
        out[i] = rand() & 0xff; // cap at 255
    }
}

void reverseArray(unsigned char *arr, int i1, int i2) {
    while (i1 < i2) {
        // swap corresponding elements across the middle
        unsigned char tmp = arr[i1];
        arr[i1] = arr[i2];
        arr[i2] = tmp;
        i1++;
        i2--;
    }
}

void leftRotate(unsigned char *arr, int d, int n) {
    reverseArray(arr, 0, d - 1);
    reverseArray(arr, d, n - 1);
    reverseArray(arr, 0, n - 1);
}

unsigned long long leftRotateLL(unsigned long long w, unsigned int d) {
    d &= 0x3f; // mod 64
    return (w << d) | (w >> (64 - d));
}

unsigned int leftRotateI(unsigned int w, unsigned int d) {
    d &= 0x1f; // mod 32
    return (w << d) | (w >> (32 - d));
}

unsigned char leftRotateC(unsigned char w, unsigned int d) {
    d &= 0x7; // mod 8
    return (w << d) | (w >> (8 - d));
}

void rightRotate(unsigned char *arr, int d, int n) {
    // complementary left rotation
    leftRotate(arr, n - d, n);
}

unsigned long long rightRotateLL(unsigned long long w, unsigned int d) {
    d &= 0x3f; // mod 64
    return (w >> d) | (w << (64 - d));
}

unsigned int rightRotateI(unsigned int w, unsigned int d) {
    d &= 0x1f; // mod 32
    return (w >> d) | (w << (32 - d));
}

unsigned char rightRotateC(unsigned char w, unsigned int d) {
    d &= 0x7; // mod 8
    return (w >> d) | (w << (8 - d));
}
