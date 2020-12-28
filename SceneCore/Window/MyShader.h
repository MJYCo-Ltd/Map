#ifndef MYSHADER_H
#define MYSHADER_H

static const char *vertexShaderSource=
        "#version 330\n"
        "out vec2 uv;\n"
        "void main(void)\n"
        "{\n"
        "vec2 pos[] = vec2[](vec2(-1.0, -1.0),\n"
        "vec2( 1.0, -1.0),\n"
        "vec2(-1.0,  1.0),\n"
        "vec2( 1.0,  1.0));\n"
        "vec2 uvpos[] = vec2[](vec2(0, 0.0),\n"
        "vec2( 1.0, 0.0),\n"
        "vec2(0.0,  1.0),\n"
        "vec2( 1.0,  1.0));\n"
        "gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);\n"
        "uv = uvpos[gl_VertexID];\n"
        "}\n";

static const char *fragmentShaderSource =
        "#version 330\n"
        "in vec2 uv;\n"
        "uniform sampler2D tex;\n"
        "out vec4 vFragColor;"
        "void main() {\n"
        "   vFragColor = vec4(texture(tex, uv).rgb, 1.0);\n"
        "}\n";

#endif // MYSHADER_H
