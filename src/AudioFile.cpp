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