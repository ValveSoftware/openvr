#ifndef _CHEWTYPES_H
#define _CHEWTYPES_H

#include <stdint.h>

typedef int GLfixed;
typedef int GLclampx;
typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef uint32_t GLuint32;
typedef int32_t GLint32;
typedef uint32_t GLuint;
typedef char GLchar;
typedef struct __GLsync *GLsync;

#ifndef GLDEBUGPROC
typedef void (APIENTRY *GLDEBUGPROC)(
	GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar *message, const void *userParam);
#endif

#ifndef GL_DEBUG_OUTPUT_SYNCHRONOUS
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242

#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_GEOMETRY_SHADER                0x8DD9
#define GL_STATIC_DRAW                    0x88E4
#define GL_ARRAY_BUFFER                   0x8892
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_FRAMEBUFFER                    0x8D40
#define GL_MULTISAMPLE                    0x809D
#define GL_RENDERBUFFER                   0x8D41
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_PROGRAM_POINT_SIZE             0x8642
#define GL_SAMPLE_COVERAGE                0x80A0
#define GL_TEXTURE_2D_MULTISAMPLE         0x9100
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_TEXTURE_MAX_LEVEL              0x813D
#define GL_TEXTURE_MAX_ANISOTROPY         0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY     0x84FF
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE_BASE_LEVEL             0x813C
#define GL_POINT_SPRITE_OES               0x8861

#define GL_PIXEL_UNPACK_BUFFER            0x88EC
#define GL_STREAM_DRAW                    0x88E0
#define GL_PIXEL_PACK_BUFFER              0x88EB
#define GL_READ_ONLY                      0x88B8
#define GL_BGRA                           0x80E1
#define GL_STREAM_READ                    0x88E1
#define GL_TEXTURE_SWIZZLE_RGBA           0x8E46

#define GL_SAMPLE_ALPHA_TO_COVERAGE			0x809E


#endif


#endif
