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
#pragma once
#include <stdint.h>
#include <mutex>

class mcs96_device;
class i8x9x_device;
class p8098_device;
class la32_t;
class rv2_t;

static constexpr int lcd_h = 100;
static constexpr int lcd_w = 840;

static constexpr uint64_t lcd_delay = 100;

enum
{
    MT32_BUTTTON_1 = 0,
    MT32_BUTTTON_2,
    MT32_BUTTTON_3,
    MT32_BUTTTON_SOUND_GROUP,
    MT32_BUTTTON_VOLUME,
    MT32_BUTTTON_4,
    MT32_BUTTTON_5,
    MT32_BUTTTON_RHYTHM,
    MT32_BUTTTON_SOUND,
    MT32_BUTTTON_MASTER_VOLUME,
};

class mt32_t
{
    friend class mcs96_device;
    friend class i8x9x_device;
    friend class p8098_device;
    friend class la32_t;
    friend class rv2_t;

private:
    uint8_t cpu_read(uint16_t address);
    void cpu_write(uint16_t address, uint8_t data);

    i8x9x_device* mcu;
    la32_t* la32;
    rv2_t* rv2;

    uint8_t ram[0x8000];
    uint8_t reg_bank;
    uint8_t *bank_ptr;

    uint8_t lcd_cg[4][8];
    uint8_t lcd_data[20];

    void lcd_write(uint8_t a, uint8_t data);
    void lcd_reset(bool soft = false);
    void lcd_font_render(int32_t x, int32_t y, uint8_t ch, bool cursor);
    void lcd_render();

    uint8_t lcd_cursor;
    uint8_t lcd_cursor_dir;
    uint8_t lcd_cgram_addr;
    uint8_t lcd_cgram_row;
    uint8_t lcd_lines;
    uint8_t lcd_cursor_font;
    uint8_t lcd_cursor_blink;
    uint8_t lcd_display_on;
    uint8_t lcd_cursor_on;
    uint64_t lcd_cycles;

    uint32_t get_knob();

    uint8_t ga_lcd_queue[20];
    uint32_t ga_lcd_queue_cnt;

    static constexpr uint32_t uart_buffer_size = 8192;
    uint8_t uart_buffer[uart_buffer_size];
    uint32_t uart_buffer_s, uart_buffer_e;
    uint64_t uart_cycles;

    void push_midi();
    
    std::mutex midi_mutex;


public:
    mt32_t();
    ~mt32_t();

    uint32_t button;
    int32_t knob;

    uint8_t rom[0x20000];
    uint8_t pcm[0x80000];
    uint8_t rvrom[0x8000];

    uint32_t lcd_buffer[lcd_h][lcd_w];

    int16_t samples[8192][2];

    void clock(uint64_t samples);
    void post_midi(uint8_t byte);

};
