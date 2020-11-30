
#include "pch.h"
#include "VertexIndexArray.h"

namespace gk
{
	bool VertexIndexArray::Create(const std::string& name, void* null)
	{
		VertexArray::Create(name, null);

		return true;
	}

	void VertexIndexArray::Destroy()
	{
		VertexArray::Destroy();

		// delete the index buffer m_ibo with glDeleteBuffers
		glDeleteBuffers(1, &m_ibo);
	}

	void VertexIndexArray::CreateIndexBuffer(GLenum indexType, GLsizei count, void* data)
	{
		// set m_indexType to indexType
		// set m_indexCount to count
		m_indexType = indexType;
		m_indexCount = count;

		// generate vertex buffer m_ibo with glGenBuffers
		// bind the m_ibo vertex buffer with glBindBuffer(GL_ELEMENT_ARRAY_BUFFER
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		size_t indexSize = (m_indexType == GL_UNSIGNED_SHORT) ? sizeof(GLushort) : sizeof(GLuint);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * indexSize, data, GL_STATIC_DRAW);
	}

	void VertexIndexArray::Draw(GLenum primitiveType)
	{
		// bind vertex array m_vao with glBindVertexArray
		// call glDrawElements with the parameters (primitiveType, m_indexCount, m_indexType, 0)
		glBindVertexArray(m_vao);
		glDrawElements(primitiveType, m_indexCount, m_indexType, 0);
	}

}