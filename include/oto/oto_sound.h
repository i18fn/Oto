#pragma once

#include <math.h>
#include <unistd.h>
#include <portaudio.h>

// #define MAX_POLYPHONIC 8
#define MAX_POLYPHONIC 1
#define FRAMES_PER_BUFFER 128
#define MAX_VOLUME 100

#define MONO_CH   1
#define STEREO_CH 2

#define PI 3.14159265358979323846
#define M_SQRT2   1.41421356237309504880
#define M_SQRT1_2 0.70710678118654752440

typedef enum {
    SINE_WAVE = 0,
    SAWTOOTH_WAVE,  // PSG
    SQUARE_WAVE,    // PSG
    TRIANGLE_WAVE,  // PSG
    WHITE_NOISE,    // PSG
} basicwave_t;

/* 発振器 */
typedef struct oscillator {
    basicwave_t wave;
    // struct oscillator *fm;
    // struct oscillator *am;

    // 妥協したFM音源
    basicwave_t fm_wave;
    float fm_freq;
} Oscillator;

/* 音色情報 */
typedef struct {
    Oscillator *oscillator;
    VectorPTR filters;
} Sound;

/* 演奏情報 */
typedef struct {
    Sound *sound;
    uint64_t length;
    float freq[MAX_POLYPHONIC];
    int8_t volume;
} Playdata;

typedef enum {
    CLIP = 0,
    FADE_IN,
    FADE_OUT,
    FADE,
    AMP,
    TREMOLO
} filtercode_t;

/* フィルタ */
typedef struct {
    filtercode_t num;
    VectorDouble *args;
} Filter;

void init_sound_stream(int64_t sampling_rate, double fade_range);
void terminate_sound_stream();

bool is_stream_active();
void set_stream_active_flag(bool b);

void write_out_data(Playdata data);
