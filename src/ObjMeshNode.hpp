
#ifndef	OBJ_MESH_NODE_HPP
#define	OBJ_MESH_NODE_HPP

#include <set>
#include <tuple>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "SceneNode.hpp"

/******************************************************************************/

typedef std::tuple<GLushort, GLushort, GLushort>	Vertex;
typedef std::set<Vertex>							VertexSet;

class ObjMeshNode : public SceneNode
{

// Inits
public:

                    ObjMeshNode			(std::string const& in_objPath);

    virtual			~ObjMeshNode		();

// Render
public:

    virtual	void	render				(MatrixStack& in_stack);

// Utils
protected:

    void			setupMesh			(GLfloat* in_pData, GLushort* in_pIndex);
    void			loadObj				(std::string const& in_objPath);

// loadObj Utils
protected:

    void			setPositions		(size_t& out_sizeData, size_t& out_beginData, const size_t in_pos);

    void			readData			(std::istream& in_stream, GLfloat* out_pData, const size_t in_beginData,
                                         const size_t in_length, const size_t in_valuePerLine);

    void			readVertice			(std::istream& in_stream, VertexSet& out_setVertice, const size_t in_firstLine,
                                         const size_t in_length, const size_t in_valuePerLine);

    Vertex			readVertex			(std::string const& in_strVertex);

    void			readIndex			(std::istream& in_stream, GLushort* out_pIndex, size_t& out_sizeIndex, VertexSet const& in_setVertice,
                                         const size_t in_beginIndex, const size_t in_length, const size_t in_valuePerLine);

    template<typename T, typename ... Trest>
    void			fillData			(T* out_pData, const T in_arg, const Trest ... in_data);

    template<typename T>
    void			fillData			(T* out_pData, const T in_arg);

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

