#ifndef ARRAYS_L_H
#define ARRAYS_L_H

char arrContains(void **arr, int n, void *target);

char *newRandomBytes(int n);

void randomBytes(char *out, int n);

void reverseArray(unsigned char *arr, int i1, int i2);

void leftRotate(unsigned char *arr, int d, int n);

unsigned long long leftRotateLL(unsigned long long w, unsigned int d);

unsigned int leftRotateI(unsigned int w, unsigned int d);

unsigned char leftRotateC(unsigned char w, unsigned int d);

void rightRotate(unsigned char *arr, int d, int n);

unsigned long long rightRotateLL(unsigned long long w, unsigned int d);

unsigned int rightRotateI(unsigned int w, unsigned int d);

unsigned char rightRotateC(unsigned char w, unsigned int d);

#endif //ARRAYS_L_H
