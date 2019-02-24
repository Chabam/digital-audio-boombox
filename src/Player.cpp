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
}

Player::~Player() {
	PaError err = Pa_Terminate();
	if (err != paNoError) {
		std::cerr << "Error Terminating Portaudio!" << std::endl;
	}
}

SndfileHandle Player::open_file(char* file_name) {
	return SndfileHandle(file_name, SFM_READ, paFloat32, 2, SAMPLE_RATE);
}

int Player::open_pa_stream(SndfileHandle file) {
	PaError err = Pa_OpenDefaultStream(
		&this->stream,
		0,
		file.channels(),
		paFloat32,
		file.samplerate(),
		FRAMES_PER_BUFFER,
		reinterpret_cast<PaStreamCallback*>(&(Player::audio_loop)),
		static_cast<void*>(&file)
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

void Player::play_file(char* file) {
	SndfileHandle file_handle = Player::open_file(file);
	int open_status = this->open_pa_stream(file);
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
	void* output,
	unsigned long framecount,
	const PaStreamCallbackTimeInfo*,
	PaStreamCallbackFlags*,
	void* file
) {
	SndfileHandle* file_handle = static_cast<SndfileHandle*>(file);
	float* output_chanel = static_cast<float*>(output);
	int float_read = framecount * file_handle->channels();
	std::fill(output_chanel, output_chanel + float_read, 0.0f);
	unsigned long num_read = file_handle->readf(output_chanel, float_read);
	if (num_read < framecount) {
		return paComplete;
	}

	return paContinue;
}