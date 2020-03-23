#if !defined( _CHEW_H ) || defined( TABLEONLY )
#define _CHEW_H

#ifndef TABLEONLY
#include <SDL_opengl.h>
#endif

#ifdef __cplusplus
#ifndef TABLEONLY
extern "C" {
#endif
#endif

#ifndef TABLEONLY

#if defined( WINDOWS ) || defined( _WINDOWS ) || defined( WIN32 )
#include <windows.h>
#else
#define APIENTRY
#endif
#include <GL/GL.h>
#include "chewtypes.h"

#define chew_FUN_EXPORT extern

#define CHEWTYPEDEF( ret, name, ... ) \
typedef ret (__stdcall *name##_t)( __VA_ARGS__ );	\
extern  name##_t	name;

#define CHEWTYPEDEF2( ret, name, usename, ... ) \
typedef ret (__stdcall *usename##_t)( __VA_ARGS__ );	\
extern  usename##_t	usename;

void chewInit();
void * chewGetProcAddress( const char *name );

#endif

// Add the things you want here; DO NOT put ; at end of line.



CHEWTYPEDEF( void, glGenVertexArrays, GLsizei n, GLuint *arrays )
CHEWTYPEDEF( void, glBindVertexArray, GLuint array )
CHEWTYPEDEF( void, glGenBuffers, GLsizei n, GLuint * buffers )
CHEWTYPEDEF( void, glBindBuffer, GLenum target, GLuint buffer )
CHEWTYPEDEF( void, glBufferData, GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
CHEWTYPEDEF( void, glNamedBufferData, GLuint buffer, GLsizeiptr size, const void *data, GLenum usage )
CHEWTYPEDEF( void, glEnableVertexAttribArray, GLuint index )
CHEWTYPEDEF( void, glDisableVertexAttribArray, GLuint index )
CHEWTYPEDEF( void, glEnableVertexArrayAttrib, GLuint vaobj, GLuint index )
CHEWTYPEDEF( void, glDisableVertexArrayAttrib, GLuint vaobj, GLuint index )
CHEWTYPEDEF( void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer )
CHEWTYPEDEF( void, glVertexAttribIPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer )
CHEWTYPEDEF( void, glVertexAttribLPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer )
CHEWTYPEDEF( void, glDeleteVertexArrays, GLsizei n, const GLuint *arrays )
CHEWTYPEDEF( void, glDeleteBuffers, GLsizei n, const GLuint * buffers )
CHEWTYPEDEF( void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data )
CHEWTYPEDEF( void, glNamedBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data )

//Already covered in SDL_opengl.h
CHEWTYPEDEF2( void, glActiveTexture, glActiveTextureCHEW, GLenum texture )
CHEWTYPEDEF2( void, glSampleCoverage, glSampleCoverageCHEW, GLfloat value, GLboolean invert )

CHEWTYPEDEF( void, glDebugMessageCallback, GLDEBUGPROC callback, const void * userParam )
CHEWTYPEDEF( void, glDebugMessageControl, GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled )

CHEWTYPEDEF( void, glGenFramebuffers, GLsizei n, GLuint * framebuffers )
CHEWTYPEDEF( void, glGenRenderbuffers, GLsizei n, GLuint * renderbuffers )
CHEWTYPEDEF( void, glBindFramebuffer, GLenum target, GLuint framebuffer )
CHEWTYPEDEF( void, glBindRenderbuffer, GLenum target, GLuint renderbuffer )
CHEWTYPEDEF( void, glRenderbufferStorageMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height )
CHEWTYPEDEF( void, glNamedRenderbufferStorageMultisample, GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height )
CHEWTYPEDEF( void, glFramebufferRenderbuffer, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
CHEWTYPEDEF( void, glTexImage2DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations )
CHEWTYPEDEF( void, glFramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
CHEWTYPEDEF( void, glBlitFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter )
CHEWTYPEDEF( void, glBlitNamedFramebuffer, GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter )
CHEWTYPEDEF( void, glDeleteFramebuffers, GLsizei n, const GLuint * framebuffers )
CHEWTYPEDEF( void, glDeleteRenderbuffers, GLsizei n, const GLuint * renderbuffers )
CHEWTYPEDEF( GLenum, glCheckFramebufferStatus, GLenum target )
CHEWTYPEDEF( GLenum, glCheckNamedFramebufferStatus, GLuint framebuffer, GLenum target )
CHEWTYPEDEF( void, glFramebufferTexture, GLenum target, GLenum attachment, GLuint texture, GLint level )



CHEWTYPEDEF( GLuint, glCreateProgram, void )
CHEWTYPEDEF( GLuint, glCreateShader, GLenum )
CHEWTYPEDEF( void, glShaderSource, GLuint shader, GLsizei count, const GLchar **string, const GLint *length )
CHEWTYPEDEF( void, glCompileShader, GLuint shader )
CHEWTYPEDEF( void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params )
CHEWTYPEDEF( void, glGetShaderInfoLog, GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog )
CHEWTYPEDEF( void, glDeleteProgram, GLuint program )
CHEWTYPEDEF( void, glDeleteShader, GLuint shader )
CHEWTYPEDEF( void, glAttachShader, GLuint program, GLuint shader )
CHEWTYPEDEF( void, glLinkProgram, GLuint program )
CHEWTYPEDEF( void, glGetProgramiv, GLuint program, GLenum pname, GLint *params )
CHEWTYPEDEF( void, glUseProgram, GLuint program )
CHEWTYPEDEF( void, glUniform1f, GLint location, GLfloat v0 )
CHEWTYPEDEF( void, glUniform2f, GLint location, GLfloat v0, GLfloat v1 )
CHEWTYPEDEF( void, glUniform3f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2 )
CHEWTYPEDEF( void, glUniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 )
CHEWTYPEDEF( void, glUniform1i, GLint location, GLint v0 )
CHEWTYPEDEF( void, glUniform2i, GLint location, GLint v0, GLint v1 )
CHEWTYPEDEF( void, glUniform3i, GLint location, GLint v0, GLint v1, GLint v2 )
CHEWTYPEDEF( void, glUniform4i, GLint location, GLint v0, GLint v1, GLint v2, GLint v3 )
CHEWTYPEDEF( void, glUniform1fv, GLint location, GLsizei count, const GLfloat *value )
CHEWTYPEDEF( void, glUniform2fv, GLint location, GLsizei count, const GLfloat *value )
CHEWTYPEDEF( void, glUniform3fv, GLint location, GLsizei count, const GLfloat *value )
CHEWTYPEDEF( void, glUniform4fv, GLint location, GLsizei count, const GLfloat *value )
CHEWTYPEDEF( void, glUniform1iv, GLint location, GLsizei count, const GLint *value )
CHEWTYPEDEF( void, glUniform2iv, GLint location, GLsizei count, const GLint *value )
CHEWTYPEDEF( void, glUniform3iv, GLint location, GLsizei count, const GLint *value )
CHEWTYPEDEF( void, glUniform4iv, GLint location, GLsizei count, const GLint *value )
CHEWTYPEDEF( void, glUniformMatrix2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )
CHEWTYPEDEF( void, glUniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )
CHEWTYPEDEF( void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )
CHEWTYPEDEF( void, glGetProgramInfoLog, GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog )
CHEWTYPEDEF( GLint, glGetUniformLocation, GLuint program, const GLchar *name )

CHEWTYPEDEF( void *, glMapBuffer, GLenum target, GLenum access )
CHEWTYPEDEF( void *, glMapNamedBuffer, GLuint buffer, GLenum access )
CHEWTYPEDEF( GLboolean, glUnmapBuffer, GLenum target )

#ifdef __cplusplus
#ifndef TABLEONLY
};
#endif
#endif

#endif // _CHEW_H
