#include "sndfile.h"
#include "string"
#include "sstream"

class AudioFile {
public:
	SNDFILE* file;
	SF_INFO info;
	const char* path;
	AudioFile(const char*);
	AudioFile();
	~AudioFile();
	sf_count_t read(float*, sf_count_t);
	void set_at_begining();
	double get_duration();
};