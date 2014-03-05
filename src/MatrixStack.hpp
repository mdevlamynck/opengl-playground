
#ifndef	MATRIX_STACK_HPP
#define	MATRIX_STACK_HPP

#include <stack>
#include <GL/glew.h>
#include <glm/glm.hpp>

/******************************************************************************/

class MatrixStack
{

// Inits
public:

            MatrixStack		();
    virtual	~MatrixStack	();

// Matrix Management
public:

    void		push	();
    void		pop		();
    glm::mat4&	get		();

/******************************************************************************/

private:

    // Stack
    glm::mat4				m_current;
    std::stack<glm::mat4>	m_stack;

};

/******************************************************************************/

#endif	// MATRIX_STACK_HPP
