
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

namespace ShaderID
{
    enum Type
    {
        Position	= 0,
        Color		= 1,

        Nb
    };
}

class Engine
{

public:

            Engine	();
    virtual	~Engine	();

    void	init	();
    void	release	();

    void	run		();

    void	event	();
    void	logic	();
    void	render	();

protected:

    void	resize			(uint32_t in_uiWidth, uint32_t in_uiHeight);

    void	updateFpsCount	();

    GLuint	createShader	(GLenum in_eShaderType, const std::string in_path);
    GLuint	createProgram	(std::vector<GLuint>& in_shaders);
    void	loadShader		(const std::string in_path, std::string& out_code);

private:

    sf::RenderWindow	m_window;

    sf::Event			m_event;
    bool				m_bQuit;

    sf::Clock			m_frameClock;
    sf::Time			m_totalTime;
    sf::Time			m_frameElapsedTime;
    sf::Time			m_frameGoalTime;
    sf::Time			m_frameActualTime;
    float				m_fps;
    sf::Font			m_fpsFont;
    sf::Text			m_fpsMeter;
    bool				m_bLockFrameRate;

    glm::vec3			m_camPosition;
    glm::vec3			m_position;
    GLfloat				m_rotation;
    glm::mat4			m_transform;
    glm::mat4			m_camera;
    glm::mat4			m_perspective;
    glm::mat4			m_viewProj;

    GLuint				m_shader;
    GLuint				m_meshBufObj;
    GLuint				m_transfUniform;
    GLuint				m_viewProjUniform;

};

#endif	// ENGINE_HPP

