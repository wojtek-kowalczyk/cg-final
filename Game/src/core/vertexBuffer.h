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
	VertexBuffer(const float* data, unsigned int count);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	unsigned int GetSize() const { return m_count * sizeof(float); }

private:
	unsigned int m_id;
	unsigned int m_count;
};
