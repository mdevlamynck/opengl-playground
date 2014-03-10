
#include <iostream>
#include <sstream>
#include <fstream>
#include <cinttypes>
#include <cstdarg>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ObjMeshNode.hpp"

/******************************************************************************/

ObjMeshNode::ObjMeshNode(std::string const& in_objPath)
{
    loadObj(in_objPath);
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

void ObjMeshNode::loadObj(std::string const& in_objPath)
{
    GLfloat*		pPosition		= NULL;
    GLfloat*		pNormal			= NULL;
    GLfloat*		pColor			= NULL;
    GLfloat*		pUv				= NULL;

    VertexSet		setVertice;

    GLfloat*		pData			= NULL;
    GLushort*		pIndex			= NULL;

    std::ifstream	file			( in_objPath, std::ios::in	);
    std::string		readStr			= "";

    size_t			line			= 0;
    size_t			pos				= 0;

    size_t			sizePosition	= 0;
    size_t			sizeNormal		= 0;
    size_t			sizeColor		= 1;
    size_t			sizeUv			= 1;
    size_t			sizeFace		= 0;

    size_t			beginPosition	= 0u-1;
    size_t			beginNormal		= 0u-1;
    size_t			beginFace		= 0u-1;

    // Find pos of every set of data
    do
    {
        pos = file.tellg();

        file >> readStr;

        if		( readStr == "v"	)
            setPositions(sizePosition,	beginPosition,	pos);
        else if	( readStr == "vn"	)
            setPositions(sizeNormal,	beginNormal,	pos);
        else if	( readStr == "f"	)
            setPositions(sizeFace,		beginFace,		pos);

        line++;
    } while(getline(file, readStr));

    // Clear eof flag to be able to go back in the file
    file.clear();

    // Load data
    pPosition	= new GLfloat [ sizePosition	* 3	];
    pNormal		= new GLfloat [ sizeNormal		* 3	];
    pColor		= new GLfloat [ sizeColor		* 4	];
    pUv			= new GLfloat [ sizeUv			* 2	];

    readData	( file, pPosition,	beginPosition,	sizePosition,	3	);
    readData	( file, pNormal,	beginNormal,	sizeNormal,		3	);

    fillData	( pColor,	0.0f, 0.0f, 1.0f, 1.0f	);
    fillData	( pUv,		0.0f, 0.0f				);

    readVertice	( file, setVertice,	beginFace, 		sizeFace,		3	);

    // Resolve Vertice
    m_nbVertex	= setVertice.size();
    m_nbFace	= sizeFace;

    pData		= new GLfloat	[ setVertice.size() * (3 + 3 + 4 + 2)	];
    pIndex		= new GLushort	[ m_nbFace * 3	];

    size_t i		= 0;
    size_t offset	= 0;
    for(Vertex vertex : setVertice)
    {
        offset = 0;

        memcpy(pData + offset + 3 * i,	pPosition +  std::get<0>(vertex) * 3,	sizeof(GLfloat) * 3	);
        offset +=  (m_nbVertex * 3);

        memcpy(pData + offset + 3 * i,	pNormal + std::get<2>(vertex) * 3,		sizeof(GLfloat) * 3	);
        offset +=  (m_nbVertex * 3);

        memcpy(pData + offset + 4 * i,	pColor, 								sizeof(GLfloat) * 4	);
        offset +=  (m_nbVertex * 4);

        memcpy(pData + offset + 2 * i,	pUv, 									sizeof(GLfloat) * 2	);

        i++;
    }

    // Construct Index
    readIndex( file, pIndex, m_nbFace, setVertice, beginFace, sizeFace, 3	);

    // Setup
    setupMesh( pData, pIndex	);

    // Cleanup
    file.close();

    delete pPosition;
    delete pNormal;
    delete pColor;
    delete pUv;
    delete pData;
    delete pIndex;
}

/******************************************************************************/

void ObjMeshNode::setPositions(size_t& out_sizeData, size_t& out_beginData, const size_t in_pos)
{
    out_sizeData++;
    if(out_beginData == 0u-1)
        out_beginData = in_pos;
}

void ObjMeshNode::readData(std::istream& in_stream, GLfloat* out_pData, const size_t in_beginData,
                            const size_t in_length, const size_t in_valuePerLine)
{
    std::string			readStr		= "";
    std::string			readLine	= "";
    std::istringstream	lineStream;

    in_stream.seekg(in_beginData);

    for(size_t line = 0 ; line < in_length; line++	)
    {
        getline( in_stream, readLine	);
        lineStream.str( readLine	);

        lineStream >> readStr;

        for( size_t value = 0; value < in_valuePerLine; value++	)
            lineStream >> out_pData[ line * in_valuePerLine + value	];

        lineStream.clear();
    }
}

void ObjMeshNode::readVertice(std::istream& in_stream, VertexSet& out_setVertice, const size_t in_beginVertice,
                                const size_t in_length, const size_t in_valuePerLine)
{
    std::string			readStr		= "";
    std::string			readLine	= "";
    std::istringstream	lineStream;

    in_stream.seekg(in_beginVertice);

    for(size_t line = 0 ; line < in_length; line++	)
    {
        getline( in_stream, readLine	);
        lineStream.str(readLine);

        lineStream >> readStr;

        for( size_t value = 0; value < in_valuePerLine; value++	)
        {
            lineStream >> readStr;
            out_setVertice.insert( readVertex( readStr	)	);
        }

        lineStream.clear();
    }
}

Vertex ObjMeshNode::readVertex(std::string const& in_strVertex)
{
    std::string			readStr			= " ";
    std::istringstream	vertexStream	( readStr	);
    size_t				pos				= 0;
    size_t				begPos			= 0;
    size_t				findPos			= in_strVertex.find_first_of("/");
    GLushort			ids[3]			= { 0, 0, 0	};

    for(pos = 0; pos < 3; pos++	)
    {
        if	( findPos == std::string::npos	)
            readStr	= in_strVertex.substr( begPos	);
        else
            readStr	= in_strVertex.substr( begPos, findPos - begPos	);

        if	( readStr == ""	)
            readStr = "1";

        vertexStream.str( readStr	);
        vertexStream >> ids[pos];
        ids[pos]--;

        begPos	= ++findPos;
        findPos = in_strVertex.find_first_of( "/", findPos	);

        vertexStream.clear();
    }

    return std::make_tuple( ids[0], ids[1], ids[2]	);
}

void ObjMeshNode::readIndex(std::istream& in_stream, GLushort* out_pIndex, size_t& out_sizeIndex, VertexSet const& in_setVertice,
                            const size_t in_beginIndex, const size_t in_length, const size_t in_valuePerLine)
{
    std::string			readStr		= "";
    std::string			readLine	= "";
    std::istringstream	lineStream;

    in_stream.seekg( in_beginIndex	);

    for(size_t line = 0 ; line < in_length; line++	)
    {
        getline( in_stream, readLine	);
        lineStream.str( readLine	);

        lineStream >> readStr;

        for( size_t value = 0; value < in_valuePerLine; value++	)
        {
            lineStream >> readStr;
            VertexSet::iterator itVertex( in_setVertice.find( readVertex( readStr	)	)	);

            size_t i = 0;
            for(VertexSet::iterator it = in_setVertice.begin(); it != itVertex && it != in_setVertice.end(); it++)
            {
                i++;
            }

            // Obj faces are couter-clockwise, we want clockwise
            out_pIndex[in_valuePerLine * line + in_valuePerLine - value - 1] = i;
        }

        lineStream.clear();
    }
}

template<typename T, typename ... Trest>
void ObjMeshNode::fillData(T* out_pData, const T in_arg, const Trest ... in_rest)
{
    fillData( out_pData, in_arg	);
    fillData( out_pData + 1, in_rest ...	);
}

template<typename T>
void ObjMeshNode::fillData(T* out_pData, const T in_arg)
{
    *out_pData = in_arg;
}
