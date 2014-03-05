
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SceneNode.hpp"

/******************************************************************************/

GLuint		SceneNode::s_mvpUniform			= 0;

GLuint		SceneNode::s_positionShader		= 0;
GLuint		SceneNode::s_colorShader		= 0;

glm::mat4*	SceneNode::s_pCameraPerspective	= NULL;

/******************************************************************************/

SceneNode::SceneNode()
{
    setTranslation	( glm::vec3(0.0f)					);
    setRotation		( 0, glm::vec3(0.0f, 1.0f, 0.0f)	);
    setScale		( glm::vec3(1.0f)					);
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
    glm::mat4& current	=	in_stack.get();
    current				=	m_scale * m_rotation * m_translation * current;

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

void SceneNode::setTranslation(glm::vec3 in_translation)
{
    m_translation		= glm::mat4(1.0f);
    translate(in_translation);
}

void SceneNode::setRotation(float in_angle, glm::vec3 in_axis)
{
    m_rotation			= glm::mat4(1.0f);
    rotate(in_angle, in_axis);
}

void SceneNode::setScale(glm::vec3 in_factors)
{
    m_scale				= glm::mat4(1.0f);
    scale(in_factors);
}

/******************************************************************************/

void SceneNode::setMvpUniform(GLuint in_mvpUniform)
{
    s_mvpUniform		= in_mvpUniform;
}

void SceneNode::setPositionShader(GLuint in_positionShader)
{
    s_positionShader	= in_positionShader;
}

void SceneNode::setColorShader(GLuint in_colorShader)
{
    s_colorShader		= in_colorShader;
}

void SceneNode::setCameraPerspective(glm::mat4* in_pCameraPerspective)
{
    s_pCameraPerspective	= in_pCameraPerspective;
}

/******************************************************************************/
