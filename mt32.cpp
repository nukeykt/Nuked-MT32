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
 */
#include <stdlib.h>
#include "mt32.h"
#include "la32.h"
#include "rv2.h"
#include "mame/emu.h"
#include "mame/i8x9x.h"

mt32_t::mt32_t()
{
    mcu = new p8098_device(this);

    mcu->device_start();
    mcu->device_reset();

    la32 = new la32_t(this);
    rv2 = new rv2_t(this);

    reg_bank = 0;
    bank_ptr = &rom[0];

    button = 0;

    knob = 1020;

    lcd_cycles = 0;

    ga_lcd_queue_cnt = 0;

    lcd_reset();

    uart_buffer_s = 0;
    uart_buffer_e = 0;
    uart_cycles = 0;
}

mt32_t::~mt32_t()
{
    delete mcu;
}

uint8_t mt32_t::cpu_read(uint16_t address)
{
    uint32_t addr_hi = (address >> 7) & 0x7fff;

    if (addr_hi >= 0x20 && addr_hi < 0x100)
    {
        //if (address == 0x2c70)
        //    address += 0;
        return rom[address];
    }
    else if (addr_hi >= 0x180 && addr_hi < 0x200)
    {
        return ram[address & 0x3fff];
    }
    else if (addr_hi >= 0x100 && addr_hi < 0x180)
    {
        return bank_ptr[address & 0x3fff];
    }
    else if (addr_hi >= 0x18 && addr_hi < 0x20)
    {
        return la32->read(address & 0x1ff);
    }
    else if (addr_hi == 7)
    {
        uint8_t v = 0;
        if (mcu->total_cycles() < lcd_cycles)
            v |= 1;
        return v;
    }
    else if (addr_hi == 4)
    {
        int sc = (address >> 1) & 15;
        int ret = 0xff;
        if ((sc & 2) == 0)
        {
            if (button & (1 << MT32_BUTTTON_1))
                ret &= ~1;
            if (button & (1 << MT32_BUTTTON_2))
                ret &= ~2;
            if (button & (1 << MT32_BUTTTON_3))
                ret &= ~4;
            if (button & (1 << MT32_BUTTTON_SOUND_GROUP))
                ret &= ~8;
            if (button & (1 << MT32_BUTTTON_VOLUME))
                ret &= ~16;
        }
        if ((sc & 1) == 0)
        {
            if (button & (1 << MT32_BUTTTON_4))
                ret &= ~1;
            if (button & (1 << MT32_BUTTTON_5))
                ret &= ~2;
            if (button & (1 << MT32_BUTTTON_RHYTHM))
                ret &= ~4;
            if (button & (1 << MT32_BUTTTON_SOUND))
                ret &= ~8;
            if (button & (1 << MT32_BUTTTON_MASTER_VOLUME))
                ret &= ~16;
        }

        return ret;
    }

    return 0xff;
}

void mt32_t::cpu_write(uint16_t address, uint8_t data)
{
    uint32_t addr_hi = (address >> 7) & 0x1ff;
    if (addr_hi >= 0x100 && addr_hi < 0x180)
    {
        if (reg_bank & 0x10)
            bank_ptr[address & 0x3fff] = data;
    }
    else if (addr_hi >= 0x180 && addr_hi < 0x200)
    {
        ram[address & 0x3fff] = data;
    }
    else if (addr_hi >= 0x18 && addr_hi < 0x20)
    {
        //printf("la32 write: %x %x\n", address & 0x1ff, data);
        la32->write(address & 0x1ff, data);
    }
    else
    {
        switch (addr_hi)
        {
            case 2:
                reg_bank = data & 0x1f;
                if (reg_bank & 0x10)
                    bank_ptr = &ram[(reg_bank & 1) << 14];
                else
                    bank_ptr = &rom[(reg_bank & 7) << 14];
                break;
            case 4:
                rv2->set_hiaddr((data >> 1) & 3);
                break;
            case 5:
                break;
            case 6:
                //printf("LCD data %x %c\n", data, data);
                if (ga_lcd_queue_cnt < 20)
                {
                    ga_lcd_queue[ga_lcd_queue_cnt++] = data;
                }
                break;
            case 7:
                //printf("LCD ctrl %x %c\n", data, data);
                if (data & 0x80)
                {
                    uint8_t pos = data & 0x7f;
                    if ((pos >= 0 && pos < 10) || (pos >= 64 && pos < 64 + 10))
                    {
                        lcd_write(0, data);
                        for (uint32_t i = 0; i < ga_lcd_queue_cnt; i++)
                        {
                            lcd_write(1, ga_lcd_queue[i]);
                        }
                    }
                }
                else
                    lcd_write(0, data);
                ga_lcd_queue_cnt = 0;
                break;
            case 8:
                rv2->write(1, data << 1);
                break;
            case 16:
                rv2->write(0, data << 1);
                break;

            default:
                printf("%x %x %c\n", address, data, data);
                break;
        }
    }
}


uint32_t mt32_t::get_knob()
{
    int32_t val = knob;

    if (val > 1020)
        val = 1020;

    //val += (rand() % 4) - 2;
    if (val > 1023)
        val = 1023;
    else if (val < 0)
        val = 0;
    return val;
}

void mt32_t::clock(uint64_t samples)
{
    la32->clock(samples);

    lcd_render();
}

void mt32_t::push_midi()
{
    if (uart_cycles == 0 || uart_cycles < mcu->total_cycles())
    {
        midi_mutex.lock();
        if (uart_buffer_s != uart_buffer_e)
        {
            uint8_t b = uart_buffer[uart_buffer_s];

            mcu->serial_w(b);

            uart_buffer_s = (uart_buffer_s + 1) % uart_buffer_size;

            uart_cycles = mcu->total_cycles() + 3840;
        }
        midi_mutex.unlock();
    }
}

void mt32_t::post_midi(uint8_t byte)
{
    midi_mutex.lock();

    if ((uart_buffer_e + 1) % uart_buffer_size != uart_buffer_s)
    {
        uart_buffer[uart_buffer_e] = byte;
        uart_buffer_e = (uart_buffer_e + 1) % uart_buffer_size;
    }
    midi_mutex.unlock();
}
