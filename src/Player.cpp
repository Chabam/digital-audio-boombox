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
	this->status = STOPPED;
}

Player::~Player() {
	PaError err = Pa_Terminate();
	if (err != paNoError) {
		std::cerr << "Error Terminating Portaudio!" << std::endl;
	}
	delete this->audio_file;
}

void Player::load_file(const char* file_name) {
	AudioFile* new_audio_file;
	try {
		new_audio_file = new AudioFile(file_name);
	} catch (...) {
		throw;
	}

	if (this->status != STOPPED) {
		this->abort_pa_stream();
		delete this->audio_file;

	}

	this->audio_file = new_audio_file;
	this->current_time = 0;

	this->start();
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
		static_cast<Player*>(user_data)->stop();
	}));

	return 0;
}

void Player::abort_pa_stream() {
	PaError err = Pa_AbortStream(this->stream);

	if (err != paNoError) {
		std::cerr << "Error aborting the Portaudio stream:" << std::endl;
		std::cerr << Pa_GetErrorText(err) << std::endl;
	}
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
	if (this->status == PLAYING) {
		this->pause();
	} else if (this->status == PAUSED){
		this->play();
	} else {
		this->start();
	}
}

void Player::pause() {
	this->status = PAUSED;
}

void Player::play() {
	this->status = PLAYING;
}

void Player::stop() {
	this->status = STOPPED;
	if (this->audio_file == nullptr) {
		throw "No file loaded!";
	}
	this->audio_file->set_at_begining();
	this->current_time = 0;
}

void Player::start() {
	if (this->audio_file == nullptr) {
		throw "No file loaded!";
	}
	int open_status = this->open_pa_stream();
	int start_status = this->start_pa_stream();
	this->status = PLAYING;

	if (open_status != 0 || start_status != 0) {
		this->close_pa_stream();
		return;
	}
}

void Player::fast_forward() {
	if (this->audio_file == nullptr) {
		throw "No file loaded!";
	}

	if (this->current_time < this->audio_file->get_duration()) {
		this->current_time += 1;
	}
}

void Player::rewind() {
	if (this->audio_file == nullptr) {
		throw "No file loaded!";
	}

	if (this->current_time > 0) {
		this->current_time -= 1;
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
	sf_seek(player->audio_file->file, player->current_time * player->audio_file->info.samplerate, SEEK_SET);

	int float_read = frames_per_buffer * player->audio_file->info.channels;
	std::fill(output_channel, output_channel + float_read, 0.0f);

	if (player->status == PLAYING) {
		player->current_time += static_cast<double>(frames_per_buffer) / player->audio_file->info.samplerate;
		unsigned long num_read = player->audio_file->read(output_channel, float_read);

		if (num_read < frames_per_buffer) {
			return paComplete;
		}

	}

	return paContinue;
}

std::string Player::print_info() {
	if (this->audio_file == nullptr) {
		return "No tack selected!";
	}
	std::ostringstream output;
	output << "Currently playing : " << this->audio_file->path << std::endl;
	output << "Status : ";
	switch (this->status) {
		case PLAYING:
			output << "playing" << std::endl;
			break;
		case PAUSED:
			output << "paused" << std::endl;
			break;
		case STOPPED:
			output << "stopped" << std::endl;
			break;
	}

	if (this->status != STOPPED) {
		output << Player::format_duration(this->current_time)
			<< "/"
			<< Player::format_duration(this->audio_file->get_duration())
			<< std::endl;
	}

	return output.str();
}

std::string Player::format_duration(long seconds) {
	std::ostringstream output;
	int minutes = seconds / 60;
	int hours = minutes / 60;

	if (hours != 0) {
		output << hours << "h ";
	}

	if (minutes != 0) {
		output << minutes << "m ";
	}

	output << seconds - (minutes * 60) - (hours * 60 * 60) << "s";

	return output.str();
}