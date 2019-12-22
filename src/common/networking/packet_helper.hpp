#ifndef __PACKET_HELPER_CLASS__
#define __PACKET_HELPER_CLASS__

#include <vector>
#include <string>
#include <glm/glm.hpp>

// #include "game_update_packet.hpp"

namespace packet_helper {
	// write functions --------------------------------
	template<typename T>
	void write_to_buffer(const T& t, std::vector<char>* buffer) {
		static_assert(std::is_pod_v<T>);
		const char* t_ptr = reinterpret_cast<const char*>(&t);
		buffer->insert(buffer->end(), t_ptr, t_ptr+sizeof(T));
	}

	template<>
	void write_to_buffer(const std::string& s, std::vector<char>* buffer) {
		write_to_buffer(s.size(), buffer);
		buffer->insert(buffer->end(), s.cbegin(), s.cend());
	}

	template<>
	void write_to_buffer(const glm::vec3& v, std::vector<char>* buffer) {
		write_to_buffer(v.x, buffer);
		write_to_buffer(v.y, buffer);
		write_to_buffer(v.z, buffer);
	}

	template<typename T>
	void write_to_buffer(const std::vector<T>& v, std::vector<char>* buffer) {
		write_to_buffer(v.size(), buffer);
		for (const T& t : v) {
			write_to_buffer(t, buffer);
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

	template<>
	void read_from_buffer<std::string>(std::string* s, const char** buffer) {
		std::size_t num_chars(0);
		read_from_buffer<std::size_t>(&num_chars, buffer);
		s->assign(*buffer, (*buffer)+num_chars);
		(*buffer) += num_chars;
	}

	template<>
	void read_from_buffer<glm::vec3>(glm::vec3* v, const char** buffer) {
		read_from_buffer(&v->x, buffer);
		read_from_buffer(&v->y, buffer);
		read_from_buffer(&v->z, buffer);
	}

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
		(*buffer) += sizeof(T) * num_elements;
	}
}

#endif
