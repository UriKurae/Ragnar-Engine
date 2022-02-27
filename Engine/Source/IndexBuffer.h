#pragma once

//#include <stdint.h>

class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(const unsigned int* indices, unsigned int count);
	virtual ~IndexBuffer();

	void SetData(const unsigned int* indices, unsigned int count);
	void Bind() const;
	void Unbind() const;

	inline const unsigned int& GetCount() const { return count; }
	inline const unsigned int& GetID() const { return ibo; }


private:
	unsigned int ibo;
	unsigned int count;
};