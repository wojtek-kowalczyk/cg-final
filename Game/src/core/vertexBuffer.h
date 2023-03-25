#pragma once

#include <vector>

class VertexBufferLayout
{
public:
	struct Element // eg. 3 floats not normalized, 4 floats normalized
	{
		unsigned int amount; 
		unsigned int type;
		unsigned int normalized;
	};

	void Add(const Element& element);
	unsigned int GetStride() const;
	unsigned int GetSizeForType(unsigned int type) const;
	const std::vector<Element>& GetElements() const;

private:
	std::vector<Element> m_elements; 
};

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	unsigned int GetSize() const { return m_size; }

private:
	unsigned int m_id;
	unsigned int m_size;
};
