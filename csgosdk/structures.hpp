#pragma once
const double g_pi = 3.14159265358979323846;
const double g_radPi = 180.0 / g_pi;

struct vector{
	float x, y, z;
};

struct vec4{
	float x, y, z, w;
};

struct map {
	char name[64];
};

typedef struct {
	float matrix[3][4];
}matrix3x4;

struct view_matrix_t {
	float matrix[16];
};

struct color {
	float r, g, b;
};

struct vertex {
	FLOAT x, y, z,
		rhw;
	DWORD color;
};

struct glow {
	color clr;
	float m_flGlowAlpha = 1.f;
	char m_unk[4];
	float m_flUnk;
	float m_flBloomAmount = 1.f;
	float m_flUnk1;
	bool m_bRenderWhenOccluded = true;
	bool m_bRenderWhenUnoccluded = false;
	bool m_bFullBloomRender = false;
	char m_unk1;
	int m_nFullBloomStencilTestValue;
	int m_nGlowStyle = 0;
};