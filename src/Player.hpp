#include "sndfile.h"
#include "portaudio.h"
#define FRAMES_PER_BUFFER 256
#define SAMPLE_RATE 44100

class Player {
	struct FileInfo {
		SNDFILE* file;
		SF_INFO info;
	} file_info;

	PaStream* stream;
	static void initialize_portaudio();
	static int audio_loop(
		const void*,
		void*,
		unsigned long,
		const PaStreamCallbackTimeInfo*,
		PaStreamCallbackFlags,
		void*
	);
	void open_file(char*);
	void close_file();
	int open_pa_stream();
	int start_pa_stream();
	void close_pa_stream();

public:
	Player();
	~Player();
	void play_file(char*);
};
