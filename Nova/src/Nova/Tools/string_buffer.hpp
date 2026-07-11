#pragma once

namespace Nova
{
	template<size_t capacity>
	class string_buffer
	{
	public:
		string_buffer() = default;
		string_buffer(const std::string& string);

		bool empty() const;
		size_t length() const;
		char* data();

		string_buffer<capacity>& operator=(const std::string& string);
		operator std::string() const;

	private:
		void assign_string(const std::string& string);

		std::array<char, capacity> m_Buffer;
	};

	template<size_t capacity>
	string_buffer<capacity>::string_buffer(const std::string& string)
	{
		assign_string(string);
	}

	template<size_t capacity>
	bool string_buffer<capacity>::empty() const
	{
		return m_Buffer[0] == '\0';
	}

	template<size_t capacity>
	size_t string_buffer<capacity>::length() const
	{
		return strlen(m_Buffer.data());
	}

	template<size_t capacity>
	char* string_buffer<capacity>::data()
	{
		return m_Buffer.data();
	}

	template<size_t capacity>
	string_buffer<capacity>& string_buffer<capacity>::operator=(const std::string& string)
	{
		assign_string(string);
		return this;
	}

	template<size_t capacity>
	string_buffer<capacity>::operator std::string() const
	{
		return std::string(m_Buffer.data(), length());
	}

	template<size_t capacity>
	void string_buffer<capacity>::assign_string(const std::string& string)
	{
		strncpy_s(m_Buffer.data(), m_Buffer.size(), string.c_str(), string.size());
		m_Buffer[m_Buffer.size() - 1] = '\0';
	}
}