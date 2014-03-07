
#ifndef	INDEXED_DRAW_NODE_HPP
#define	INDEXED_DRAW_NODE_HPP

#include <set>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "SceneNode.hpp"

/******************************************************************************/

class IndexedDrawNode : public SceneNode
{

// Inits
public:

                    IndexedDrawNode		(GLfloat* in_pData, size_t in_sizeData,
                                         GLushort* in_pIndex, size_t in_sizeIndex);

                    IndexedDrawNode		(std::string in_objPath );

    virtual			~IndexedDrawNode	();

// Render
public:

    virtual	void	render				(MatrixStack& in_stack);

// Utils
protected:

    void			setupMesh			(GLfloat* in_pData, size_t in_sizeData,
                                         GLushort* in_pIndex, size_t in_sizeIndex);

    void			loadObj				(std::string in_objPath);

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
