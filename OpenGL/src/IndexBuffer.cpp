#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count): m_Count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	/*
	generates a new buffer object and assigns its unique ID to the variable buffer.The 1 specifies that one buffer object should be generated.
	The ID is stored in the buffer variable, which will be used to refer to this specific buffer later.
	*/
	GLCall(glGenBuffers(1, &m_RendererID));
	/*
	binds the buffer we just created(buffer) to the GL_ARRAY_BUFFER target.
	GL_ARRAY_BUFFER is a target to which the buffer is bound, indicating that this buffer will be used for vertex attribute data.
	*/
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID))
    /*
	allocates memory for the buffer and transfers the data from the CPU(host memory) to the GPU(graphics card memory).
	Specifies the size in bytes of the data to be transferred. 6 represents the number of float elements in the positions array, and sizeof(float) gives the size of a single float (typically 4 bytes), so the total size is 6 * 4 = 24 bytes.
	indicates that the data will not change (or will change very rarely) and will be used many times for drawing operations. This allows the GPU to optimize storage.
	*/
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));

}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}


void IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID))

}

void IndexBuffer::unbind() const 
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))

}
