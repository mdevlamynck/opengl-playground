
#ifndef	INDEXED_DRAW_NODE_HPP
#define	INDEXED_DRAW_NODE_HPP

#include <set>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "SceneNode.hpp"

/******************************************************************************/

class IndexedDrawNode : public SceneNode
{

// Inits
public:

                    IndexedDrawNode		(GLfloat* in_pData, size_t in_sizeData,
                                         GLshort* in_pIndex, size_t in_sizeIndex);
    virtual			~IndexedDrawNode	();

// Render
public:

    virtual	void	render				(MatrixStack& in_stack);

/******************************************************************************/

private:

    GLuint	m_vao;
    GLuint	m_dataBuffer;
    GLuint	m_indexBuffer;

    size_t	m_sizeData;
    size_t	m_sizeIndex;

};

/******************************************************************************/

#endif	// INDEXED_DRAW_NODE_HPP
