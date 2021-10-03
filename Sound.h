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

#include <stdlib.h>

#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "SoundClip.h"

#define MAX_SOUND_CLIPS 16

/* List of sound files */
#define BEAMSOUND "sounds/beam.wav"
#define THRUSTSOUND "sounds/thrust.wav"
#define EXPLOSIONSOUND "sounds/explosion.wav"
#define RESETSOUND "sounds/zing.wav"

//#define BONUSSOUND "sounds/chimes.wav"
#define BONUSSOUND "sounds/extrapac.wav"
#define TITLESOUND "sounds/title.wav"

class Sound {
      public:
       void sound_start();
       void sound_stop();
       void play_sound_clip(SoundClip *sound_clip_ptr);
       void stop_sound_clip(SoundClip *sound_clip_ptr);
       SoundClip *load_sound_clip(char *filename);
       void unload_sound_clip(SoundClip *sound_clip_ptr);
};

