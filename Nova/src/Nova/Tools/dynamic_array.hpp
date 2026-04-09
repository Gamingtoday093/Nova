#pragma once
#include "pch.h"

namespace Nova
{
	template<typename T>
	class dynamic_array
	{
	public:
		dynamic_array() = default;
		dynamic_array(const std::vector<T>& vector);
		~dynamic_array();

		void operator=(const std::vector<T>& vector);

		T* data() { return m_Ptr; }
		const T* data() const { return m_Ptr; }
		size_t size() const { return m_Length; }
		bool empty() const { return m_Length == 0; }

		T* begin() { return m_Ptr; }
		T* end() { return m_Ptr + m_Length; }
		const T* begin() const { return m_Ptr; }
		const T* end() const { return m_Ptr + m_Length; }

	private:
		T* m_Ptr = nullptr;
		size_t m_Length = 0;
	};

	template<typename T>
	dynamic_array<T>::dynamic_array(const std::vector<T>& vector)
	{
		if (vector.empty()) return;
		m_Ptr = new T[vector.size()];
		m_Length = vector.size();
		memcpy_s(m_Ptr, m_Length, std::data(vector), vector.size());
	}

	template<typename T>
	dynamic_array<T>::~dynamic_array()
	{
		if (m_Ptr) delete[] m_Ptr;
	}

	template<typename T>
	void dynamic_array<T>::operator=(const std::vector<T>& vector)
	{
		if (vector.empty())
		{
			if (m_Ptr) delete[] m_Ptr;
			m_Ptr = nullptr;
			m_Length = 0;
			return;
		}
		m_Ptr = new T[vector.size()];
		m_Length = vector.size();
		memcpy_s(m_Ptr, m_Length, std::data(vector), vector.size());
	}
}
