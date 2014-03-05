
#ifndef	RENDERED_NODE_HPP
#define	RENDERED_NODE_HPP

#include <set>
#include <GL/glew.h>

#include "SceneNode.hpp"

/******************************************************************************/

class RenderedNode : public SceneNode
{

// Inits
public:

                RenderedNode		();
    virtual		~RenderedNode		();

// Common Rendered Nodes Data
public:

    static void	setTransformUniform		(GLuint in_transformUnif);

    static void	setPositionShader		(GLuint in_positionShader);
    static void	setColorShader			(GLuint in_colorShader);

    static void setCameraPerspective	(glm::mat4 const& in_cameraPerspective);

/******************************************************************************/

protected:

    static GLuint		s_transformUnif;

    static GLuint		s_positionShader;
    static GLuint		s_colorShader;

    static glm::mat4	s_cameraPerspective;

};

/******************************************************************************/

#endif	// RENDERED_NODE_HPP
