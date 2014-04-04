#ifndef CC_PPM_H
#define CC_PPM_H

#include <string>
using namespace std;

#define LINEMAX 128

enum ppm_type {
	ppm_RGBA8 = 0,
	ppm_RGB8,
	ppm_RGBA4,
	ppm_RGB4,
	ppm_ALPHA8,
	ppm_ALPHA4
};

class cc_ppm {
	int _type;
	int _depth;
	int _step;
	int _width;
	int _height;
	void *_buffer;
public:
	cc_ppm();
	~cc_ppm();

	int load_texture(int id, string filename);
	int load_ppm(string filename);
	int save_ppm(string filename, ppm_type type, int width, int height, void* data);
private:
	char* readline(FILE *f, char* buffer);
	int ppm_header(FILE *f);
	int ppm_data(FILE *f, int id, int skipp);
	void loadppm_from_file(FILE *rgb, FILE *alpha);
	void ppm_type(string format);
	void save_rgb(string filename, int width, int height, int step, int depth);
	void save_alpha(string filename, int width, int height, int step, int depth, int offset);
};

#endif
