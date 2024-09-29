#include <iostream>

#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));	/*
		By enabling the vertex attribute array at index 0, you tell OpenGL to use the data provided in the buffer bound to GL_ARRAY_BUFFER for the vertex attribute at location 0.
		*/
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset)); /*
		0: The index of the vertex attribute to configure. This must match the index used in glEnableVertexAttribArray(0).
		2: The number of components per vertex attribute. 2 components (x and y coordinates for a 2D position).
		GL_FALSE: Specifies whether the data should be normalized, the values will be taken as they are (raw floats in this case).
		sizeof(float) * 2: The byte offset between consecutive vertex attributes (the stride). two float values (x and y),
		0: The offset (in bytes) of the first component of the vertex attribute in the buffer. 0 means the data starts right at the beginning of the buffer.
		*/
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));

}

