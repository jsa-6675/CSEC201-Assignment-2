#include "hash.h"

unsigned char* SSHA(const unsigned char* msg, size_t length) {
    unsigned char A, B, C, D, E; //Initial Seed Value
    A = 56;
    B = 99;
    C = 102;
    D = 67;
    E = 76;

    for (int i = 0; i < length; i++) {
        for (int round = 0; round < 8; round++) {
            unsigned char old_A = A;
            unsigned char old_E = E;

            unsigned char shifted_A = A >> 2;
            unsigned char shifted_B = B >> 1;
            unsigned char g = (B & C) | (C & D);

            A = old_E;
            B = old_A;
            C = shifted_A + old_E;
            D = shifted_A ^ shifted_B;
            E = g + msg[i] + shifted_B;
        }
    }

    unsigned char* digest = (unsigned char*)malloc(DIGEST_SIZE * sizeof(unsigned char));
    digest[0] = A;
    digest[1] = B;
    digest[2] = C;
    digest[3] = D;
    digest[4] = E;
    return digest;
}

unsigned char* SSHA2(const unsigned char* msg, size_t length) {
    // Allocate 5 bytes for the hash output (A, B, C, D, E)
    unsigned char* hash = (unsigned char*)malloc(5 * sizeof(unsigned char));
    if (hash == NULL) {
        return NULL;
    }

    // Initialize registers A, B, C, D, E
    // (Adjust these initial values if your assignment specifies different constants, e.g., 0)
    unsigned char A = 'A';
    unsigned char B = 'B';
    unsigned char C = 'C';
    unsigned char D = 'D';
    unsigned char E = 'E';

    for (size_t i = 0; i < length; i++) {
        // Compute intermediate components
        unsigned char a_shift = A >> 2;
        unsigned char b_shift = B >> 1;
        unsigned char bc_and = B & C;
        unsigned char cd_and = C & D;
        unsigned char or_res = bc_and | cd_and;

        // Compute next state values simultaneously
        unsigned char next_A = E;
        unsigned char next_B = A;
        unsigned char next_C = a_shift + E;
        unsigned char next_D = a_shift ^ b_shift;
        unsigned char next_E = b_shift + or_res + msg[i];

        // Update state registers for the next round
        A = next_A;
        B = next_B;
        C = next_C;
        D = next_D;
        E = next_E;
    }

    // Store final state in the allocated buffer
    hash[0] = A;
    hash[1] = B;
    hash[2] = C;
    hash[3] = D;
    hash[4] = E;

    return hash;
}

int digest_equal(struct Digest digest1, struct Digest digest2) {
    return ((digest1.hash0 == digest2.hash0) &&
        (digest1.hash1 == digest2.hash1) &&
        (digest1.hash2 == digest2.hash2) &&
        (digest1.hash3 == digest2.hash3) &&
        (digest1.hash4 == digest2.hash4));
}

void printDigest(struct Digest digest) {
    printf("%d %d %d %d %d\n", digest.hash0, digest.hash1, digest.hash2, digest.hash3, digest.hash4);
}