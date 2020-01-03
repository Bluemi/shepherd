#ifndef __PACKET_HELPER_CLASS__
#define __PACKET_HELPER_CLASS__

#include <vector>
#include <optional>
#include <string>
#include <glm/glm.hpp>

namespace packet_helper {
	// write functions --------------------------------
	template<typename T>
	void write_to_buffer(const T& t, std::vector<char>* buffer) {
		static_assert(std::is_pod_v<T>);
		const char* t_ptr = reinterpret_cast<const char*>(&t);
		buffer->insert(buffer->end(), t_ptr, t_ptr+sizeof(T));
	}

	void write_to_buffer(const std::string& s, std::vector<char>* buffer);
	void write_to_buffer(const glm::vec3& v, std::vector<char>* buffer);
	void write_to_buffer(const glm::vec2& v, std::vector<char>* buffer);
	void write_to_buffer(const glm::ivec3& v, std::vector<char>* buffer);

	template<typename T>
	void write_to_buffer(const std::vector<T>& v, std::vector<char>* buffer) {
		write_to_buffer(v.size(), buffer);
		for (const T& t : v) {
			write_to_buffer(t, buffer);
		}
	}

	template<typename T>
	void write_to_buffer(const std::optional<T>& o, std::vector<char>* buffer) {
		write_to_buffer(o.has_value(), buffer);
		if (o) {
			write_to_buffer(*o, buffer);
		}
	}

	// read functions --------------------------------
	template<typename T>
	void read_from_buffer(T* obj, const char** buffer) {
		static_assert(std::is_pod_v<T>);
		const T* obj_ptr = reinterpret_cast<const T*>(*buffer);
		(*buffer) += sizeof(T);
		*obj = *obj_ptr;
	}

	void read_from_buffer(std::string* s, const char** buffer);
	void read_from_buffer(glm::vec3* v, const char** buffer);
	void read_from_buffer(glm::vec2* v, const char** buffer);
	void read_from_buffer(glm::ivec3* v, const char** buffer);

	template<typename T>
	void read_from_buffer(std::vector<T>* vec, const char** buffer) {
		std::size_t num_elements(0);
		read_from_buffer(&num_elements, buffer);

		vec->clear();
		vec->reserve(num_elements);

		for (std::size_t i = 0; i < num_elements; i++) {
			T t;
			read_from_buffer(&t, buffer);
			vec->push_back(t);
		}
	}

	template<typename T>
	void read_from_buffer(std::optional<T>* o, const char** buffer) {
		bool has_value = false;
		read_from_buffer(&has_value, buffer);

		if (has_value) {
			T t;
			read_from_buffer(&t, buffer);
			*o = t;
		} else {
			o->reset();
		}
	}
}

#endif
