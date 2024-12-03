#include <filesystem>
#include <fstream>
#include <string_view>

#include "SDL_log.h"
#include "shader.hpp"

void Shader::delete_program(u32 *data)
{
	if (data == nullptr) return;
	glDeleteProgram(*data);
	delete data;
}

u32 Shader::read_shader(std::string_view filename, GLenum shader_type)
{
	std::filesystem::path file_path(filename);

	if (std::filesystem::exists(file_path)) {
		std::ifstream file(file_path);

		if (file.is_open()) {
			std::string content((std::istreambuf_iterator<char>(file)),
								(std::istreambuf_iterator<char>()));
			file.close();

			SDL_Log("Loaded shader: %s", filename.data());

			u32			shader		 = glCreateShader(shader_type);
			const char *context_cstr = content.c_str();

			glShaderSource(shader, 1, &context_cstr, nullptr);
			glCompileShader(shader);

			i32					  success;
			std::array<char, 512> infoLog;
			std::ranges::fill(infoLog, 0);
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 512, nullptr, infoLog.data());
				SDL_Log("Error compiling shader: %s", infoLog.data());
			}

			SDL_Log("Compiled shader: %s", filename.data());

			return shader;
		}
	}

	return std::numeric_limits<u32>().max();
}

Shader::Shader(const std::string_view vertex_path,
			   const std::string_view fragment_path)
{
	u32 vertex_shader	= read_shader(vertex_path, GL_VERTEX_SHADER);
	u32 fragment_shader = read_shader(fragment_path, GL_FRAGMENT_SHADER);

	id.reset(new u32(glCreateProgram()));
	glAttachShader(*id, vertex_shader);
	glAttachShader(*id, fragment_shader);
	glLinkProgram(*id);

	i32					  success;
	std::array<char, 512> infoLog;
	std::ranges::fill(infoLog, 0);
	glGetProgramiv(*id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(*id, 512, nullptr, infoLog.data());
		SDL_Log("Error linking shader program: %s", infoLog.data());
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void Shader::use()
{
	glUseProgram(*id);
}
