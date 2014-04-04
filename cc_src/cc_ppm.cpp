#include "cc_ppm.h"
#include <string>
using namespace std;
//class cc_ppm {
//public:
cc_ppm::cc_ppm() {

}
cc_ppm::~cc_ppm() {
	
}

int cc_ppm::load_texture(int id, string filename) {
	string tmp = filename + ".ppm";
	FILE* rgb = fopen(tmp, "rb");
	tmp = filename + ".pgm";
	FILE* alpha = fopen(tmp, "rb");
	if (rgb == NULL && alpha == NULL) {
		return -1;
		cout << "Can't open " << filename << ".ppm/ .pgm" << endl;
	}
	int ok = loadppm_from_file(rgb, alpha);
	if (rgb)
		fclose(rgb);
	if (alpha)
		fclose(alpha);
	if (!ok) {
		if (_buffer) {
			free(_buffer);
		}		
		cout << "Invalid file " << filename << endl;
	}

	texture_2D_pixel_format type;
	if (_depth == 255) {
		if (_step == 4) {
			type = texture_RGBA8888;
		} else if (_step == 3) {
			type = texture_RGB888;
		} else {
			type = texture_A8;
		}
	} else {
		if (_step == 4) {
			type = texture_RGBA4444;
			uint16_t* tmp = (uint16_t*)malloc(_width * _height * sizeof(uint16_t));
			for (int i = 0; i < _width * _height; i++) {
				uint32_t r = _buffer[i*4+0];
				uint32_t g = _buffer[i*4+1];
				uint32_t b = _buffer[i*4+2];
				uint32_t a = _buffer[i*4+3];
				tmp[i] = r << 12 | g << 8 | b << 4 | a;
			}
			free(_buffer);
			_buffer = (uint8_t*)tmp;
		} else if (_step == 3) {
			type = texture_RGB565;
			uint16_t *tmp = (uint16_t*)malloc(_width * _height * sizeof(uint16_t));
			for (int i = 0; i < _width * _height; i++) {
				uint32_t r = _buffer[i*3+0];
				if (r == 15) {
					r = 31;
				} else {
					r <<= 1;
				}
				uint32_t g = _buffer[i*3+1];
				if (g == 15) {
					g = 63;
				} else {
					g <<= 2;
				}
				uint32_t b = _buffer[i*3+2];
				if (b == 15) {
					b = 31;
				} else {
					b <<= 1;
				}
				tmp[i] = r << 11 | g << 5 | b;
			}
			free(_buffer);
			_buffer = (uint8_t*)tmp;
		} else {
			type = texture_A8;
			for (int i = 0; i < _width * _height; i++) {
				uint8_t c = _buffer[i];
				if (c == 15) {
					_buffer[i] = 255;
				} else {
					_buffer[i] *= 16;
				}
			}
		}
	}
	const char* err = texture_load(id, type, _width, _height, _buffer);
	free(_buffer);
	if (err) {
		return -1;
	}
	return 0;
}
int cc_ppm::load_ppm(string filename) {
	string tmp = filename + ".ppm";
	FILE* rgb = fopen(tmp, "rb");
	tmp = filename + ".pgm";
	FILE* alpha = fopen(tmp, "rb");
	if (rgb == NULL && alpha == NULL) {
		return -1;
	}
	int ok = loadppm_from_file(rgb, alpha);
	if (rgb) {
		fclose(rgb);
	}
	if (alpha) {
		fclose(alpha);
	}
	if (!ok) {
		if (_buffer) {
			free(_buffer);
		}
	}
	if (_depth == 255) {
		if (_step == 4) {
			return (int)ppm_RGBA8;
		} else if (_step == 3) {
			return (int)ppm_RGB8;
		} else {
			return (int)ppm_ALPHA8; 
		}
	} else {
		if (_step == 4) {
			return (int)ppm_RGBA4;
		} else if (_step == 3) {
			return (int)ppm_RGB4;
		} else {
			return (int)ppm_ALPHA4;
		}
	}
}
int cc_ppm::save_ppm(string filename, ppm_type type, int width, int height, void* data) {

}
//private:
char* cc_ppm::readline(FILE *f, char* buffer) {
	while(true) {
		char* ret = fgets(buffer, LINEMAX, f);
		if (ret == NULL) {
			return NULL;
		}
		if (ret[0] != '#') {
			return ret;
		}
	}
}
int cc_ppm::ppm_header(FILE *f) {
	char tmp[LINEMAX];
	char *line = readline(f, tmp);
	if (line == NULL)
		return 0;
	char c = 0;
	sscanf(line, "P%c", &c);
	_type = c;
	line  = readline(f, tmp);
	if (line == NULL)
		return 0;
	sscanf(line, "%d %d", &_width, %_height);
	line = readline(f, tmp);
	if (line == NULL)
		return 0;
	sscanf(line, "%d", &_depth);
	return 1;
}
int cc_ppm::ppm_data(FILE *f, int id, int skipp) {
	int n = _width * _height;
	uint8_t *buf = _buffer + skip;
	uint8_t *tmp;
	switch (id) {
	case '3': // RGB text
		for (int i = 0; i < n; i++) {
			int r, g, b;
			fscanf(f, "%d %d %d", &r, &g, &b);
			buf[i*_step+0] = (uint8_t)r;
			buf[i*_step+1] = (uint8_t)g;
			buf[i*_step+2] = (uint8_t)b;
		}
		break;
	case '2': // alpha text
		for (int i = 0; i < n; i++) {
			int alpha;
			fscanf(f, "%d", &alpha);
			buf[i*_step] = (uint8_t)alpha;
		}
		break;
	case '6': // RGB binary
		tmp = (uint8_t*)malloc(n*3);
		if (fread(tmp, n*3, 1, f) == 0) {
			free(tmp);
			return 0;
		}
		for (int i = 0; i < n; i++) {
			_buffer[i*_step+0] = tmp[i*3+0];
			_buffer[i*_step+1] = tmp[i*3+1];
			_buffer[i*_step+2] = tmp[i*3+2];
		}
		free(tmp);
		break;
	case '5': // alpha binary
		tmp = (uint8_t*)malloc(n);
		if (fread(tmp, n, 1, f) == 0) {
			free(tmp);
			return 0;
		}
		for (int i = 0; i < n; i++) {
			_buffer[i*_step] = tmp[i];
		}
		free(tmp);
		break;
	default:
		return 0;
	}	
	return 1;
}
void cc_ppm::loadppm_from_file(FILE *rgb, FILE *alpha) {
	_buffer = NULL;
	_step = 0;
	int rgb_id = 0;
	int alpha_id = 0;
	if (rgb) {
		if (!ppm_header(rgb)) {
			return 0;
		}
		rgb_id = _type;
		_step += 3;
	}	
	if (alpha) {
		if (rgb == NULL) {
			if (!ppm_header(alpha)) {
				return 0;
			}
		} else {
			cc_ppm pgm;
			if (!pgm->ppm_header(alpha)) {
				return 0;
			}
			if (_depth != pgm._depth || _width != pgm._width || _height != pgm._height) {
				return 0;
			}
			alpha_id = pgm._type;
		}
		_step += 1;
	}
	_buffer = (uint8_t*)malloc(_height * _width * sizeof(uint8_t));
	if (rgb) {
		if (!ppm_data(rgb, rgb_id, 0)) {
			return 0;
		}
	}
	if (alpha) {
		int skip = 0;
		if (rgb) {
			skip = 3;
		}
		if (!ppm_data(alpha, alpha_id, skip)) {
			return 0;
		}
	}
	return 1;
}
void cc_ppm::ppm_type(string format) {

}
void cc_ppm::save_rgb(string filename, int width, int height, int step, int depth) {

}
void cc_ppm::save_alpha(string filename, int width, int height, int step, int depth, int offset) {

}
