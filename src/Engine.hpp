
#ifndef	ENGINE_HPP
#define	ENGINE_HPP

#include <cinttypes>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SceneNode.hpp"
#include "MatrixStack.hpp"

/******************************************************************************/

namespace ShaderID
{
    enum Type
    {
        Position	= 0,
        Color		= 1,

        Nb
    };
}

namespace ObjectId
{
    enum Type
    {
        Torus	= 0,
        Suzane	= 1,

        Nb
    };
}

/******************************************************************************/

class Engine
{

// Inits
public:

            Engine	();
    virtual	~Engine	();

    void	init	();
    void	release	();

// Run
    void	run		();

// Update frame
protected:

    void	event	();
    void	logic	();
    void	render	();

// Window event
protected:
    void	resize			(uint32_t in_uiWidth, uint32_t in_uiHeight);

// FPS update
protected:
    void	updateFpsCount	();

// Utils
protected:

    GLuint	createShader	(GLenum in_eShaderType, const std::string in_path);
    GLuint	createProgram	(std::vector<GLuint>& in_shaders);
    void	loadShader		(const std::string in_path, std::string& out_code);

/******************************************************************************/

private:

    // Window
    sf::RenderWindow	m_window;

    // Events
    sf::Event			m_event;
    bool				m_bQuit;

    // Framerate calc / control / display
    sf::Clock			m_frameClock;
    sf::Time			m_frameElapsedTime;
    sf::Time			m_frameGoalTime;
    sf::Time			m_frameActualTime;
    float				m_fps;
    sf::Font			m_fpsFont;
    sf::Text			m_fpsMeter;
    bool				m_bLockFrameRate;

    // Scene content
    SceneNode*			m_pSceneRoot;
    SceneNode*			m_pSceneLight;
    SceneNode*			m_pSceneSuzanne;
    MatrixStack			m_matrixStack;
    float				m_rotation;

    // View / Projection
    glm::vec3			m_camPosition;
    glm::mat4			m_camera;
    glm::mat4			m_perspective;

    // Lighting
    glm::vec4			m_lightColor;
    glm::vec4			m_lightPosition;
    glm::vec4			m_ambiantLightColor;

    // Handles
    GLuint				m_shader;
    GLuint				m_modelViewProjectionUniform;
    GLuint				m_modelViewUniform;
    GLuint				m_lightColorUniform;
    GLuint				m_lightPositionUniform;
    GLuint				m_ambiantLightColorUniform;

};

/******************************************************************************/

#endif	// ENGINE_HPP

