#include "sugoma_gl.h"
#include <GL/glew.h>
#include "debug/logger.h"
namespace sugoma::graphics
{
    GLHandle GLUtility::CompileShader(const std::string& src, GLEnum kind)
	{
		GLHandle shader = glCreateShader(kind);
		const char* source = src.c_str();
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		GLint compile_status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
		if (!compile_status)
		{
			GLint logLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

			std::string log(logLength, ' ');
			glGetShaderInfoLog(shader, logLength, nullptr, &log[0]);

			sugoma_error("Shader compilation failed.\n" << log);
			glDeleteShader(shader);
			return 0;
		}
		return shader;
	}
	bool GLUtility::LinkProgram(GLHandle handle)
	{
		glLinkProgram(handle);
		int link_status = 0;
		glGetProgramiv(handle, GL_LINK_STATUS, &link_status);
		if (!link_status)
		{
			char infoLog[512];
			glGetProgramInfoLog(handle, 512, NULL, infoLog);
			sugoma_error("Pipeline link failure : " << infoLog);
			return false;
		}
		return true;
	}
}
