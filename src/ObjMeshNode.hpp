
#ifndef	OBJ_MESH_NODE_HPP
#define	OBJ_MESH_NODE_HPP

#include <set>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "SceneNode.hpp"

/******************************************************************************/

class ObjMeshNode : public SceneNode
{

// Inits
public:

                    ObjMeshNode			(bool in_bSmooth);

    virtual			~ObjMeshNode		();

// Render
public:

    virtual	void	render				(MatrixStack& in_stack);

// Utils
protected:

    void			setupMesh			(GLfloat* in_pData, GLushort* in_pIndex);
    void			loadObj				(bool in_bSmooth);

/******************************************************************************/

private:

    GLuint	m_vao;
    GLuint	m_dataBuffer;
    GLuint	m_indexBuffer;

    size_t	m_nbVertex;

    size_t	m_nbFace;

};

/******************************************************************************/

#endif	// OBJ_MESH_NODE_HPP

