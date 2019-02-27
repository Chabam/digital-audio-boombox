#include "Player.hpp"
#include "iostream"
#include "algorithm"

void Player::initialize_portaudio() {
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		std::cerr << "Error initalizing Portaudio!" << std::endl;
	}
}

Player::Player() {
	Player::initialize_portaudio();
	this->audio_file = new AudioFile();
}

Player::~Player() {
	PaError err = Pa_Terminate();
	if (err != paNoError) {
		std::cerr << "Error Terminating Portaudio!" << std::endl;
	}
	delete this->audio_file;
}

void Player::load_file(const char* file_name) {
	delete this->audio_file;
	this->audio_file = new AudioFile(file_name);
}

int Player::open_pa_stream() {
	PaError err = Pa_OpenDefaultStream(
		&this->stream,
		0,
		this->audio_file->info.channels,
		paFloat32,
		this->audio_file->info.samplerate,
		FRAMES_PER_BUFFER,
		Player::audio_loop,
		this->audio_file
	);

	if (err != paNoError) {
		std::cerr << "Error opening the Portaudio stream:" << std::endl;
		std::cerr << Pa_GetErrorText(err) << std::endl;
		return 1;
	}
	return 0;
}

void Player::close_pa_stream() {
	PaError err = Pa_CloseStream(this->stream);

	if (err != paNoError) {
		std::cerr << "Error closing the Portaudio stream:" << std::endl;
		std::cerr << Pa_GetErrorText(err) << std::endl;
	}
}

int Player::start_pa_stream() {
	PaError err = Pa_StartStream(this->stream);

	if (err != paNoError) {
		std::cerr << "Error starting the Portaudio stream:" << std::endl;
		std::cerr << Pa_GetErrorText(err) << std::endl;
		return 1;
	}

	return 0;
}

void Player::play_file(char* file_path) {
	Player::load_file(file_path);
	int open_status = this->open_pa_stream();
	int start_status = this->start_pa_stream();

	if (open_status != 0 || start_status != 0) {
		this->close_pa_stream();
		return;
	}

	while(Pa_IsStreamActive(this->stream)) {
		Pa_Sleep(100);
	}

	std::cout << "Done playing!" << std::endl;
}

int Player::audio_loop(
	const void*,
	void* output_buffer,
	unsigned long frames_per_buffer,
	const PaStreamCallbackTimeInfo*,
	PaStreamCallbackFlags,
	void* user_data
) {
	AudioFile* audio_file = static_cast<AudioFile*>(user_data);
	float* output_channel = static_cast<float*>(output_buffer);

	int float_read = frames_per_buffer * audio_file->info.channels;
	std::fill(output_channel, output_channel + float_read, 0.0f);

	unsigned long num_read = audio_file->read(output_channel, float_read);

	if (num_read < frames_per_buffer) {
		return paComplete;
	}

	return paContinue;
}