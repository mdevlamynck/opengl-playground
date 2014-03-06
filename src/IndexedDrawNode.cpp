
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IndexedDrawNode.hpp"

/******************************************************************************/

IndexedDrawNode::IndexedDrawNode(GLfloat* in_pData, size_t in_sizeData, GLshort* in_pIndex, size_t in_sizeIndex)
    : m_sizeData	( in_sizeData	)
    , m_sizeIndex	( in_sizeIndex	)
{
    glGenBuffers				( 1, &m_dataBuffer	);
    glBindBuffer				( GL_ARRAY_BUFFER, m_dataBuffer	);
    glBufferData				( GL_ARRAY_BUFFER,			sizeof(float) * in_sizeData, in_pData, GL_STATIC_DRAW	);
    glBindBuffer				( GL_ARRAY_BUFFER, 0	);

    glGenBuffers				( 1, &m_indexBuffer	);
    glBindBuffer				( GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer	);
    glBufferData				( GL_ELEMENT_ARRAY_BUFFER,	sizeof(float) * in_sizeIndex, in_pIndex, GL_STATIC_DRAW	);
    glBindBuffer				( GL_ELEMENT_ARRAY_BUFFER, 0	);

    glGenVertexArrays			( 1, &m_vao	);
    glBindVertexArray			( m_vao	);

    glBindBuffer				( GL_ARRAY_BUFFER, m_dataBuffer	);
    glEnableVertexAttribArray	( s_positionShader	);
    glEnableVertexAttribArray	( s_colorShader		);
    glVertexAttribPointer		( s_positionShader,	4,	GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer		( s_colorShader,	4,	GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(GLfloat) * m_sizeData / 2)	);
    glBindBuffer				( GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer	);

    glBindVertexArray			( 0	);
    glDisableVertexAttribArray	( s_positionShader	);
    glDisableVertexAttribArray	( s_colorShader		);
}

IndexedDrawNode::~IndexedDrawNode()
{
    glDeleteBuffers				( 1, &m_dataBuffer		);
    glDeleteBuffers				( 1, &m_indexBuffer		);
    glDeleteVertexArrays		( 1, &m_vao	);
}

/******************************************************************************/

void IndexedDrawNode::render(MatrixStack& in_stack)
{
    // Save Transformation Matrix
    in_stack.push();

    // Apply Node Transformations
    glm::mat4& current	= in_stack.get();
    current				= (*s_pCameraPerspective) * current * m_translation * m_rotation * m_scale;

    // Render Node
    glBindVertexArray	( m_vao	);
    glUniformMatrix4fv	( s_mvpUniform, 1, GL_FALSE, glm::value_ptr(current)	);

    glDrawElements		( GL_TRIANGLES, m_sizeIndex, GL_UNSIGNED_SHORT, 0	);

    glBindVertexArray	( 0	);

    // Render Children
    for(SceneNode* child : m_children)
        child->render(in_stack);

    // Restore Transformation Matrix
    in_stack.pop();

}

/******************************************************************************/
