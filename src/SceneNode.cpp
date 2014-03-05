
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SceneNode.hpp"

/******************************************************************************/

SceneNode::SceneNode()
{
}

SceneNode::~SceneNode()
{
    removeAllChildren();
}

/******************************************************************************/

void SceneNode::render(MatrixStack& in_stack)
{
    // Save Transformation Matrix
    in_stack.push();

    // Apply Node Transformations
    glm::mat4& current	= in_stack.get();
    current				= m_scale * m_rotation * m_translation * current;

    // Render Children
    for(SceneNode* child : m_children)
        child->render(in_stack);

    // Restore Transformation Matrix
    in_stack.pop();

}

/******************************************************************************/

void SceneNode::addChild(SceneNode* in_pSceneNode)
{
    m_children.insert(in_pSceneNode);
}

void SceneNode::removeChild(SceneNode* in_pSceneNode)
{
    auto child = m_children.find(in_pSceneNode);
    if(child != m_children.end())
        delete *child;

    m_children.erase(child);
}

void SceneNode::removeAllChildren()
{
    for(SceneNode* node : m_children)
        delete node;

    m_children.clear();
}

/******************************************************************************/

void SceneNode::translate(glm::vec3 in_translation)
{
    m_translation	= glm::translate	( m_translation, in_translation	);
}

void SceneNode::rotate(float in_angle, glm::vec3 in_axis)
{
    m_rotation		= glm::rotate		( m_rotation, in_angle, in_axis	);
}

void SceneNode::scale(glm::vec3 in_factors)
{
    m_scale			= glm::scale		( m_scale, in_factors			);
}

/******************************************************************************/
