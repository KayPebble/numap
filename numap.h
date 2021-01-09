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


#ifndef NUMAP_H
#define NUMAP_H

#include <stdint.h>

uint8_t  numap_map8 (uint8_t*  numap, uint8_t  in);
uint16_t numap_map16(uint16_t* numap, uint16_t in);
uint32_t numap_map32(uint32_t* numap, uint32_t in);
uint64_t numap_map64(uint64_t* numap, uint64_t in);

int numap_compile16(uint16_t* numap, uint8_t* buf, void* reentry);

#endif
