#include "cc_texture.h"

// sington class
// class cc_texture_pool {
//private:
cc_texture_pool::cc_texture() {

}
// public:
cc_texture_pool::~cc_texture() {

}

static cc_texture_pool* cc_texture_pool::instance() {

}
const char* cc_texture_pool::texture_load(int id, texture_2D_pixel_format type, int width, int height, void *buffer) {
	if (id >= MAX_TEXTURE) {
		return "Too many texture";
	}
	texture *tex = &_tex[id];
	if (id >= _count) {
		_count = id + 1;
	}
	tex->width = width;
	tex->height = height;
	tex->invw = 1.0f / (float)width;
	tex->invh = 1.0f / (float)height;
	if (tex->id == 0) {
		// 获得一个纹理对象表示符
		glGenTextures(1, &tex->id);
	}
	if (buffer == NULL) {
		return NULL;
	}
	if ((type == texture_RGBA8888) || (is_pot(width) && is_pot(height))) {
		// 设置像素存储模式，第二个参数用于指定存储器每个像素行
		// 有多少个字节对齐，对齐字节越高，系统越优化
		glPixelStorei(GL_UNPACK_ALIGMENT, 4);
	} else {
		glPixelStorei(GL_UNPACK_ALIGMENT, 1);
	}
	glActiveTexture(GL_TEXTURE0);
	shader_texture(tex->id);
	// 确定如何把图像从纹理图像空间映射到帧缓冲图像空间
	// 映射需要重新构造纹理图像,这样就会造成应用到多边形上的图像失真
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	switch (type) {
		case texture_RGBA8888:
		// 生成一个2d纹理 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		break;
		case texture_RGB888:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
		break;
		case texture_RGBA4444:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, buffer);
		break;
		case texture_RGB565:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, buffer);
		break;
		case texture_A8:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, buffer);
		break;
		default:
		glDeleteTextures(1, &tex->id);
		tex->id = 0;
		return "Invalid pixel format";
	}

	return NULL;
}
void cc_texture_pool::texture_unload(int id) {
	if (id < 0 || id >= _count)
		return;
	texture *tex = &_tex[id];
	if (tex->id == 0)
		return;
	glDeleteTextures(1, &tex->id);
	tex->id = 0;
}
GLuint cc_texture_pool::texture_glid(int id) {
	if (id < 0 || id >= _count)
		return 0;
	return _tex[id].id;
}
void cc_texture_pool::texture_coord(int id, float *x, float *y) {
	if (id < 0 || id >= _count) {
		*x = *y = 0;
		return;
	}
	texture *tex = &_tex[id];
	*x *= tex->invw;
	*y *= tex->invh;
}
void cc_texture_pool::texture_clear_all() {
	for (int i = 0; i < _count; i++) {
		texture_unload(i);
	}
}
void cc_texture_pool::texture_exit() {
	texture_clear_all();
	_count = 0;
}
