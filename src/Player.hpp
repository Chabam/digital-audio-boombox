#include "portaudio.h"
#include "AudioFile.hpp"
#include "sstream"
#include "string"
#define FRAMES_PER_BUFFER 256

class Player {
	PaStream* stream;
	AudioFile* audio_file;
	double current_time;
	enum STATUS {
		PLAYING,
		PAUSED,
		STOPPED
	};
	Player::STATUS status;

	static void initialize_portaudio();
	static int audio_loop(
		const void*,
		void*,
		unsigned long,
		const PaStreamCallbackTimeInfo*,
		PaStreamCallbackFlags,
		void*
	);
	int open_pa_stream();
	int start_pa_stream();
	void close_pa_stream();
	void abort_pa_stream();
	static std::string format_duration(long);

public:
	Player();
	~Player();
	void load_file(const char*);
	void play_pause();
	void pause();
	void play();
	void stop();
	void start();
	void fast_forward();
	void rewind();
	std::string print_info();
};
