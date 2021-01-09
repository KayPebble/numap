/*
 * Copyright 2021 Andrew Kistler
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <windows.h>
#include <numap.h>

// 'a' = 97
// 'z' = 122
// 'A' = 65
//
uint16_t numap2[] = { 3, 0, 'a', 'A' - 'a', 'z' + 1, 0 };

static inline uint8_t toupper1(uint8_t c)
{
  uint8_t numap[] = { 3, // 3 partitions

                      0, 'a',             // 1st [0,'a') -> 0
                     'A' - 'a', 'z' + 1, // 2nd ['a','z'] -> 'A' - 'a'
		      0 };                // 3rd ('z', UINT8_MAX] -> 0
  return c + numap_map8(numap, c);
}

static inline uint8_t toupper2(uint8_t c)
{
  uint16_t o;
  uint16_t temp;

  __asm__(
    "addb $133, %[input];"
    "movw $0, %[temp];"
    "cmovcw %[temp], %%cx;"

    "addb $26, %[input];"
    "movw $224, %[temp];"
    "cmovcw %[temp], %%cx;"

    "addb $97, %[input];"
    "movw $0, %[temp];"
    "cmovcw %[temp], %%cx;"

    "movw %%cx, (%[output]);"
  :
  :
    [ output ] "r" ( &o ),
    [ temp ] "r" ( temp ),
    [ input ] "r" ( c )
  : "ecx"
 );

 return c + (uint8_t)o;
}

#define NSAMPLES 32768

int main(int argc, char** argv)
{
  int i;
  uint8_t arr[NSAMPLES];
  time_t t;
  LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
  LARGE_INTEGER Frequency;

  if (argc == 2) {
    uint16_t c;
    sscanf(argv[1], "%d", &c);
    printf("->%d\n", toupper1(c));
    printf("->%d\n", toupper2(c));
    return 0;
  }

  srand((unsigned)time(&t));

  for (i = 0; i < NSAMPLES; i++) {
    arr[i] = rand();
  }

  QueryPerformanceFrequency(&Frequency);
  QueryPerformanceCounter(&StartingTime);

  for (i = 0; i < NSAMPLES; i++) {
    toupper1(arr[i]);
  }

  QueryPerformanceCounter(&EndingTime);

  ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
  ElapsedMicroseconds.QuadPart *= 1000000;
  ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

  printf("[1] %lu us", ElapsedMicroseconds.QuadPart);
  printf(" / %d\n", NSAMPLES);

  QueryPerformanceCounter(&StartingTime);

  for (i = 0; i < NSAMPLES; i++) {
    toupper2(arr[i]);
  }

  QueryPerformanceCounter(&EndingTime);

  ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
  ElapsedMicroseconds.QuadPart *= 1000000;
  ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

  printf("[2] %lu us", ElapsedMicroseconds.QuadPart);
  printf(" / %d\n", NSAMPLES);

  numap_compile16(numap2, arr, arr);

  return 0;
}

