
#ifndef	SCENE_NODE_HPP
#define	SCENE_NODE_HPP

#include <set>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "MatrixStack.hpp"

/******************************************************************************/

class SceneNode
{

// Inits
public:

                    SceneNode				();
    virtual			~SceneNode				();

// Render
public:

    virtual void	render					(MatrixStack& in_stack);

// Children Management
public:

    void			addChild				(SceneNode* in_pSceneNode);
    void			removeChild				(SceneNode* in_pSceneNode);
    void			removeAllChildren		();

// Matrix Modifications
public:

    void			translate				(glm::vec3 in_translation);
    void			rotate					(float in_angle, glm::vec3 in_axis);
    void			scale					(glm::vec3 in_factors);

    void			setTranslation			(glm::vec3 in_translation);
    void			setRotation				(float in_angle, glm::vec3 in_axis);
    void			setScale				(glm::vec3 in_factors);

// Common Rendered Nodes Data
public:

    static void		setMvpUniform			(GLuint in_mvpUniform);

    static void		setPositionShader		(GLuint in_positionShader);
    static void		setColorShader			(GLuint in_colorShader);

    static void 	setCameraPerspective	(glm::mat4* in_pCameraPerspective);


/******************************************************************************/

protected:

    // 3D Transformations
    glm::mat4	m_translation;
    glm::mat4	m_rotation;
    glm::mat4	m_scale;
    glm::mat4	m_transform;

    // Children
    std::set<SceneNode*>	m_children;

    // Common Rendered Nodes Data
    static GLuint		s_mvpUniform;

    static GLuint		s_positionShader;
    static GLuint		s_colorShader;

    static glm::mat4*	s_pCameraPerspective;

};

/******************************************************************************/

#endif	// SCENE_NODE_HPP
