#include "AudioFile.hpp"

AudioFile::AudioFile(const char* file_path) {
    this->file = sf_open(file_path, SFM_READ, &this->info);
}

AudioFile::~AudioFile() {
    sf_close(this->file);
}

sf_count_t AudioFile::read(float* output, sf_count_t frames) {
    return sf_read_float(this->file, output, frames);
}

void AudioFile::set_at_begining() {
    sf_seek(this->file, 0, SEEK_SET);
}

double AudioFile::get_duration() {
    return static_cast<double>(this->info.frames) / this->info.samplerate;
}