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
#include "mt32.h"
#include "rv2.h"

rv2_t::rv2_t(mt32_t *_mt32) : mt32(_mt32) {
    rom = &mt32->rvrom[0];
}

void rv2_t::set_hiaddr(uint32_t _hiaddr) {
    hiaddr = _hiaddr;
    rom = &mt32->rvrom[hiaddr << 13];
}

void rv2_t::accum_op(uint8_t ctrl, uint8_t ext) {
    if ((ctrl & 16) == 0) {
        accum = 0;
    } else {
        bool update = ((((ctrl >> 4) & 0xe) | ext) & (1 << shift_sel)) != 0;
        if (update) {
            int32_t sum = accum + shifter + shifter_add;
            if (sum < -32768)
                sum = -32768;
            else if (sum > 32767)
                sum = 32767;
            if (ctrl & 8) {
                sum = ~sum;
            }
            accum = (int16_t)sum;
        }
    }
    shifter_add = shifter & 1;
    shifter = shifter >> 1;
    if ((ctrl & 4) == 0) {
        shifter = ramin;
    }
    if (shifter >= 0) {
        shifter_add = 0;
    }
}

void rv2_t::clock(uint32_t cycles) {
    uint32_t saw1v = (saw1 >> 1) & 15;
    if (saw0 & 16) {
        saw1v = 15;
    }

    for (uint32_t i = 0; i < cycles; i += 4) {
        // output
        if (state == 192) {
            sample0 = accum;
        } else if (state == 228) {
            sample1 = accum;
        }

        // accumulators
        accum_op(ctrl0, (ctrlext >> 1) & 1);
        accum_op(ctrl1, ctrlext & 1);

        // rom and ram fetch for next cycle

        uint8_t* bytes = &rom[hiaddrsaw | state];
        uint32_t ramaddr = (samples + (bytes[0] | (bytes[2] << 8))) & 0x3fff;
        if ((bytes[1] & 2) == 0) {
            ram[ramaddr] = (bytes[1] & 1) != 0 ? input : accum;
        }
        ramin = ram[ramaddr];

        ctrl0 = bytes[1];
        ctrl1 = bytes[3];
        ctrlext = (bytes[2] >> 6) & 3;

        // state
        state += 4;
        if (state == 0) {
            samples++;

            hiaddrsaw = (saw0 << 9) & 0x1c00;
            hiaddrsaw |= (saw1v << 6) & 0x300;

            shift_sel = saw1v & 3;
        }
    }
}
