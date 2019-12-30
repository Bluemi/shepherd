#include "packet_helper.hpp"

namespace packet_helper {
	void write_to_buffer(const std::string& s, std::vector<char>* buffer) {
		write_to_buffer(s.size(), buffer);
		buffer->insert(buffer->end(), s.cbegin(), s.cend());
	}

	void write_to_buffer(const glm::vec3& v, std::vector<char>* buffer) {
		write_to_buffer(v.x, buffer);
		write_to_buffer(v.y, buffer);
		write_to_buffer(v.z, buffer);
	}

	void write_to_buffer(const glm::vec2& v, std::vector<char>* buffer) {
		write_to_buffer(v.x, buffer);
		write_to_buffer(v.y, buffer);
	}

	void write_to_buffer(const glm::ivec3& v, std::vector<char>* buffer) {
		write_to_buffer(v.x, buffer);
		write_to_buffer(v.y, buffer);
		write_to_buffer(v.z, buffer);
	}

	void read_from_buffer(std::string* s, const char** buffer) {
		std::size_t num_chars(0);
		read_from_buffer<std::size_t>(&num_chars, buffer);
		s->assign(*buffer, (*buffer)+num_chars);
		(*buffer) += num_chars;
	}

	void read_from_buffer(glm::vec3* v, const char** buffer) {
		read_from_buffer(&v->x, buffer);
		read_from_buffer(&v->y, buffer);
		read_from_buffer(&v->z, buffer);
	}

	void read_from_buffer(glm::vec2* v, const char** buffer) {
		read_from_buffer(&v->x, buffer);
		read_from_buffer(&v->y, buffer);
	}

	void read_from_buffer(glm::ivec3* v, const char** buffer) {
		read_from_buffer(&v->x, buffer);
		read_from_buffer(&v->y, buffer);
		read_from_buffer(&v->z, buffer);
	}
}
