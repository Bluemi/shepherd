#ifndef __SHADER_PROGRAM_CLASS__
#define __SHADER_PROGRAM_CLASS__

#include <string>
#include <optional>

#include <glm/glm.hpp>

class shader;

class shader_program
{
	public:
		static std::optional<shader_program> from_files(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
		static std::optional<shader_program> from_code(const std::string& vertex_shader_code, const std::string& fragment_shader_code);
		static std::optional<shader_program> from_shaders(const shader& vertex_shader, const shader& fragment_shader);

		void use() const;
		unsigned int get_id() const;

		int get_uniform_location(const std::string& name) const;
		void set_bool(const std::string& name, bool value) const;
		void set_float(const std::string& name, float value) const;
		void set_int(const std::string& name, int value) const;
		void set_2f(const std::string& name, const glm::vec2& value) const;
		void set_3f(const std::string& name, const glm::vec3& value) const;
		void set_4f(const std::string& name, const glm::vec4& value) const;
		void set_4fv(const std::string& name, const glm::mat4& value) const;

	private:
		shader_program(unsigned int id);

		// Program ID
		unsigned int _id;
};

#endif
