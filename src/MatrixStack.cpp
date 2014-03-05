
#include "MatrixStack.hpp"

/******************************************************************************/

MatrixStack::MatrixStack()
{
}

MatrixStack::~MatrixStack()
{
}

/******************************************************************************/

void MatrixStack::push()
{
    m_stack.push(m_current);
}

void MatrixStack::pop()
{
    m_current = m_stack.top();
    m_stack.pop();
}

glm::mat4& MatrixStack::get()
{
    return m_current;
}

/******************************************************************************/
