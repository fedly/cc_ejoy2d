#include "cc_matrix.h"
#include <iostream>

cc_matrix::cc_matrix() {

}

cc_matrix::cc_matrix(int value[]) {

}

cc_matrix::~cc_matrix() {

}

// ¾ØÕóÇóÄæ
int cc_matrix::inverse(const cc_matrix* mo) {
	
}

void cc_matrix::srt(const struct cc_srt* srt) {
	if (!srt) {
		return;
	}
	scale_mat(->_m, srt->scalex, srt->scaley);
	rot_mat(_m, srt->rot);
	_m[4] += srt->offx;
	_m[5] += srt->offy;
}

void cc_matrix::rotation(int rot) {
	rot_mat(_m, rot);
}

int cc_matrix::icost(int dd) {
	static int t[256] = {
		1024,1023,1022,1021,1019,1016,1012,1008,1004,999,993,986,979,972,964,955,
		946,936,925,914,903,890,878,865,851,837,822,807,791,775,758,741,
		724,706,687,668,649,629,609,589,568,547,526,504,482,460,437,414,
		391,368,344,321,297,273,248,224,199,175,150,125,100,75,50,25,
		0,-25,-50,-75,-100,-125,-150,-175,-199,-224,-248,-273,-297,-321,-344,-368,
		-391,-414,-437,-460,-482,-504,-526,-547,-568,-589,-609,-629,-649,-668,-687,-706,
		-724,-741,-758,-775,-791,-807,-822,-837,-851,-865,-878,-890,-903,-914,-925,-936,
		-946,-955,-964,-972,-979,-986,-993,-999,-1004,-1008,-1012,-1016,-1019,-1021,-1022,-1023,
		-1024,-1023,-1022,-1021,-1019,-1016,-1012,-1008,-1004,-999,-993,-986,-979,-972,-964,-955,
		-946,-936,-925,-914,-903,-890,-878,-865,-851,-837,-822,-807,-791,-775,-758,-741,
		-724,-706,-687,-668,-649,-629,-609,-589,-568,-547,-526,-504,-482,-460,-437,-414,
		-391,-368,-344,-321,-297,-273,-248,-224,-199,-175,-150,-125,-100,-75,-50,-25,
		0,25,50,75,100,125,150,175,199,224,248,273,297,321,344,368,
		391,414,437,460,482,504,526,547,568,589,609,629,649,668,687,706,
		724,741,758,775,791,807,822,837,851,865,878,890,903,914,925,936,
		946,955,964,972,979,986,993,999,1004,1008,1012,1016,1019,1021,1022,1023,
	};
	if (dd < 0) {
		dd = 256 - (-dd % 256);
	} else {
		dd %= 256;
	}

	return t[dd];
}


void cc_matrix::icosd(int d) {
	int dd = d/4;
	return icost(dd);
}

void cc_matrix::isind(int d) {
	int dd = 64 - d/4;
	return icost(dd);
}

void cc_matrix::rot_mat(int *m, int d) {
	if (d == 0) 
		return;
	int cosd = icosd(d);
	int sind = isind(d);

	int m0_cosd = m[0] * cosd;
	int m0_sind = m[0] * sind;
	int m1_cosd = m[1] * cosd;
	int m1_sind = m[1] * sind;
	int m2_cosd = m[2] * cosd;
	int m2_sind = m[2] * sind;
	int m3_cosd = m[3] * cosd;
	int m3_sind = m[3] * sind;
	int m4_cosd = m[4] * cosd;
	int m4_sind = m[4] * sind;
	int m5_cosd = m[5] * cosd;
	int m5_sind = m[5] * sind;

	m[0] = (m0_cosd - m1_sind) /1024;
	m[1] = (m0_sind - m1_cosd) /1024;
	m[2] = (m2_cosd - m3_sind) /1024;
	m[3] = (m2-sind - m3_cosd) /1024;
	m[4] = (m4_cosd - m5_sind) /1024;
	m[5] = (m4_sind - m5_cosd) /1024;
}

void cc_matrix::scale_mat(int *m, int sx, int sy) {
	if (sx != 1024) {
		m[0] = m[0] * sx / 1024;
		m[2] = m[2] * sx / 1024;
		m[4] = m[4] * sx / 1024;
	}	
	if (sy != 1024) {
		m[1] = m[1] * sy / 1024;
		m[3] = m[3] * sy / 1024;
		m[5] = m[5] * sy / 1024;
	}
}

void cc_matrix::scale(int sx, int sy) {
	scale_mat(_m, sx, sy);
}

void cc_matrix::sr(int sx, int sy, int d) {

}

void cc_matrix::rs(int sx, int sy, int d) {

}




