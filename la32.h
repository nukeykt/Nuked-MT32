/*
 * Copyright (C) 2024, 2025 nukeykt
 *
 * This file is part of Nuked-MT32.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *  Thanks:
 *      John McMaster (https://siliconprawn.org):
 *          LA32 chip decap
 *
 */
#pragma once
#include <stdint.h>

class mt32_t;

class la32_t
{
private:
    mt32_t* mt32;

    uint8_t* pcm;

    uint64_t la32_cycles;

    uint8_t reg_1c0; // 0
    uint8_t reg_1c1; // 192
    uint8_t reg_1c2; // 0
    uint8_t reg_1c3; // 0
    uint8_t reg_data_l;
    uint16_t reg_file[6][32];
    uint32_t ram2[3][32];

    uint32_t w46;

    uint16_t w186;
    uint16_t w187;
    uint16_t w188;

    uint8_t int_status;
    bool int_state;

    int32_t accum[2][8];
    uint32_t oddeven;

    int32_t mem[2];
    int32_t prev;

public:

    la32_t(mt32_t* _mt32);

    void write(uint16_t address, uint8_t data);
    uint8_t read(uint16_t address);
    void clock(uint32_t samples);

};

