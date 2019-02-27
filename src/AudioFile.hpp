#include "sndfile.h"
class AudioFile {
public:
    SNDFILE* file;
    SF_INFO info;
    AudioFile() = default;
    AudioFile(const char*);
    ~AudioFile();
    sf_count_t read(float*, sf_count_t);
};