#ifndef CC_MATRIX_H
#define CC_MATRIX_H

struct cc_srt {
	int offx;
	int offy;
	int scalex;
	int scaley;
	int rot;
};

class cc_matrix {
	int _m[6];
public:
	cc_matrix();
	cc_matrix(int value[]);
	~cc_matrix();

	int inverse(const cc_matrix* mo);
	void srt(const struct cc_srt* srt);
	void rotation(int rot);
	void scale(int sx, int sy);
	void sr(int sx, int sy, int d);
	void rs(int sx, int sy, int d);
private:
	inline void scale_mat(int *m, int sx, int sy);
	// 把矩阵设为单位矩阵
	inline void identity() {
		_m[0] = 1024;
		_m[1] = 0;
		_m[2] = 0;
		_m[3] = 1024;
		_m[4] = 0;
		_m[5] = 0;
	}
	// 矩阵mm1 乘上 mm2
	inline void mul(const cc_matrix* mm1, const cc_matrix* mm2) {
		const int *m1 = mm1->_m;
		const int *m2 = mm2->_m;
		_m[0] = (m1[0] * m2[0] + m1[1] * m2[2]) /1024;
		_m[1] = (m1[0] * m2[1] + m1[1] * m2[3]) /1024;
		_m[2] = (m1[2] * m2[0] + m1[3] * m2[2]) /1024;
		_m[3] = (m1[2] * m2[1] + m1[3] * m2[3]) /1024;
		_m[4] = (m1[4] * m2[0] + m1[5] * m2[2]) /1024 + m2[4];
		_m[5] = (m1[4] * m2[1] + m1[5] * m2[3]) /1024 + m2[5];
	}
};

#endif
