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

#include <numap.h>
#include <string.h>
#include <stdio.h>

#define MAP(T,t) \
T numap_map##t(T* arr, T in) \
{ \
  T i; \
  T ix; \
  T n; \
  n = arr[0]; \
  for (i = 0, ix = 2; i < (n - 1); i++, ix += 2) { \
    if (in < arr[ix]) { \
      return arr[ix - 1]; \
    } \
  } \
  return arr[ix - 1]; \
}

MAP(uint8_t, 8)
MAP(uint16_t, 16)
MAP(uint32_t, 32)
MAP(uint64_t, 64)

int numap_compile16(uint16_t* arr, uint8_t* buf, void* reentry)
{
  uint16_t i;
  uint16_t n;
  uint16_t ix;
  uint16_t partsize = 0;
  uint16_t size;

  uint8_t code[] = { 0x66, 0x05, 0x00, 0x00,   // addw $imm, %ax;
                     0x66, 0xBA, 0x00, 0x00,   // movw $imm, %dx;
                     0x66, 0x0F, 0x42, 0xCA }; // cmovcw %dx, %cx;

  uint8_t jmp[] =  { 0x66, 0x89, 0x0B,             // mov %cx, (%ebx)
	             0xB9, 0x00, 0x00, 0x00, 0x00, // movq $0, %ecx;
                     0xFF, 0xE1 };                 // jmp ecx

  n = arr[0];

  if (NULL == buf) { return sizeof(code) * n + sizeof(jmp); }

  size = 0;


  ix = (n << 1) - 1;
  partsize = 0 - arr[ix - 1];

  memcpy(&buf[size], code, sizeof(code));
  memcpy(&buf[size+2], &partsize, sizeof(partsize));
  memcpy(&buf[size+6], &arr[ix], sizeof(partsize));
  size += sizeof(code);

  for (i = 0, ix = (n << 1) - 3; i < (n - 2); i++, ix -= 2) {
    partsize = arr[ix + 1] - arr[ix - 1];
    memcpy(&buf[size], code, sizeof(code));
    memcpy(&buf[size+2], &partsize, sizeof(partsize));
    memcpy(&buf[size+6], &arr[ix], sizeof(partsize));
    size += sizeof(code);
  }

  partsize = arr[ix + 1];
  memcpy(&buf[size], code, sizeof(code));
  memcpy(&buf[size+2], &partsize, sizeof(partsize));
  memcpy(&buf[size+6], &arr[ix], sizeof(partsize));
  size += sizeof(code);

  memcpy(&buf[size], jmp, sizeof(jmp));
  memcpy(&buf[size+4], &reentry, sizeof(void*));
  size += sizeof(jmp);

  return size;
}

