#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>


#define WORDSIZE 32

uint32_t H[] = {
  0x6a09e667,
  0xbb67ae85,
  0x3c6ef372,
  0xa54ff53a,
  0x510e527f,
  0x9b05688c,
  0x1f83d9ab,
  0x5be0cd19
};

static uint32_t K[64] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
  0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
  0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
  0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
  0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
  0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
  0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
  0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

uint32_t ch (uint32_t x, uint32_t y, uint32_t z) {
  return (x & y) ^ (~x & z);
}

uint32_t maj (uint32_t x, uint32_t y, uint32_t z) {
  return (x & y) ^ (x & z) ^ (y & z);
}

uint32_t left_rotate (uint32_t n, uint8_t d) {
  return (n << d) | (n >> (WORDSIZE-d));
}

uint32_t right_rotate (uint32_t n, uint8_t d) {
  uint32_t temp = (n >> d) | (n << (WORDSIZE-d));
  return temp;
}

uint32_t left_shift (uint32_t n, uint8_t d) {
  return n << d;
}

uint32_t right_shift (uint32_t n, uint8_t d) {
  return n >> d;
}

uint32_t lc_sigma0 (uint32_t x) {
  uint32_t temp = right_rotate(x, 7) ^ right_rotate(x, 18) ^ right_shift(x, 3);
  return temp;
}

uint32_t lc_sigma1 (uint32_t x) {
  uint32_t temp = right_rotate(x, 17) ^ right_rotate(x, 19) ^ right_shift(x, 10);
  return temp;
}

uint32_t cap_sigma0 (uint32_t x) {
  return right_rotate(x, 2) ^ right_rotate(x, 13) ^ right_rotate(x, 22);
}

uint32_t cap_sigma1 (uint32_t x) {
  return right_rotate(x, 6) ^ right_rotate(x, 11) ^ right_rotate(x, 25);
}

void digest (void *buff, uint64_t nblocks) {
  struct block{
    uint8_t data[64]; // 512 bits
  };

  struct block *blocks = (struct block *) buff;
  
  
  for (int i=0; i<nblocks; i++) {
    uint32_t W[64];

    uint32_t a, b, c, d, e, f, g, h; // Working variables

    memcpy(W, &blocks[i], sizeof(struct block)); // First 512 bits

    for (int t=16; t<64; t++) {
      W[t] = lc_sigma1(W[t-2]) + W[t-7] + lc_sigma0(W[t-15]) + W[t-16];
    }

    a = H[0];
    b = H[1];
    c = H[2];
    d = H[3];
    e = H[4];
    f = H[5];
    g = H[6];
    h = H[7];

    for (int t=0; t<64; t++) {
      uint32_t T1 = h + cap_sigma1(e) + ch(e, f, g) + K[t] + W[t];
      uint32_t T2 = cap_sigma0(a) + maj(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + T1;
      d = c;
      c = b;
      b = a;
      a = T1 + T2;
    }

    H[0] += a;
    H[1] += b;
    H[2] += c;
    H[3] += d;
    H[4] += e;
    H[5] += f;
    H[6] += g;
    H[7] += h;
  }
}

char *alloc (char *input, uint64_t inlen, uint64_t buffsize) {
  char *buff = malloc(buffsize);
  if (!buff) {
    printf("Malloc error\n");
    exit(1);
  }

  strncpy(buff, input, inlen);

  return buff;
}

void pad(char *buff, uint64_t inlen, uint64_t buffsize) {
  uint32_t taken = 0;
  for (int i=0; i<buffsize; i+=4) {
    for (int j=3; j>=0; j--) {
      if (!buff[i+j]) {
        taken = i+j;
        break;
      }
    }
    if (taken) break;
  }
  buff[taken] = 0b10000000;

  // uint64_t *big_buff = (uint64_t *)buff;
  uint64_t num = inlen*8;
  memcpy(buff+buffsize-4, &num, sizeof(uint64_t));
}

void big_endian(char *buff, uint64_t num_bytes) {
  char *temp = malloc(num_bytes);
  memcpy(temp, buff, num_bytes);
  for (int i=0; i<num_bytes; i+=4) {
    for (int j=0; j<4; j++) {
      buff[i+j] = temp[i+3-j];
    }
  }
  free(temp);
  temp = NULL;
}

void print_res() {
  uint8_t *print_buff = (uint8_t *)H;
  printf("Hash: ");
  for (int i=0; i<32; i+=4) {
    for (int j=3; j>=0; j--) {
      printf("%02x", print_buff[i+j]);
    }
  }
  printf("\n");
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Unexpected number of argument. Usage: %s [pass]\n", argv[0]);
    return 1;
  }

  char *input = argv[1];
  uint64_t inlen = strlen(input);
  uint64_t nchunks = (inlen+8)*8 / 512 + 1; // (input length + 8 bytes for length segment)*number of bits per byte / number of bits per block + 1 to start from a buffer of one block
  uint64_t buffsize = nchunks*512/8;

  char *buff = alloc(input, inlen, buffsize);
  memset(buff+inlen+1, 0, buffsize-inlen-1);

  big_endian(buff, inlen); // Convert text encoding to big endian
  pad(buff, inlen, buffsize);
  digest(buff, nchunks);

  print_res();

  free(buff);
  buff = NULL;

  return 0;
}
