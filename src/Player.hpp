#include "sndfile.hh"
#include "portaudio.h"
#define FRAMES_PER_BUFFER 256
#define SAMPLE_RATE 44100

class Player {
PaStream* stream;
static void initialize_portaudio();
static int audio_loop(
	const void*,
	void*,
	unsigned long,
	const PaStreamCallbackTimeInfo*,
	PaStreamCallbackFlags*,
	void*
);
static SndfileHandle open_file(char*);
int open_pa_stream(SndfileHandle);
int start_pa_stream();
void close_pa_stream();

public:
	Player();
	~Player();
	void play_file(char*);
};
