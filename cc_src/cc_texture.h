#ifdef CC_TEXTURE_H
#define CC_TEXTURE_H

#include "opengl.h"

enum texture_2D_pixel_format {
	texture_RGBA8888 = 1,
	texture_RGBA4444,
	texture_PVRTC4,
	texture_PVRTC2,
	texture_RGB888,
	texture_RGB565,
	texture_A8
};

#define MAX_TEXTURE 128

class cc_texture {
	int _width;
	int _height;
	float _invw;
	float _invh;
	GLuint _id;
};

// sington class
class cc_texture_pool {
	int _count;
	cc_texture _tex[MAX_TEXTURE];
private:
	cc_texture();
public:
	~cc_texture();

	static cc_texture_pool *instance();
	const char* texture_load(int id, texture_2D_pixel_format type, int width, int height, void *buffer);
	void texture_unload(int id);
	GLuint texture_glid(int id);
	void texture_coord(int id, float *x, float *y);
	void texture_clear_all();
	void texture_exit();
private:
	inline bool is_pot(x) {
		return (x & (x - 1)) == 0;
	} 
};

#endif