#include <iostream>
#include <iomanip>
#include <cstring>
#include <random>
#include <vector>
#include <bitset>
#include "hash/sha.h"

// Function to print hex output
void printHex(unsigned char *data, int len) {
    for (int i = 0; i < len; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    }
    std::cout << std::endl;
}

// Generate random word
std::string generateRandomWord() {
    std::string word;
    static constexpr char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> lengthDist(1, 100); // Random length generation from 1 to 100

    int length = lengthDist(gen); // Random word length
    std::uniform_int_distribution<> charDist(0, sizeof(alphanum) - 2);

    for (int i = 0; i < length; ++i) {
        word += alphanum[charDist(gen)];
    }
    return word;
}

// Calculate y0 value from output
unsigned short calculateY0(unsigned char *output, int bits) {
    unsigned short y0 = 0;
    int numBytesNeeded = (bits + 7) / 8;
    unsigned short mask = (1 << bits) - 1;
    for (int j = 0; j < numBytesNeeded; ++j) {
        y0 |= static_cast<unsigned short>(output[j]) << (8 * j);
    }
    return y0 & mask; // Masking to get only required number of bits
}

// Run experiments
void runExperiments(int bits, std::vector<unsigned short>& y0Values, const std::vector<const char*>& passwords, const char* mode) {
    constexpr int experiments = 1000;
    int secondPreimageComplexitySum = 0;
    int collisionComplexitySum = 0;

    for (int exp = 0; exp < experiments; ++exp) {
        int N = 5000;
        int secondPreimageComplexity = 0;
        int collisionComplexity = 0;
        bool secondPreimageFound = false;
        bool collisionFound = false;
        std::vector<unsigned short> yValues(N);

        for (int n = 0; n < N; ++n) {
            std::string randPassword = generateRandomWord();
            unsigned char *output;

            void *ctx = sha_initContextStr(const_cast<char *>(mode));
            sha_updateStr(const_cast<char *>(mode), ctx,
                          reinterpret_cast<unsigned char *>(const_cast<char *>(randPassword.c_str())),
                          randPassword.length());
            sha_digestStr(const_cast<char *>(mode), ctx, &output);

            unsigned short yi = calculateY0(output, bits);
            yValues[n] = yi;

            // Check for second pre-image
            if (!secondPreimageFound && yi == y0Values[0]) {
                secondPreimageComplexity = n + 1;
                secondPreimageFound = true;
            }

            // Check for collisions
            for (int i = 0; i < n; ++i) {
                if (secondPreimageFound && !collisionFound && yValues[i] == yi) {
                    collisionComplexity = n + 1;
                    collisionFound = true;
                    break;
                }
            }

            // Stop if both second pre-image and collision are found
            if (secondPreimageFound && collisionFound)
                break;

            sha_free(ctx);
            free(output);
        }
        secondPreimageComplexitySum += secondPreimageComplexity;
        collisionComplexitySum += collisionComplexity;
    }


    std::cout << std::dec << "Average second pre-image complexity (bits=" << bits << "): "
              << secondPreimageComplexitySum / static_cast<double>(experiments) << std::endl;
    std::cout << std::dec << "Average collision complexity (bits=" << bits << "): "
              << collisionComplexitySum / static_cast<double>(experiments) << std::endl;
}

int main() {
    const std::vector passwords = {"The quick brown fox jumps over the lazy dog", "The quick brown fox jumps over the lazy dog.", "00000000"};
    const std::vector shaModes = { SHA3_256_STR}; // SHA3_128_STR, SHA3_256_STR, for another test

    for (int bits = 8; bits <= 16; bits += 2) {
        std::vector<unsigned short> y0Values(passwords.size());

        for (int p = 0; p < passwords.size(); ++p) {
            const char *password = passwords[p];
            const char *mode = shaModes[0];

            unsigned char *output;
            void *ctx = sha_initContextStr(const_cast<char *>(mode));
            sha_updateStr(const_cast<char *>(mode), ctx, reinterpret_cast<unsigned char *>(const_cast<char *>(password)),
                          std::strlen(password));
            sha_digestStr(const_cast<char *>(mode), ctx, &output);

            std::cout << "Password: \"" << password << "\"" << std::endl;
            std::cout << "Mode: " << mode << std::endl;
            std::cout << "Output: ";
            printHex(output, sha_getRetLen(const_cast<char *>(mode)));

            y0Values[p] = calculateY0(output, bits);

            sha_free(ctx);
            free(output);
        }

        for (int i = 0; i < passwords.size(); ++i) {
            std::cout << "y0 values for password \"" << passwords[i] << "\":" << std::endl;
            std::cout << "Mode " << shaModes[0] << ": " << std::bitset<16>(y0Values[i]) << std::endl;
            std::cout << std::endl;
        }

        runExperiments(bits, y0Values, passwords, shaModes[0]);
    }

    return 0;
}
