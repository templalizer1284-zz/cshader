# cshader
## Simple shader loader for C, loads only vertex and fragment shader.

## Implementation:

First put **cshader.c** and **cshader.h** in the same directory with your code.

Then include it:

`#include "cshader.h"`

## Usage:

cshader mimics OP programming, first you have to create an 'object' and initialize it.

`Shader shader = ShaderInit();`
`shader.Load(&shader, "path_to_vertex_shader", "path_to_fragment_shader");`

That's it really. Then just use it where you need it.

`glUseProgram(shader.program);`

And **DON'T** forget to destroy it after.

`shader.Destroy(&shader);`

If there is a syntax error in GLSL cshader will dump it in **shader.log** file. Also don't forget to add **cshader.c** to your Makefile's file list or you will get undefined reference.

`FILES = yourcode.c cshader.c`
`main:`
  `gcc ... etc..`

That's it, i hope i made someone's life a little bit easier especially if that person just began to learn OpenGL.
