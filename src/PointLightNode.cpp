
#include <iostream>
#include <sstream>
#include <fstream>
#include <cinttypes>
#include <cstdarg>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PointLightNode.hpp"

/******************************************************************************/

PointLightNode::PointLightNode()
    : m_position( glm::vec4( 0.0, 0.0, 0.0, 1.0	))
{
}

PointLightNode::~PointLightNode()
{
}

/******************************************************************************/

void PointLightNode::render(MatrixStack& in_stack)
{
    // Save Transformation Matrix
    in_stack.push();

    // Apply Node Transformations
    glm::mat4& current	=	in_stack.get();
    current				*=	m_rotation * m_translation;

    glm::vec4 camPos	= (*s_pViewMatrix) * current * m_position;
    glUniform4fv		( s_lightPositionUniform, 1, glm::value_ptr(camPos)	);

    // Render Children
    for(SceneNode* child : m_children)
        child->render(in_stack);

    // Restore Transformation Matrix
    in_stack.pop();

}

/******************************************************************************/
