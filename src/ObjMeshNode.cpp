
#include <iostream>
#include <sstream>
#include <fstream>
#include <cinttypes>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ObjMeshNode.hpp"

GLfloat pyramidDataSmooth[] = {
    // Vertex
     0.0f,	 1.0f,	 0.0f,
     1.0f,	-1.0f,	 1.0f,
    -1.0f,	-1.0f,	 1.0f,

     0.0f,	 1.0f,	 0.0f,
    -1.0f,	-1.0f,	 1.0f,
    -1.0f,	-1.0f,	-1.0f,

     0.0f,	 1.0f,	 0.0f,
    -1.0f,	-1.0f,	-1.0f,
     1.0f,	-1.0f,	-1.0f,

     0.0f,	 1.0f,	 0.0f,
     1.0f,	-1.0f,	-1.0f,
     1.0f,	-1.0f,	 1.0f,

    -1.0f,	-1.0f,	-1.0f,
    -1.0f,	-1.0f,	 1.0f,
     1.0f,	-1.0f,	 1.0f,
     1.0f,	-1.0f,	-1.0f,

    // Normal
     0.0f,	 1.0f,	 0.0f,
     1.0f,	-2.0f,	 1.0f,
    -1.0f,	-2.0f,	 1.0f,

     0.0f,	 1.0f,	 0.0f,
    -1.0f,	-2.0f,	 1.0f,
    -1.0f,	-2.0f,	-1.0f,

     0.0f,	 1.0f,	 0.0f,
    -1.0f,	-2.0f,	-1.0f,
     1.0f,	-2.0f,	-1.0f,

     0.0f,	 1.0f,	 0.0f,
     1.0f,	-2.0f,	-1.0f,
     1.0f,	-2.0f,	 1.0f,

    -1.0f,	-2.0f,	-1.0f,
    -1.0f,	-2.0f,	 1.0f,
     1.0f,	-2.0f,	 1.0f,
     1.0f,	-2.0f,	-1.0f,

    // Color
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,

     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,

     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,

     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,

     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,

    // Uv
     0.0f,	 0.0f,
     0.0f,	 0.0f,
     0.0f,	 0.0f,

     0.0f,	 0.0f,
     0.0f,	 0.0f,
     0.0f,	 0.0f,

     0.0f,	 0.0f,
     0.0f,	 0.0f,
     0.0f,	 0.0f,

     0.0f,	 0.0f,
     0.0f,	 0.0f,
     0.0f,	 0.0f,

     0.0f,	 0.0f,
     0.0f,	 0.0f,
     0.0f,	 0.0f,
     0.0f,	 0.0f,
};

GLfloat pyramidDataHard[] = {
    // Vertex
     0.0f,	 1.0f,	 0.0f,
     1.0f,	-1.0f,	 1.0f,
    -1.0f,	-1.0f,	 1.0f,

     0.0f,	 1.0f,	 0.0f,
    -1.0f,	-1.0f,	 1.0f,
    -1.0f,	-1.0f,	-1.0f,

     0.0f,	 1.0f,	 0.0f,
    -1.0f,	-1.0f,	-1.0f,
     1.0f,	-1.0f,	-1.0f,

     0.0f,	 1.0f,	 0.0f,
     1.0f,	-1.0f,	-1.0f,
     1.0f,	-1.0f,	 1.0f,

    -1.0f,	-1.0f,	-1.0f,
    -1.0f,	-1.0f,	 1.0f,
     1.0f,	-1.0f,	 1.0f,
     1.0f,	-1.0f,	-1.0f,

    // Normal
     0.0f,	 0.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,

    -1.0f,	 0.0f,	 0.0f,
    -1.0f,	 0.0f,	 0.0f,
    -1.0f,	 0.0f,	 0.0f,

     0.0f,	 0.0f,	-1.0f,
     0.0f,	 0.0f,	-1.0f,
     0.0f,	 0.0f,	-1.0f,

     1.0f,	 0.0f,	 0.0f,
     1.0f,	 0.0f,	 0.0f,
     1.0f,	 0.0f,	 0.0f,

     0.0f,	-1.0f,	 0.0f,
     0.0f,	-1.0f,	 0.0f,
     0.0f,	-1.0f,	 0.0f,
     0.0f,	-1.0f,	 0.0f,

    // Color
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,

     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,

     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,

     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,

     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,
     0.0f,	 0.0f,	 1.0f,	 1.0f,

    // Uv
     0.0f,	 0.0f,
     0.0f,	 0.0f,
     0.0f,	 0.0f,

     0.0f,	 0.0f,
     0.0f,	 0.0f,
     0.0f,	 0.0f,

     0.0f,	 0.0f,
     0.0f,	 0.0f,
     0.0f,	 0.0f,

     0.0f,	 0.0f,
     0.0f,	 0.0f,
     0.0f,	 0.0f,

     0.0f,	 0.0f,
     0.0f,	 0.0f,
     0.0f,	 0.0f,
     0.0f,	 0.0f,
};

GLushort pyramidIndex[] = {
    0,	1,	2,
    3,	4,	5,
    6,	7,	8,
    9,	10,	11,
    12,	13,	14,
    12,	14,	15,
};

/******************************************************************************/

ObjMeshNode::ObjMeshNode(bool in_bSmooth)
{
    loadObj(in_bSmooth);
}

ObjMeshNode::~ObjMeshNode()
{
    glDeleteBuffers				( 1, &m_dataBuffer		);
    glDeleteBuffers				( 1, &m_indexBuffer		);
    glDeleteVertexArrays		( 1, &m_vao	);
}

/******************************************************************************/

void ObjMeshNode::render(MatrixStack& in_stack)
{
    // Save Transformation Matrix
    in_stack.push();

    // Apply Node Transformations
    glm::mat4& current	= in_stack.get();

    current				= (*s_pViewMatrix) * current * m_translation * m_rotation * m_scale;
    glUniformMatrix4fv	( s_modelViewUniform,			1, GL_FALSE, glm::value_ptr(current)	);

    current				= (*s_pProjectionMatrix) * current;
    glUniformMatrix4fv	( s_modelViewProjectionUniform,	1, GL_FALSE, glm::value_ptr(current)	);

    // Render Node
    glBindVertexArray	( m_vao	);
    glDrawElements		( GL_TRIANGLES, m_nbFace * 3, GL_UNSIGNED_SHORT, 0	);
    glBindVertexArray	( 0	);

    // Render Children
    for(SceneNode* child : m_children)
        child->render(in_stack);

    // Restore Transformation Matrix
    in_stack.pop();

}

/******************************************************************************/

void ObjMeshNode::setupMesh(GLfloat* in_pData, GLushort* in_pIndex)
{
    // Allocate Buffers
    glGenBuffers				( 1, &m_dataBuffer	);
    glBindBuffer				( GL_ARRAY_BUFFER, m_dataBuffer	);
    glBufferData				( GL_ARRAY_BUFFER,			sizeof(GLfloat) * m_nbVertex * (3 + 3 + 4 + 2), in_pData, GL_STATIC_DRAW	);
    glBindBuffer				( GL_ARRAY_BUFFER, 0	);

    glGenBuffers				( 1, &m_indexBuffer	);
    glBindBuffer				( GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer	);
    glBufferData				( GL_ELEMENT_ARRAY_BUFFER,	sizeof(GLushort) * m_nbFace * 3, in_pIndex, GL_STATIC_DRAW	);
    glBindBuffer				( GL_ELEMENT_ARRAY_BUFFER, 0	);

    // Allocate VAO
    glGenVertexArrays			( 1, &m_vao	);
    glBindVertexArray			( m_vao	);

    // Setup vertex attributes
    glBindBuffer				( GL_ARRAY_BUFFER, m_dataBuffer	);
    glEnableVertexAttribArray	( s_positionShader	);
    glEnableVertexAttribArray	( s_normalShader	);
    glEnableVertexAttribArray	( s_colorShader		);
    glEnableVertexAttribArray	( s_uvShader		);
    glVertexAttribPointer		( s_positionShader,	3,	GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer		( s_normalShader,	3,	GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(GLfloat) * m_nbVertex *  3			) );
    glVertexAttribPointer		( s_colorShader,	4,	GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(GLfloat) * m_nbVertex * (3 + 3)		) );
    glVertexAttribPointer		( s_uvShader,		2,	GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(GLfloat) * m_nbVertex * (3 + 3 + 4)	) );
    glBindBuffer				( GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer	);

    // Cleanup
    glBindVertexArray			( 0	);
    glEnableVertexAttribArray	( s_positionShader	);
    glEnableVertexAttribArray	( s_normalShader	);
    glEnableVertexAttribArray	( s_colorShader		);
    glEnableVertexAttribArray	( s_uvShader		);
}

void ObjMeshNode::loadObj(bool in_bSmooth)
{
    m_nbVertex	= 4 * 3 + 4;
    m_nbFace	= 6;

    GLfloat*	pData	= new GLfloat	[ m_nbVertex * (3 + 3 + 4 + 2)	];
    GLushort*	pIndex	= new GLushort	[ m_nbFace * 3	];

    if(in_bSmooth)
        pData	= pyramidDataSmooth;
    else
        pData	= pyramidDataHard;
    pIndex	= pyramidIndex;

    setupMesh(pData, pIndex);
}
