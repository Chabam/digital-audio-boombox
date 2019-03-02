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
	this->audio_file = nullptr;
	this->stream = nullptr;
	this->player_status = STOPPED;
}

Player::~Player() {
	PaError err = Pa_Terminate();
	if (err != paNoError) {
		std::cerr << "Error Terminating Portaudio!" << std::endl;
	}
	delete this->audio_file;
}

void Player::load_file(const char* file_name) {
	if (this->audio_file != nullptr) {
		delete this->audio_file;
	}

	if (this->stream != nullptr) {
		this->close_pa_stream();
	}

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
		this
	);

	if (err != paNoError) {
		std::cerr << "Error opening the Portaudio stream:" << std::endl;
		std::cerr << Pa_GetErrorText(err) << std::endl;
		return 1;
	}

	Pa_SetStreamFinishedCallback(this->stream, static_cast<PaStreamFinishedCallback*>([](void* user_data) {
		Player* player = static_cast<Player*>(user_data);
		player->stop();
	}));

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

void Player::play_pause() {
	if (this->player_status == PLAYING) {
		this->pause();
	} else if (this->player_status == PAUSED){
		this->play();
	} else {
		this->start();
	}
}

void Player::pause() {
	this->player_status = PAUSED;
}

void Player::play() {
	this->player_status = PLAYING;
}

void Player::stop() {
	this->player_status = STOPPED;
	if (this->audio_file == nullptr) {
		throw "No file loaded!";
	}
	this->audio_file->set_at_begining();
}

void Player::start() {
	if (this->audio_file == nullptr) {
		throw "No file loaded!";
	}
	int open_status = this->open_pa_stream();
	int start_status = this->start_pa_stream();
	this->player_status = PLAYING;

	if (open_status != 0 || start_status != 0) {
		this->close_pa_stream();
		return;
	}
}

int Player::audio_loop(
	const void*,
	void* output_buffer,
	unsigned long frames_per_buffer,
	const PaStreamCallbackTimeInfo*,
	PaStreamCallbackFlags,
	void* user_data
) {
	Player* player = static_cast<Player*>(user_data);
	float* output_channel = static_cast<float*>(output_buffer);

	int float_read = frames_per_buffer * player->audio_file->info.channels;
	std::fill(output_channel, output_channel + float_read, 0.0f);

	if (player->player_status == PLAYING) {
		unsigned long num_read = player->audio_file->read(output_channel, float_read);

		if (num_read < frames_per_buffer) {
			return paComplete;
		}
	}

	return paContinue;
}