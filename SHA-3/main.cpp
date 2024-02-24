#include <bitset>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <random>
#include <vector>
#include "hash/sha.h"

void print_hex(unsigned char *data, int len) {
    for (int i = 0; i < len; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    }
    std::cout << std::endl;
}

std::string generate_random_word() {
    std::string word;
    static constexpr char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> length_dist(1, 20); // Random length generation from 1 to 20

    int length = length_dist(gen); // Random word length
    std::uniform_int_distribution<> char_dist(0, sizeof(alphanum) - 2);

    for (int i = 0; i < length; ++i) {
        word += alphanum[char_dist(gen)];
    }
    return word;
}

int main() {
    const char *passwords[] = {"", "hahahaha", "00000000"};
    int num_passwords = sizeof(passwords) / sizeof(passwords[0]);

    const char *sha_modes[] = {SHA3_512_STR}; // SHA3_128_STR, SHA3_256_STR, for another test
    int num_modes = sizeof(sha_modes) / sizeof(sha_modes[0]);

    std::vector y0_values(num_passwords, std::vector<unsigned char>(num_modes));

    for (int p = 0; p < num_passwords; ++p) {
        const char *password = passwords[p];
        const char *message = password;

        std::cout << "Password: \"" << password << "\"" << std::endl;

        for (int i = 0; i < num_modes; ++i) {
            const char *mode = sha_modes[i];
            int ret_len = sha_getRetLen(const_cast<char *>(mode));

            unsigned char *output;

            void *ctx = sha_initContextStr(const_cast<char *>(mode));
            sha_updateStr(const_cast<char *>(mode), ctx, reinterpret_cast<unsigned char *>(const_cast<char *>(message)),
                          std::strlen(message));
            sha_digestStr(const_cast<char *>(mode), ctx, &output);

            std::cout << "Mode: " << mode << std::endl;
            std::cout << "Output: ";
            print_hex(output, ret_len);

            unsigned char y0 = output[0];

            y0_values[p][i] = y0;

            // Free context and output
            sha_free(ctx);
            free(output);
        }
        std::cout << std::endl;
    }

    // Printing y0 values
    for (int i = 0; i < num_passwords; ++i) {
        std::cout << "y0 values for password \"" << passwords[i] << "\":" << std::endl;
        for (int j = 0; j < num_modes; ++j) {
            std::cout << "Mode " << sha_modes[j] << ": " << std::bitset<8>(y0_values[i][j]) << std::endl;
        }
        std::cout << std::endl;
    }

    // Find second pre-image
    int sr_second = 0;
    for (int k = 0; k < 1000; k++) {
        int N = 100000;
        int second_preimage_complexity = 0;
        bool found = false;
        std::vector yi_values(N, std::vector<unsigned char>(num_modes));

        for (int n = 0; n < N; ++n) {
            std::string rand_password = generate_random_word();
            const char *mode = "SHA3_512_STR";

            unsigned char *output;

            void *ctx = sha_initContextStr(const_cast<char *>(mode));
            sha_updateStr(const_cast<char *>(mode), ctx,
                          reinterpret_cast<unsigned char *>(const_cast<char *>(rand_password.c_str())),
                          rand_password.length());
            sha_digestStr(const_cast<char *>(mode), ctx, &output);

            unsigned char yi = output[0];

            yi_values[n][0] = yi;

            std::cout << std::dec;
            std::bitset<16> yi_first_8_bits(yi);

            for (int i = 0; i < 1; ++i) {
                std::bitset<16> y0_first_8_bits(y0_values[i][0]);

                // Сравнение
                if (yi_first_8_bits == y0_first_8_bits) {
                    second_preimage_complexity = n + 1;
                    sr_second += second_preimage_complexity;
                    //std::cout << second_preimage_complexity << std::endl;
                    found = true;
                    break;
                }
            }

            if(found) {
                break;
            }

            sha_free(ctx);
            free(output);
        }

    }
    std::cout << sr_second/1000;





    return 0;
}
