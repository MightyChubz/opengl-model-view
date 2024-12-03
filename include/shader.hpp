#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <memory>
#include <string_view>
#include <type_traits>

#include "stddefs.hpp"

class Shader
{
	static void delete_program(u32 *data);
	u32			read_shader(std::string_view filename, GLenum shader_type);

  public:
	std::shared_ptr<u32> id{nullptr, delete_program};

	Shader(const std::string_view vertex_path,
		   const std::string_view fragment_path);

	void					   use();

	template <typename T> void set(const std::string_view name, T value) const
	{
		if constexpr (std::is_same_v<T, bool>) {
			glUniform1i(glGetUniformLocation(*id, name.data()),
						static_cast<int>(value));
		} else if constexpr (std::is_same_v<T, int>) {
			glUniform1i(glGetUniformLocation(*id, name.data()), value);
		} else if constexpr (std::is_same_v<T, float>) {
			glUniform1f(glGetUniformLocation(*id, name.data()), value);
		}
	}
};

#endif
