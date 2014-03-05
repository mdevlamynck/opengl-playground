
#ifndef	ARRAY_DRAW_NODE_HPP
#define	ARRAY_DRAW_NODE_HPP

#include <set>
#include <GL/glew.h>

#include "RenderedNode.hpp"

/******************************************************************************/

class ArrayDrawNode : public RenderedNode
{

// Inits
public:

            ArrayDrawNode	(float* in_pData, size_t in_size);
    virtual	~ArrayDrawNode	();

// Render
public:

    void	render			(MatrixStack& in_stack);

/******************************************************************************/

protected:

    GLuint	m_vao;
    GLuint	m_dataBuffer;

    size_t	m_size;

};

/******************************************************************************/

#endif	// ARRAY_DRAW_NODE_HPP
