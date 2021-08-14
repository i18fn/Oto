#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <mmsystem.h>
#include <math.h>

#include "sound_data.h"
#include "oscillator/oscillator.h"

#define NUMBER_OF_BUFFER 8
#define BUFFER_SIZE      200

#define MONO   1
#define STEREO 2

#define MAX_VOLUME            5000.0
#define QUANTIZATION_BIT_RATE 16
#define SOUND_DATA_BYTE       2

void play_sound(SOUND s, int32_t sampling_freq, uint8_t velocity) {
    int16_t out_buffer[NUMBER_OF_BUFFER][BUFFER_SIZE];

    WAVEHDR out_header[NUMBER_OF_BUFFER];
    for (int32_t i = 0; i < NUMBER_OF_BUFFER; i++) {
        memset(&out_header[i], 0, sizeof(WAVEHDR));
    }

    HWAVEOUT out_handle = NULL;
    WAVEFORMATEX wave_format_ex = {
        WAVE_FORMAT_PCM,
        MONO,
        sampling_freq,
        sampling_freq * SOUND_DATA_BYTE,
        SOUND_DATA_BYTE,
        QUANTIZATION_BIT_RATE,
        0
    };
    waveOutOpen(&out_handle, 0, &wave_format_ex, 0, 0, CALLBACK_NULL);
    waveOutPause(out_handle);

    int32_t out0 = 0;  // 何番目のバッファを書き込んでるのかを指す
    int32_t out1 = 0;
    int32_t offset = 0;

    double  out_volume = ((double)velocity / 255) * MAX_VOLUME;
    int32_t num_of_frame = s->length / BUFFER_SIZE;
    int32_t frame = 0;
    while (frame < num_of_frame) {
        if (out0 < NUMBER_OF_BUFFER) {
            // バッファへの1frame分の書き込み
            for (int32_t n = 0; n < BUFFER_SIZE; n++) {
                out_buffer[out0][n] = (int16_t)(out_volume * s->data[offset + n]);
            }
            offset += BUFFER_SIZE;
            frame++;

            // バッファの設定
            out_header[out0].lpData = (int8_t *)out_buffer[out0];  // 音データの場所
            out_header[out0].dwBufferLength = BUFFER_SIZE * 2;  // buffer_size * 2[byte](int16)
            out_header[out0].dwFlags = 0;

            // バッファのロック
            waveOutPrepareHeader(out_handle, &out_header[out0], sizeof(WAVEHDR));

            // バッファを出力待ちキューに追加する
            waveOutWrite(out_handle, &out_header[out0], sizeof(WAVEHDR));

            out0++;
            if (out0 == NUMBER_OF_BUFFER) {
                waveOutRestart(out_handle);
            }

        } else if ((out_header[out1].dwFlags & WHDR_DONE) != 0) {
            // 出力バッファのおわりまで音データが再生された
            for (int32_t n = 0; n < BUFFER_SIZE; n++) {
                out_buffer[out1][n] = (int16_t)(out_volume * s->data[offset + n]);
            }
            offset += BUFFER_SIZE;
            frame++;

            waveOutPrepareHeader(out_handle, &out_header[out1], sizeof(WAVEHDR));

            // バッファの設定
            out_header[out1].lpData = (int8_t *)out_buffer[out1];  // 音データの場所
            out_header[out1].dwBufferLength = BUFFER_SIZE * 2;  // buffer_size * 2[byte](int16)
            out_header[out1].dwFlags = 0;

            // バッファのロック
            waveOutPrepareHeader(out_handle, &out_header[out1], sizeof(WAVEHDR));

            // バッファを出力待ちキューに追加する
            waveOutWrite(out_handle, &out_header[out1], sizeof(WAVEHDR));

            out1++;
            if (out1 == NUMBER_OF_BUFFER) out1 = 0;
        }
    }

    for (out0 = 0; out0 < NUMBER_OF_BUFFER; out0++) {
        while ((out_header[out0].dwFlags & WHDR_DONE) == 0) {
            Sleep(1);
        }
    }

    waveOutPause(out_handle);

    for (out0 = 0; out0 < NUMBER_OF_BUFFER; out0++) {
        if ((out_header[out0].dwFlags & WHDR_PREPARED) != 0) {
            waveOutUnprepareHeader(out_handle, &out_header[out0], sizeof(WAVEHDR));
        }
    }

    waveOutClose(out_handle);
}

void play(double freq, double second, uint8_t velocity, 
          int32_t wave, int32_t channel, int32_t sampling_freq) {
    uint64_t length = (uint64_t)(second * sampling_freq);
    SOUND s = new_sound(length);

    write_wave(wave, s, freq, length, sampling_freq, 1, 1);
    play_sound(s, sampling_freq, velocity);

    free_sound(s);
}

int main(void) {
    // gcc sound_io.c sound_data.c oscillator/wave.c oscillator/oscillator.c -O2 -lwinmm

    play(269.292, 0.75,  40, PSG_TRIANGLE_WAVE, 1, 20000);
    play(302.270, 0.75,  80, PSG_TRIANGLE_WAVE, 1, 20000);
    play(339.286, 0.75, 120, PSG_TRIANGLE_WAVE, 1, 20000);
    play(359.461, 0.75, 160, PSG_TRIANGLE_WAVE, 1, 20000);
    play(403.482, 0.75, 200, PSG_TRIANGLE_WAVE, 1, 20000);
    play(452.893, 0.75, 220, PSG_TRIANGLE_WAVE, 1, 20000);
    play(508.355, 0.75, 255, PSG_TRIANGLE_WAVE, 1, 20000);

    return 0;
}