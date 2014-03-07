
#include <iostream>
#include <sstream>
#include <fstream>
#include <cinttypes>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IndexedDrawNode.hpp"

/******************************************************************************/

IndexedDrawNode::IndexedDrawNode(GLfloat* in_pData, size_t in_sizeData, GLushort* in_pIndex, size_t in_sizeIndex)
{
    setupMesh(in_pData, in_sizeData, in_pIndex, in_sizeIndex);
}

IndexedDrawNode::IndexedDrawNode(std::string in_objPath)
{
    loadObj(in_objPath);
}

IndexedDrawNode::~IndexedDrawNode()
{
    glDeleteBuffers				( 1, &m_dataBuffer		);
    glDeleteBuffers				( 1, &m_indexBuffer		);
    glDeleteVertexArrays		( 1, &m_vao	);
}

/******************************************************************************/

void IndexedDrawNode::render(MatrixStack& in_stack)
{
    // Save Transformation Matrix
    in_stack.push();

    // Apply Node Transformations
    glm::mat4& current	= in_stack.get();
    current				= (*s_pCameraPerspective) * current * m_translation * m_rotation * m_scale;

    // Render Node
    glBindVertexArray	( m_vao	);
    glUniformMatrix4fv	( s_mvpUniform, 1, GL_FALSE, glm::value_ptr(current)	);

    glDrawElements		( GL_TRIANGLES, m_sizeIndex, GL_UNSIGNED_SHORT, 0	);

    glBindVertexArray	( 0	);

    // Render Children
    for(SceneNode* child : m_children)
        child->render(in_stack);

    // Restore Transformation Matrix
    in_stack.pop();

}

/******************************************************************************/

void IndexedDrawNode::setupMesh(GLfloat* in_pData, size_t in_sizeData, GLushort* in_pIndex, size_t in_sizeIndex)
{
    m_sizeData	= in_sizeData;
    m_sizeIndex	= in_sizeIndex;

    glGenBuffers				( 1, &m_dataBuffer	);
    glBindBuffer				( GL_ARRAY_BUFFER, m_dataBuffer	);
    glBufferData				( GL_ARRAY_BUFFER,			sizeof(GLfloat) * in_sizeData, in_pData, GL_STATIC_DRAW	);
    glBindBuffer				( GL_ARRAY_BUFFER, 0	);

    glGenBuffers				( 1, &m_indexBuffer	);
    glBindBuffer				( GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer	);
    glBufferData				( GL_ELEMENT_ARRAY_BUFFER,	sizeof(GLushort) * in_sizeIndex, in_pIndex, GL_STATIC_DRAW	);
    glBindBuffer				( GL_ELEMENT_ARRAY_BUFFER, 0	);

    glGenVertexArrays			( 1, &m_vao	);
    glBindVertexArray			( m_vao	);

    glBindBuffer				( GL_ARRAY_BUFFER, m_dataBuffer	);
    glEnableVertexAttribArray	( s_positionShader	);
    glEnableVertexAttribArray	( s_colorShader		);
    glVertexAttribPointer		( s_positionShader,	4,	GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer		( s_colorShader,	4,	GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(GLfloat) * m_sizeData / 2)	);
    glBindBuffer				( GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer	);

    glBindVertexArray			( 0	);
    glDisableVertexAttribArray	( s_positionShader	);
    glDisableVertexAttribArray	( s_colorShader		);
}

void IndexedDrawNode::loadObj(std::string in_objPath)
{
    GLfloat*	pData 			= NULL;
    GLushort*	pIndex			= NULL;

    size_t		sizeData		= 0;
    size_t		sizeIndex		= 0;

    uint32_t	beginVertexLine	= 0;
    uint32_t	beginIndexLine	= 0;

    uint32_t	line			= 0;

    // Open file & get size buffers size
    std::ifstream	file(in_objPath, std::ios::in);
    std::string		lineStr;

    assert(file.is_open() && "Failed to load obj");

    while(getline(file, lineStr))
    {
        if		( lineStr.at(0) == 'v' )
        {
            sizeData++;

            if(beginVertexLine == 0)
                beginVertexLine = line;
        }
        else if	( lineStr.at(0) == 'f' )
        {
            sizeIndex++;

            if(beginIndexLine == 0)
                beginIndexLine = line;
        }
        line++;
    }

    file.clear();
    file.seekg(0, file.beg);
    line = 0;

    // Allocate memory
    pData	= new GLfloat	[ sizeData	* 4 * 2	];
    pIndex	= new GLushort	[ sizeIndex	* 3	];

    // Read data
    while(line < beginVertexLine)
    {
        getline(file, lineStr);
        line++;
    }
    for(int i = line - beginVertexLine; i < sizeData; i++)
    {
        char tmp;
        file >> tmp;
        file >> pData[i*4+0];
        file >> pData[i*4+1];
        file >> pData[i*4+2];
        pData[i*4+3] = 1.0f;

        pData[(sizeData+i)*4+0] = 0.0f;
        pData[(sizeData+i)*4+1] = 0.0f;
        pData[(sizeData+i)*4+2] = 1.0f;
        pData[(sizeData+i)*4+3] = 1.0f;

        line++;
        getline(file, lineStr);
    }

    while(line < beginIndexLine)
    {
        getline(file, lineStr);
        line++;
    }
    for(int i = line - beginIndexLine; i < sizeIndex; i++)
    {
        char tmp;
        file >> tmp;
        file >> pIndex[i*3+0];
        file >> pIndex[i*3+1];
        file >> pIndex[i*3+2];

        pIndex[i*3+0]--;
        pIndex[i*3+1]--;
        pIndex[i*3+2]--;

        line++;
        getline(file, lineStr);
    }

    file.close();

    // send mesh to OpenGL
    setupMesh(pData, sizeData * 4 * 2, pIndex, sizeIndex * 3);

    // Release memory
    delete pData;
    delete pIndex;
}
