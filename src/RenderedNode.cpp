
#include "RenderedNode.hpp"

/******************************************************************************/

GLuint		RenderedNode::s_transformUnif		= 0;

GLuint		RenderedNode::s_positionShader		= 0;
GLuint		RenderedNode::s_colorShader			= 0;

glm::mat4	RenderedNode::s_cameraPerspective	= glm::mat4(1.0f);

/******************************************************************************/

RenderedNode::RenderedNode()
{
}

RenderedNode::~RenderedNode()
{
}

/******************************************************************************/

void RenderedNode::setTransformUniform(GLuint in_transformUnif)
{
    s_transformUnif		= in_transformUnif;
}

void RenderedNode::setPositionShader(GLuint in_positionShader)
{
    s_positionShader	= in_positionShader;
}

void RenderedNode::setColorShader(GLuint in_colorShader)
{
    s_colorShader		= in_colorShader;
}

void RenderedNode::setCameraPerspective(glm::mat4 const& in_cameraPerspective)
{
    s_cameraPerspective	= in_cameraPerspective;
}

/******************************************************************************/
