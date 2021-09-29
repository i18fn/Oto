#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "track.h"
#include "oscillator.h"

#define PI 3.14159265358979323846

static int32_t SAMPLING_FREQ = 20000;
void set_wave_sampling_freq(int32_t fs) {
    SAMPLING_FREQ = fs;
}

double sine_wave_func(int32_t n, double freq, double phase) {
    return sin((2.0 * PI * freq * n / (double)SAMPLING_FREQ) + phase);
}

double sawtooth_wave_func(int32_t n, double freq, uint32_t arity) {
    double s = 0;
    for (uint32_t i = 1; i <= arity; i++) {
        s += 1.0 / (double)i * sine_wave_func(n, i * freq, 0);
    }
    return s;
}

double square_wave_func(int32_t n, double freq, uint32_t arity) {
    double s = 0;
    for (uint32_t i = 1; i <= arity; i = i + 2) {
        s += 1.0 / (double)i * sine_wave_func(n, i * freq, 0);
    }
    return s;
}

double triangle_wave_func(int32_t n, double freq, uint32_t arity) {
    double s = 0;
    for (uint32_t i = 1; i <= arity; i = i + 2) {
        s += 1.0 / i / i * sin(PI * i / 2.0) * sine_wave_func(n, i * freq, 0);
    }
    return s;    
}

double psg_sawtooth_wave_func(int32_t n, double freq) {
    int32_t t0 = SAMPLING_FREQ / freq;
    int32_t m = n % t0;

    return 1.0 - 2.0 * m / t0;
}

double psg_square_wave_func(int32_t n, double freq) {
    int32_t t0 = SAMPLING_FREQ / freq;
    int32_t m = n % t0;

    if (m < t0 / 2) {
        return 1.0;
    } else {
        return -1.0;
    }
}

double psg_triangle_wave_func(int32_t n, double freq) {
    int32_t t0 = SAMPLING_FREQ / freq;
    int32_t m = n % t0;

    if (m < t0 / 2) {
        return -1.0 + 4.0 * m / t0;
    } else {
        return  3.0 - 4.0 * m / t0;
    }
}

double white_noise_func(int32_t n, uint32_t arity) {
    double s = 0;
    for (uint32_t i = 1; i <= arity; i++) {
        double phase = (double)rand() / RAND_MAX * 2.0 * PI;
        s += sine_wave_func(n, (double)i, phase);
    }
    return s;
}

/* 正弦波を足し合わせる回数 */
static int32_t ARITY = 50;

void sine_wave(TRACK t, double freq, uint64_t length) {
    for (uint64_t i = 0; i < length; i++) {
        t->data[t->ptr + i] += sine_wave_func(i, freq, 0);
    }
}

void sawtooth_wave(TRACK t, double freq, uint64_t length) {
    for (uint64_t i = 0; i < length; i++) {
        t->data[t->ptr + i] += sawtooth_wave_func(i, freq, ARITY);
    }
}

void square_wave(TRACK t, double freq, uint64_t length) {
    for (uint64_t i = 0; i < length; i++) {
        t->data[t->ptr + i] += square_wave_func(i, freq, ARITY);
    }
}

void triangle_wave(TRACK t, double freq, uint64_t length) {
    for (uint64_t i = 0; i < length; i++) {
        t->data[t->ptr + i] += triangle_wave_func(i, freq, ARITY);
    }
}

void psg_sawtooth_wave(TRACK t, double freq, uint64_t length) {
    for (uint64_t i = 0; i < length; i++) {
        t->data[t->ptr + i] += psg_sawtooth_wave_func(i, freq);
    }
}

void psg_square_wave(TRACK t, double freq, uint64_t length) {
    for (uint64_t i = 0; i < length; i++) {
        t->data[t->ptr + i] += psg_square_wave_func(i, freq);
    }
}

void psg_triangle_wave(TRACK t, double freq, uint64_t length) {
    for (uint64_t i = 0; i < length; i++) {
        t->data[t->ptr + i] += psg_triangle_wave_func(i, freq);
    }
}

void white_noise(TRACK t, uint64_t length) {
    for (uint64_t i = 0; i < length; i++) {
        t->data[t->ptr + i] += white_noise_func(i, ARITY);
    }
}

/* フェードを掛ける幅 */
static double FADE_RANGE = 0.05;

void write_wave(int32_t wave, 
                TRACK t, double freq, uint64_t length,
                int32_t sampling_freq, bool fade_flag, bool seek_cursol_flag) {
    set_wave_sampling_freq(sampling_freq);
    
    if ((t->ptr + length) > t->length) {
        // 音データの長さを超えてしまう場合
        length = t->length - t->ptr;
        if (length == 0) return;
    }

    switch (wave) {
    case SINE_WAVE:
        sine_wave(t, freq, length);
        break;
    case SAWTOOTH_WAVE:
        sawtooth_wave(t, freq, length);
        break;
    case SQUARE_WAVE:
        square_wave(t, freq, length);
        break;
    case TRIANGLE_WAVE:
        triangle_wave(t, freq, length);
        break;
    case PSG_SAW_WAVE:
        psg_sawtooth_wave(t, freq, length);
        break;
    case PSG_SQUARE_WAVE:
        psg_square_wave(t, freq, length);
        break;
    case PSG_TRIANGLE_WAVE:
        psg_triangle_wave(t, freq, length);
        break;
    case WHITE_NOISE:
        white_noise(t, length);
        break;
    }

    /* フェード処理 */
    if (fade_flag) {
        for (uint64_t n = 0; n < (sampling_freq * FADE_RANGE); n++) {
            t->data[t->ptr + n] *= (double)n / (sampling_freq * FADE_RANGE);
            t->data[t->ptr + length - n - 1] *= (double)n / (sampling_freq * FADE_RANGE);
        }        
    }

    if (seek_cursol_flag) t->ptr += length;
}