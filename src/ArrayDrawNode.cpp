
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ArrayDrawNode.hpp"

/******************************************************************************/

ArrayDrawNode::ArrayDrawNode(GLfloat* in_pData, size_t in_size)
    : m_size	( in_size	)
{
    glGenBuffers				( 1, &m_dataBuffer	);
    glBindBuffer				( GL_ARRAY_BUFFER, m_dataBuffer	);
    glBufferData				( GL_ARRAY_BUFFER, sizeof(GLfloat) * in_size, in_pData, GL_STATIC_DRAW	);
    glBindBuffer				( GL_ARRAY_BUFFER, 0	);

    glGenVertexArrays			( 1, &m_vao	);
    glBindVertexArray			( m_vao	);

    glBindBuffer				( GL_ARRAY_BUFFER, m_dataBuffer	);
    glEnableVertexAttribArray	( s_positionShader	);
    glEnableVertexAttribArray	( s_colorShader		);
    glVertexAttribPointer		( s_positionShader,	4,	GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer		( s_colorShader,	4,	GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(GLfloat) * m_size / 2)	);

    glBindVertexArray			( 0	);
    glDisableVertexAttribArray	( s_positionShader	);
    glDisableVertexAttribArray	( s_colorShader		);
}

ArrayDrawNode::~ArrayDrawNode()
{
    glDeleteBuffers				( 1, &m_dataBuffer	);
    glDeleteVertexArrays		( 1, &m_vao	);
}

/******************************************************************************/

void ArrayDrawNode::render(MatrixStack& in_stack)
{
    // Save Transformation Matrix
    in_stack.push();

    // Apply Node Transformations
    glm::mat4& current	= in_stack.get();
    current				= (*s_pProjectionMatrix) * (*s_pViewMatrix) * current * m_translation * m_rotation * m_scale;

    // Render Node
    glBindVertexArray	( m_vao	);
    glUniformMatrix4fv	( s_modelViewProjectionUniform, 1, GL_FALSE, glm::value_ptr(current));

    glDrawArrays		( GL_TRIANGLES, 0, m_size / (2 * 4)	);

    glBindVertexArray	( 0	);

    // Render Children
    for(SceneNode* child : m_children)
        child->render(in_stack);

    // Restore Transformation Matrix
    in_stack.pop();
}

/******************************************************************************/
