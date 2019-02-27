#include "portaudio.h"
#include "AudioFile.hpp"
#define FRAMES_PER_BUFFER 256
#define SAMPLE_RATE 44100

class Player {
	PaStream* stream;
	AudioFile* audio_file;
	static void initialize_portaudio();
	static int audio_loop(
		const void*,
		void*,
		unsigned long,
		const PaStreamCallbackTimeInfo*,
		PaStreamCallbackFlags,
		void*
	);
	void load_file(const char*);
	int open_pa_stream();
	int start_pa_stream();
	void close_pa_stream();

public:
	Player();
	~Player();
	void play_file(char*);
};
