/*
 * Copyright 2020~2021 SYSON, MICHAEL B.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *     
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @company: USBONG
 * @author: VIDAL, ERIC E. (2006)
 * @editor: SYSON, MICHAEL B. (PRESENT)
 * @date created: 2006
 * @date updated: 20211004
 * @website address: http://www.usbong.ph
 *
 * Reference: 
 * 1) Astle, D. and Hawkin, K. (2004). "Beginning OpenGL game programming". USA: Thomson Course Technology
 *
 * Acknowledgments:
 * 1) "Bulalakaw Wars" Team (2007): 
 * Syson, M., Camacho, R., Gonzales, D., Del Rosario, R., Vidal, E., et al.
 * 
 */

/**
 * This program demonstrates how to play sound clips.
 * Looping and volume control are supported.
 *
 * Eric Vidal
 */

#include "Sound.h"

/* array of sound clip pointers to keep track of running sounds */
SoundClip *sound_clip_ptrs[MAX_SOUND_CLIPS];

/* contains device initialization info */
SDL_AudioSpec desired, actual;

void sound_callback(void *user, unsigned char *bytestream, int length)
{
    /* this function mixes multiple sound sources; it is not meant
     * to be called directly! */

    int i, j;
    signed short *stream = (signed short *) bytestream;

    /* for all buffer samples... */
    for (j = 0; j < actual.samples; j++)
    {
        signed int leftacc = 0, rightacc = 0;

        /* for all playing clips... */
        for (i = 0; i < MAX_SOUND_CLIPS; i++)
        {
            signed int left, right;

            /* skip empty array cells */
            if (sound_clip_ptrs[i] == NULL)
                continue;

            /* get the sound sample */
            left = sound_clip_ptrs[i]->data[sound_clip_ptrs[i]->position * 2];
            right = sound_clip_ptrs[i]->data[sound_clip_ptrs[i]->position * 2 + 1];

            /* modify volume of sample */
            left = left * sound_clip_ptrs[i]->left_volume / 100;
            right = right * sound_clip_ptrs[i]->right_volume / 100;
            
            /* update our current position in the sound clip */
            sound_clip_ptrs[i]->position++;

            /* if we are at the end... */
            if (sound_clip_ptrs[i]->position == sound_clip_ptrs[i]->length)
            {
                sound_clip_ptrs[i]->position = 0;
                if (! sound_clip_ptrs[i]->looping)
                {
                    /* if the sound is not supposed to loop, remove it
                     * from our array */
                    sound_clip_ptrs[i] = NULL;
                }
            }

            /* add to accumulators */
            leftacc += left;
            rightacc += right;
        }

        /* clip final values to avoid distortion */
        if (leftacc > 32767) leftacc = 32767;
        if (leftacc < -32767) leftacc = -32767;
        if (rightacc > 32767) rightacc = 32767;
        if (rightacc < -32767) rightacc = -32767;

        /* save final values */
        stream[j * 2] = leftacc;
        stream[j * 2 + 1] = rightacc;
    }
}

void Sound::sound_start()
{
    /* make sure we start with a clean slate */
    memset(sound_clip_ptrs, 0, sizeof(sound_clip_ptrs));

    /* open audio device */
    desired.freq = 44100;
    desired.format = AUDIO_S16SYS;
    desired.channels = 2;
    desired.samples = 512;
    desired.callback = sound_callback;
    SDL_InitSubSystem(SDL_INIT_AUDIO);
    if (SDL_OpenAudio(&desired, &actual) < 0 ||
        actual.format != AUDIO_S16SYS || actual.channels != 2)
    {
        /* audio is not available (or audio format is not supported);
         * we simply terminate here in this example, but ideally we
         * should allow the game to continue without sound */
        SDL_Quit();
        exit(1);
    }
    SDL_PauseAudio(0);  /* audio is paused until we unpause it */
}

void Sound::sound_stop()
{
    /* close audio device */
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void Sound::play_sound_clip(SoundClip *sound_clip_ptr)
{
    SDL_LockAudio();

    /* find an unused slot in our array of playing sounds;
     * actual playback is handled in a separate function */
    int i;
    for (i = 0; i < MAX_SOUND_CLIPS; i++)
    {
        if (sound_clip_ptrs[i] == NULL || sound_clip_ptrs[i] == sound_clip_ptr)
        {
            sound_clip_ptrs[i] = sound_clip_ptr;
            break;
        }       
    }

    SDL_UnlockAudio();
}

void Sound::stop_sound_clip(SoundClip *sound_clip_ptr)
{
    SDL_LockAudio();

    /* find the sound clip in our array of playing sounds and remove it */
    int i;
    for (i = 0; i < MAX_SOUND_CLIPS; i++)
    {
        if (sound_clip_ptrs[i] == sound_clip_ptr)
        {
            sound_clip_ptrs[i]->position = 0;
            sound_clip_ptrs[i] = NULL;
            break;
        }
    }

    SDL_UnlockAudio();
}

SoundClip* Sound::load_sound_clip(char *filename)
{
    SDL_AudioSpec audiospec;
    unsigned char *data;
    unsigned int length;
    SDL_AudioCVT converter;
    SoundClip *sound_clip_ptr;

    SDL_LockAudio();

    /* load the WAV file */
    if (SDL_LoadWAV(filename, &audiospec, &data, &length) == NULL)
    {
        SDL_UnlockAudio();
        return NULL;
    }
    
    /* create a converter in case the file is not in the desired format */
    if (SDL_BuildAudioCVT(&converter,
                          audiospec.format, audiospec.channels, audiospec.freq,
                          actual.format, actual.channels, actual.freq) < 0)
    {
        SDL_FreeWAV(data);
        SDL_UnlockAudio();
        return NULL;
    }

    /* create the struct to hold the final sound clip
     * and copy the sound data over */
    sound_clip_ptr = (SoundClip *) malloc(sizeof(SoundClip));
    sound_clip_ptr->data = (signed short *) malloc(length * converter.len_mult);
    memcpy(sound_clip_ptr->data, data, length);
    SDL_FreeWAV(data);

    /* convert the audio */
    converter.buf = (unsigned char *) sound_clip_ptr->data;
    converter.len = length;
    if (SDL_ConvertAudio(&converter))
    {
        /* shouldn't happen */
        free(sound_clip_ptr->data);
        free(sound_clip_ptr);
        SDL_UnlockAudio();
        return NULL;
    }

    /* initialize other parts of the sound clip struct */
    sound_clip_ptr->looping = 0;
    sound_clip_ptr->left_volume = 100;
    sound_clip_ptr->right_volume = 100;
    sound_clip_ptr->length = length * converter.len_ratio / 4;
    sound_clip_ptr->position = 0;

    SDL_UnlockAudio();
    return sound_clip_ptr;
}

void Sound::unload_sound_clip(SoundClip *sound_clip_ptr)
{
    SDL_LockAudio();
    stop_sound_clip(sound_clip_ptr);
    free(sound_clip_ptr->data);
    free(sound_clip_ptr);
    SDL_UnlockAudio();
}
