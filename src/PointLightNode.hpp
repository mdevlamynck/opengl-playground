
#ifndef	POINT_LIGHT_NODE_HPP
#define	POINT_LIGHT_NODE_HPP

#include <set>
#include <tuple>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "SceneNode.hpp"

/******************************************************************************/

typedef std::tuple<GLushort, GLushort, GLushort>	Vertex;
typedef std::set<Vertex>							VertexSet;

class PointLightNode : public SceneNode
{

// Inits
public:

                    PointLightNode			();

    virtual			~PointLightNode		();

// Render
public:

    virtual void	render				(MatrixStack& in_stack);

/******************************************************************************/

private:

    glm::vec4	m_position;
};

/******************************************************************************/

#endif	// POINT_LIGHT_NODE_HPP

