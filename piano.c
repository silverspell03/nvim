#define SDL_MAIN_HANDLED 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL3/SDL.h>
#include <math.h>



static void SDLCALL GenerateSineWave(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount)
{
    SDL_AudioSpec spec;
    if (additional_amount <= 0) return;

    int bytes_per_sample = sizeof(float);
    int samples = additional_amount / bytes_per_sample;

    if (additional_amount % bytes_per_sample != 0) return;
    
    float *buf = malloc(samples * sizeof(float));
    if (!buf) return;
    
    SDL_GetAudioStreamFormat(stream, &spec, NULL);

    int freq = spec.freq;
    
    for (int i = 0; i < samples; i++)
    {
        // un La c'est 440 vibrations par secondes, 440 fois par sec où le nombre vaut 1.0
        // Avec 44100 samples par seconde, PI*2 pour l'entierete de la plage des sinus, de -1  a 1  
        float r = sin(M_PI * 2 * 440 * i / 44100 );
        buf[i] = r;
    }

    SDL_PutAudioStreamData(stream, buf, additional_amount);
}

static void VisualizeData(const void *buf, int len)
{

}

static void SDLCALL GenerateNoise(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount)
{
    if (additional_amount <= 0) return;

    int bytes_per_sample = sizeof(float);
    int samples = additional_amount / bytes_per_sample;

    if (additional_amount % bytes_per_sample != 0) return;
    
    float *buf = malloc(samples * sizeof(float));
    if (!buf) return;

    for (int i = 0; i < samples; i++)
    {
        float r = (float)rand() / (float)RAND_MAX;
        buf[i] = (r * 2.0f - 1.0f) * 0.2f;  /* 0.2 = volume */
    }

    SDL_PutAudioStreamData(stream, buf, additional_amount);
}

static void InitAudio()
{
    srand(time(NULL));  // seed with current time

    if (!SDL_Init(SDL_INIT_AUDIO, SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_INIT failed :%s\n", SDL_GetError());
    };
    SDL_AudioSpec spec;
    spec.format = SDL_AUDIO_F32;   /* float32 samples */
    spec.channels = 1;
    spec.freq = 44100;
}

static void InitVideo() {
    srand(time(NULL));  // seed with current time

    if (!SDL_Init(SDL_INIT_AUDIO, SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_INIT failed :%s\n", SDL_GetError());
    };
    SDL_AudioSpec spec;
    spec.format = SDL_AUDIO_F32;   /* float32 samples */
    spec.channels = 1;
    spec.freq = 44100;
}

int main()
{

    /* Open a playback stream with a callback that will feed the stream */
    SDL_AudioStream *stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, /* device id (default playback) */
        &spec,                             /* our app format */
        GenerateNote,                              /* callback to supply data */
        NULL                               /* userdata */
    );
	
    if (!stream) {
        fprintf(stderr, "SDL_OpenAudioDeviceStream failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    /* Start playback of the stream's device */
    SDL_ResumeAudioStreamDevice(stream);

    /* let it play for ~5 seconds */
    SDL_Delay(2000);

    /* cleanup */
    SDL_DestroyAudioStream(stream); /* closes device too */
    SDL_Quit();
    return 0;
}
