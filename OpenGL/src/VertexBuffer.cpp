#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	/*
	generates a new buffer object and assigns its unique ID to the variable buffer.The 1 specifies that one buffer object should be generated.
	The ID is stored in the buffer variable, which will be used to refer to this specific buffer later.
	*/
	GLCall(glGenBuffers(1, &m_RendererID));
	/*
	binds the buffer we just created(buffer) to the GL_ARRAY_BUFFER target.
	GL_ARRAY_BUFFER is a target to which the buffer is bound, indicating that this buffer will be used for vertex attribute data.
	*/
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID))
	/*
	allocates memory for the buffer and transfers the data from the CPU(host memory) to the GPU(graphics card memory).
	Specifies the size in bytes of the data to be transferred. 6 represents the number of float elements in the positions array, and sizeof(float) gives the size of a single float (typically 4 bytes), so the total size is 6 * 4 = 24 bytes.
	indicates that the data will not change (or will change very rarely) and will be used many times for drawing operations. This allows the GPU to optimize storage.
	*/
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID))
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0))
}
