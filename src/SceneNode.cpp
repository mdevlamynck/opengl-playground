
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SceneNode.hpp"

/******************************************************************************/

GLuint		SceneNode::s_modelViewProjectionUniform		= 0;
GLuint		SceneNode::s_modelViewUniform				= 1;
GLuint		SceneNode::s_lightColorUniform				= 2;
GLuint		SceneNode::s_lightPositionUniform			= 3;

GLuint		SceneNode::s_positionShader					= 0;
GLuint		SceneNode::s_normalShader					= 1;
GLuint		SceneNode::s_colorShader					= 2;
GLuint		SceneNode::s_uvShader						= 3;

glm::mat4*	SceneNode::s_pViewMatrix					= NULL;
glm::mat4*	SceneNode::s_pProjectionMatrix				= NULL;

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
    current				*=	m_translation * m_rotation * m_scale;

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

void SceneNode::setModelViewProjectionUniform(GLuint in_modelViewProjectionUniform)
{
    s_modelViewProjectionUniform	= in_modelViewProjectionUniform;
}
void SceneNode::setModelViewUniform(GLuint in_modelViewUniform)
{
    s_modelViewUniform				= in_modelViewUniform;
}
void SceneNode::setLightColorUniform(GLuint in_lightColorUniform)
{
    s_lightColorUniform				= in_lightColorUniform;
}
void SceneNode::setLightPositionUniform(GLuint in_lightPositionUniform)
{
    s_lightPositionUniform			= in_lightPositionUniform;
}

void SceneNode::setPositionShader(GLuint in_positionShader)
{
    s_positionShader	= in_positionShader;
}

void SceneNode::setNormalShader(GLuint in_normalShader)
{
    s_normalShader		= in_normalShader;
}

void SceneNode::setColorShader(GLuint in_colorShader)
{
    s_colorShader		= in_colorShader;
}

void SceneNode::setUvShader(GLuint in_uvShader)
{
    s_uvShader			= in_uvShader;
}

void SceneNode::setViewMatrix(glm::mat4* in_pViewMatrix)
{
    s_pViewMatrix			= in_pViewMatrix;
}

void SceneNode::setProjectionMatrix(glm::mat4* in_pProjectionMatrix)
{
    s_pProjectionMatrix	= in_pProjectionMatrix;
}

/******************************************************************************/
