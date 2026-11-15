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
#include <stdio.h>
#include <Windows.h>
#include <gl\GL.h>
#include <SDL.h>
//#include "GL\gl3w.h"
#include "mt32.h"
#include "midi.h"

SDL_Window* window;
SDL_GLContext gl_context;

mt32_t mt32;

void mt32_callback(void*, Uint8* stream, int len)
{
    mt32.clock(len / 4);
    memcpy(stream, mt32.samples, len);
}

int main(int argc, char **argv)
{
    FILE* f;

    f = fopen("mt32_cpu.bin", "rb");

    if (!f)
        return 0;

    if (fread(mt32.rom, 1, 0x20000, f) != 0x20000)
        return 0;

    fclose(f);

    f = fopen("mt32_pcm.bin", "rb");

    if (!f)
        return 0;

    if (fread(mt32.pcm, 1, 0x80000, f) != 0x80000)
        return 0;

    fclose(f);

    f = fopen("mt32_reverb.bin", "rb");

    if (!f)
        return 0;

    if (fread(mt32.rvrom, 1, 0x8000, f) != 0x8000)
        return 0;

    fclose(f);

    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    int w = lcd_w;
    int h = lcd_h;

    window = SDL_CreateWindow("MT-32", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window)
        return 0;

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);

    uint64_t starttic = SDL_GetTicks64();
    uint64_t otic = 0;

    bool quit = false;

    mt32.button = 0;
    //mt32.button = (1 << MT32_BUTTTON_4) | (1 << MT32_BUTTTON_RHYTHM) | (1 << MT32_BUTTTON_MASTER_VOLUME);
    //mt32.button = (1 << MT32_BUTTTON_MASTER_VOLUME);

    GLuint lcd_tex;

    glGenTextures(1, &lcd_tex);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, lcd_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_2D, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    MIDI_Init(0);

    SDL_AudioSpec spec = {};
    spec.freq = 32000;
    spec.format = AUDIO_S16;
    spec.channels = 2;
    spec.samples = 1024;
    spec.callback = mt32_callback;

    SDL_AudioSpec spec_actual = {};
    auto dev = SDL_OpenAudioDevice(nullptr, 0, &spec, &spec_actual, 0);

    if (!dev)
        return 0;

    SDL_PauseAudioDevice(dev, 0);

    while (!quit)
    {
#if 0
        uint64_t t = SDL_GetTicks64() - starttic;

        if (t - otic > 1000)
        {
            t = otic;
            starttic = SDL_GetTicks64() - t;
        }

        otic = t;

        t *= 8192;
        mt32.clock(t);
#endif

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lcd_w, lcd_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, mt32.lcd_buffer);

        glViewport(0, 0, w, h);

        glBegin(GL_QUADS);

        glTexCoord2f(0, 1); glVertex2f(-1, -1);
        glTexCoord2f(0, 0); glVertex2f(-1, 1);
        glTexCoord2f(1, 0); glVertex2f(1, 1);
        glTexCoord2f(1, 1); glVertex2f(1, -1);

        glEnd();

        glFinish();
        SDL_GL_SwapWindow(window);

        SDL_Delay(5);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.scancode == SDL_SCANCODE_MINUS)
                    {
                        mt32.knob -= 10;
                        if (mt32.knob < 0)
                            mt32.knob = 0;
                        printf("knob: %i\n", mt32.knob);
                        break;
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_EQUALS)
                    {
                        mt32.knob += 10;
                        if (mt32.knob > 1023)
                            mt32.knob = 1023;
                        printf("knob: %i\n", mt32.knob);
                        break;
                    }
                    __fallthrough;
                case SDL_KEYUP:
                {
                    int bit = -1;

                    if (event.key.keysym.scancode >= SDL_SCANCODE_1 && event.key.keysym.scancode <= SDL_SCANCODE_0)
                        bit = event.key.keysym.scancode - SDL_SCANCODE_1;

                    if (bit >= 0)
                    {
                        if (event.type == SDL_KEYDOWN)
                            mt32.button |= 1 << bit;
                        else
                            mt32.button &= ~(1 << bit);
                    }

                    break;
                }
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        w = event.window.data1;
                        h = event.window.data2;
                    }
                    break;
            }
        }
    }

    MIDI_Quit();

    return 0;
}

