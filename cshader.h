#ifndef CSHADER_H
#define CSHADER_H

typedef struct Shader Shader;

struct Shader {
	GLuint vertex;
	GLuint fragment;
	GLuint program;
	char *vertexcode;
	char *fragmentcode;
	void (*Load)(Shader *, const char *, const char *);
	void (*Destroy)(Shader *);
};

extern Shader ShaderInit(void);
extern void ShaderLoad(Shader*, const char *, const char *);
extern void ShaderDestroy(Shader *);

#endif
