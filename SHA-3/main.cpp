#include <bitset>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "hash/sha.h"

void print_hex(unsigned char *data, int len) {
    for (int i = 0; i < len; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    }
    std::cout << std::endl;
}

int main() {
    const char *passwords[] = {"sveklo", "hahahaha", "00000000"};
    int num_passwords = sizeof(passwords) / sizeof(passwords[0]);

    for (int p = 0; p < num_passwords; ++p) {
        const char *password = passwords[p];
        const char *message = password;
        unsigned char *output;
        std::cout << "Password: \"" << password << "\"" << std::endl;

        const char *sha_modes[] = {SHA3_128_STR, SHA3_256_STR, SHA3_512_STR};
        int num_modes = sizeof(sha_modes) / sizeof(sha_modes[0]);

        for (int i = 0; i < num_modes; ++i) {
            const char *mode = sha_modes[i];
            int ret_len = sha_getRetLen(const_cast<char *>(mode));

            void *ctx = sha_initContextStr(const_cast<char *>(mode));
            sha_updateStr(const_cast<char *>(mode), ctx, reinterpret_cast<unsigned char *>(const_cast<char *>(message)),
                          std::strlen(message));
            sha_digestStr(const_cast<char *>(mode), ctx, &output);

            std::cout << "Mode: " << mode << std::endl;
            std::cout << "Output: ";
            print_hex(output, ret_len);

            // Take the first 8 bits (byte) of the hash
            unsigned char y0 = output[0];
            std::cout << "y0: ";
            std::cout << std::bitset<8>(y0) << std::endl;

            // Free context and output
            sha_free(ctx);
            free(output);
        }
        std::cout << std::endl;
    }

    return 0;
}
