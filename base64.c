
#include "base64.h"



char convertBitsToBase64Char(unsigned int bits);
void addPadding(char* base64, size_t padding);

char* binaryToBase64(const unsigned int* binary, size_t binary_len) {
    static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t base64_len = ((binary_len + 2) / 3) * 4;
    char* base64 = (char*) malloc(base64_len + 1);
    size_t i, j;

    for (i = 0, j = 0; i < binary_len; i += 3, j += 4) {
        unsigned int bits = 0;
        size_t k;

        for (k = 0; k < 3; k++) {
            bits <<= 6;
            if (i + k < binary_len) {
                bits |= binary[i + k];
            }
        }

        base64[j] = convertBitsToBase64Char((bits & 0xFC0000) >> 18);
        base64[j + 1] = convertBitsToBase64Char((bits & 0x03F000) >> 12);
        base64[j + 2] = convertBitsToBase64Char((bits & 0x000FC0) >> 6);
        base64[j + 3] = convertBitsToBase64Char(bits & 0x00003F);
    }

    size_t padding = (3 - (binary_len % 3)) % 3;
    addPadding(base64, padding);

    base64[base64_len] = '\0';
    return base64;
}

char convertBitsToBase64Char(unsigned int bits) {
    static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    return base64_table[bits];
}

void addPadding(char* base64, size_t padding) {
    size_t base64_len = strlen(base64);
    size_t i;
    for (i = 0; i < padding; i++) {
        base64[base64_len - 1 - i] = '=';
    }
}


