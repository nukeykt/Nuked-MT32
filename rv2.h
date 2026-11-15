/*
 * Copyright (C) 2026 nukeykt
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
 *      InfoSecDJ (https://infosecdj.net/):
 *          Reverb chip decap
 *      giulioz (https://github.com/giulioz):
 *          Sending chip to decap
 */
#pragma once
#include <stdint.h>

class mt32_t;

class rv2_t
{
private:
    mt32_t* mt32;

    uint8_t* rom;

    uint8_t state = 0;
    uint16_t samples = 0;
    uint8_t saw0 = 0;
    uint8_t saw1 = 0;
    uint32_t hiaddr = 0;
    uint32_t hiaddrsaw = 0;
    int16_t input = 0;
    int16_t sample0 = 0;
    int16_t sample1 = 0;

    int16_t shifter = 0;
    int16_t shifter_add = 0;
    int16_t accum = 0;
    int16_t ramin = 0;
    uint8_t ctrl0 = 0, ctrl1 = 0, ctrlext = 0;
    int32_t shift_sel = 0;

    uint16_t ram[0x4000] = { 0 };

public:

    rv2_t(mt32_t* _mt32);

    void write(uint16_t address, uint8_t data) {
        if (address & 1) {
            saw1 = data & 31;
        }
        else {
            saw0 = data & 30;
        }
    }
    void set_hiaddr(uint32_t _hiaddr);
    void clock(uint32_t cycles);
    void set_input(int16_t _input) {
        input = _input >> 1;
    }
    void get_samples(int16_t* samples) {
        samples[0] = sample0;
        samples[1] = sample1;
    }
    void sync() {
        state = 0;
    }

    void accum_op(uint8_t ctrl, uint8_t ext);
};
