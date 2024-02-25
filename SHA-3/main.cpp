#include <iostream>
#include <iomanip>
#include <cstring>
#include <random>
#include <vector>
#include <bitset>
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
    std::uniform_int_distribution<> length_dist(1, 100); // Random length generation from 1 to 20

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

    for (int bits = 8; bits <= 16; bits += 2) {
        std::vector<unsigned short> y0_values(num_passwords);

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

                unsigned short y0 = 0;
                int num_bytes_needed = (bits + 7) / 8;
                unsigned short mask = (1 << bits) - 1;
                for (int j = 0; j < num_bytes_needed; ++j) {
                    y0 |= static_cast<unsigned short>(output[j]) << (8 * j);
                }
                y0 &= mask; // Masking to get only required number of bits

                y0_values[p] = y0;

                // Free context and output
                sha_free(ctx);
                free(output);
            }
            std::cout << std::endl;
        }

        // Printing y0 values
        for (int i = 0; i < num_passwords; ++i) {
            std::cout << "y0 values for password \"" << passwords[i] << "\":" << std::endl;
            std::cout << "Mode " << sha_modes[0] << ": " << std::bitset<16>(y0_values[i]) << std::endl;
            std::cout << std::endl;
        }

        // Find second pre-image and collisions
        int second_preimage_complexity_sum = 0;
        int collision_complexity_sum = 0;
        int experiments = 1000;

        for (int exp = 0; exp < experiments; ++exp) {
            int N = 1000;
            int second_preimage_complexity = 0;
            int collision_complexity = 0;
            bool second_preimage_found = false;
            bool collision_found = false;
            std::vector<unsigned short> y_values(N);

            for (int n = 0; n < N; ++n) {
                std::string rand_password = generate_random_word();
                const char *mode = "SHA3_512_STR";

                unsigned char *output;

                void *ctx = sha_initContextStr(const_cast<char *>(mode));
                sha_updateStr(const_cast<char *>(mode), ctx,
                            reinterpret_cast<unsigned char *>(const_cast<char *>(rand_password.c_str())),
                            rand_password.length());
                sha_digestStr(const_cast<char *>(mode), ctx, &output);

                unsigned short yi = 0;
                int num_bytes_needed = (bits + 7) / 8;
                unsigned short mask = (1 << bits) - 1;
                for (int j = 0; j < num_bytes_needed; ++j) {
                    yi |= static_cast<unsigned short>(output[j]) << (8 * j);
                }
                yi &= mask; // Masking to get only required number of bits

                y_values[n] = yi;

                // Check for second pre-image
                if (!second_preimage_found && yi == y0_values[0]) {
                    second_preimage_complexity = n + 1;
                    second_preimage_found = true;
                }

                // Check for collisions
                for (int i = 0; i < n; ++i) {
                    if (second_preimage_found && !collision_found && y_values[i] == yi) {
                        collision_complexity = n + 1;
                        collision_found = true;
                        break;
                    }
                }

                // Stop if both second pre-image and collision are found
                if (second_preimage_found && collision_found)
                    break;

                sha_free(ctx);
                free(output);
            }

            second_preimage_complexity_sum += second_preimage_complexity;
            collision_complexity_sum += collision_complexity;
        }

        std::cout << std::dec << "Average second pre-image complexity (bits=" << bits << "): " << second_preimage_complexity_sum / static_cast<double>(experiments) << std::endl;
        std::cout << std::dec <<"Average collision complexity (bits=" << bits << "): " << collision_complexity_sum / static_cast<double>(experiments) << std::endl;
    }

    return 0;
}
