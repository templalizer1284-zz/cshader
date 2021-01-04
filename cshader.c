#include "cshader.h"

Shader ShaderInit(){
	Shader shader;
	
	shader.Load = &ShaderLoad;
	shader.Destroy = &ShaderDestroy;

	return shader;
}

void ShaderLoad(Shader *shader, const char *vertex_file_path, const char *fragment_file_path){

	/* Load Vertex Shader File */
	FILE *VertexShaderStream;
	if( (VertexShaderStream = fopen(vertex_file_path, "r")) == NULL ){
		printf("Shaders: Unable to open %s\n", vertex_file_path);
		printf("Shaders: Terminating...\n");
	}

	FILE *VertexErrorLog;
	if( (VertexErrorLog = fopen("shader.log", "w")) == NULL ){
		printf("Shaders: Unable to open %s\n", "shader.log");
		printf("Shaders: Terminating...\n");
	}
	
	fseek(VertexShaderStream, 0, SEEK_END); /* set pointer at the end of file */
	long fsize = ftell(VertexShaderStream); /* get file size */
	rewind(VertexShaderStream); /* set pointer and the beginning of a file */

	int vlength = fsize;

	shader->vertexcode = malloc(fsize + 1);
	shader->vertexcode[fsize] = 0;

	int i = 0;
	printf("Shaders: Reading shader...\n");
	while(true){
		if( (shader->vertexcode[i] = fgetc(VertexShaderStream)) == EOF ){
			break;
		}

		i++;
	}

	shader->vertexcode[i] = 0;
	fclose(VertexShaderStream);

	/* Same thing for Fragment Shader */
	FILE *FragmentShaderStream;
	if( (FragmentShaderStream = fopen(fragment_file_path, "r")) == NULL ){
		printf("Shaders: Cannot open %s. \n Terminating...\n", fragment_file_path);
		fclose(FragmentShaderStream);
	}

	fseek(FragmentShaderStream, 0, SEEK_END);
	fsize = ftell(FragmentShaderStream);
	rewind(FragmentShaderStream);

	int flength = fsize;

	shader->fragmentcode = malloc(fsize + 1);
	i = 0;
	while(true){
		if( (shader->fragmentcode[i] = fgetc(FragmentShaderStream)) == EOF ){
			break;
		}

		i++;
	}

	shader->fragmentcode[i] = 0;
	fclose(FragmentShaderStream);

	/* Creating shader objects */
	if( (shader->vertex = glCreateShader(GL_VERTEX_SHADER)) == 0 ){
		printf("Shaders: Error creating vertex shader object!\n");
	}

	if( (shader->fragment = glCreateShader(GL_FRAGMENT_SHADER)) == 0 ){
		printf("Shaders: Error creating fragment shader object!\n");
	}

	const char *VertexShaderPointer = shader->vertexcode;
	const char *FragmentShaderPointer = shader->fragmentcode;

	/* Attaching Shaders */
	glShaderSource(shader->vertex, 1, &VertexShaderPointer, &vlength);
	glShaderSource(shader->fragment, 1, &FragmentShaderPointer, &flength);

	/* Compiling Shaders */
	printf("Shaders: Compiling...\n");
	glCompileShader(shader->vertex);
	glCompileShader(shader->fragment);

	/* Error checking for compilation */
	printf("Shaders: Error checking...\n");
	GLint VertexShaderLogLength, vlen, FragmentShaderLogLength, flen;
	
	glGetShaderiv(shader->vertex, GL_INFO_LOG_LENGTH, &VertexShaderLogLength);
	glGetShaderiv(shader->fragment, GL_INFO_LOG_LENGTH, &FragmentShaderLogLength);

	if( VertexShaderLogLength > 1 ){
		GLchar *VertexCompilerLog = (GLchar*)malloc(VertexShaderLogLength * 2);
		glGetShaderInfoLog(shader->vertex, VertexShaderLogLength, &vlen, VertexCompilerLog);
		printf("Shaders: Vertex shader error! Check shader.log for more info.\n");
		fprintf(VertexErrorLog, "%s", VertexCompilerLog);
		free(VertexCompilerLog);
	}

	if( FragmentShaderLogLength > 1){
		GLchar *FragmentCompilerLog = (GLchar*)malloc(FragmentShaderLogLength * 2);
		glGetShaderInfoLog(shader->fragment, FragmentShaderLogLength, &flen, FragmentCompilerLog);
		printf("Shaders: Fragment shader error! Check shader.log for more info. \n");
		fprintf(VertexErrorLog, "%s", FragmentCompilerLog);
		free(FragmentCompilerLog);
	}

	/* Link the program */
	printf("Shaders: Linking...\n");
	if( (shader->program = glCreateProgram()) == 0 ){
		printf("Shaders: Creating program object failed.\n");
	}

	glAttachShader(shader->program, shader->vertex);
	glAttachShader(shader->program, shader->fragment);

	glLinkProgram(shader->program);

	/* Error checking for linking */
	GLint linked, ProgramLogLength, plen;
	glGetProgramiv(shader->program, GL_LINK_STATUS, &linked);
	if(linked == GL_TRUE){
		/* ok */
	}else{
		printf("Shaders: Shader program linking error.\n");
		glGetProgramiv(shader->program, GL_INFO_LOG_LENGTH, &ProgramLogLength);

		GLchar *ProgramCompilerLog = (GLchar*)malloc(sizeof(ProgramLogLength));
		glGetProgramInfoLog(shader->program, ProgramLogLength, &plen, ProgramCompilerLog);
		fprintf(VertexErrorLog, "%s", ProgramCompilerLog);
		free(ProgramCompilerLog);
	}

	free(shader->vertexcode);
	free(shader->fragmentcode);
	fclose(VertexErrorLog);
}

void ShaderDestroy(Shader *shader){
	glDetachShader(shader->vertex, shader->program);
	glDetachShader(shader->fragment, shader->program);
	glDeleteShader(shader->vertex);
	glDeleteShader(shader->fragment);
	glDeleteProgram(shader->program);

	shader->Load = NULL;
	shader->Destroy = NULL;
}
