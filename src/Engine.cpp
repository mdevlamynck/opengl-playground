
#include <cassert>
#include <iostream>
#include <fstream>
#include "Engine.hpp"

Engine::Engine()
    : m_window(
            sf::VideoMode(800, 600),
            "OpenGl sample",
            sf::Style::Default,
            sf::ContextSettings(24, 8, 4, 3, 3)
    )
    , m_bQuit			( false	)
    , m_frameGoalTime	( sf::milliseconds(16) )
    , m_bLockFrameRate	( false	)
{
    assert( m_fpsFont.loadFromFile("/usr/share/fonts/TTF/FreeMono.ttf") && "Font not found" );
    m_fpsMeter.setFont(m_fpsFont);

    glewInit();
}

Engine::~Engine()
{
}

void Engine::init()
{
    // Load the shaders
    std::vector<GLuint> shaderList;

    shaderList.push_back( createShader(GL_VERTEX_SHADER,	"../../src/vertex.glsl")	);
    shaderList.push_back( createShader(GL_FRAGMENT_SHADER,	"../../src/fragment.glsl")	);

    m_shader            = createProgram(shaderList);

    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

    // Set uniforms
    m_viewProjUniform	= glGetUniformLocation(m_shader, "viewProj");

    // Camera & Perspective
    m_camPosition   = glm::vec3(0.0f, 0.0f, 5.0f);
    m_camera		= glm::lookAt(
        m_camPosition,
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f)
    );

    resize(m_window.getSize().x, m_window.getSize().y);

    // Z Buffer
    glEnable		( GL_DEPTH_TEST );
    glDepthMask		( GL_TRUE		);
    glDepthFunc 	( GL_LESS       );
	glDepthRange	( 0.0f, 1.0f	);

    // Face culling
    glEnable	( GL_CULL_FACE	);
    glCullFace	( GL_BACK		);
    glFrontFace	( GL_CW			);
}

void Engine::release()
{
    glDeleteProgram	( m_shader);
}

void Engine::run()
{
    m_frameClock.restart();

    while(!m_bQuit)
    {
        event	();
        logic   ();
        render	();

        updateFpsCount();
    }
}

void Engine::event()
{
    while(m_window.pollEvent(m_event))
    {
        if		(m_event.type == sf::Event::Closed)
            m_bQuit = true;

        else if	(m_event.type == sf::Event::Resized)
            resize(m_event.size.width, m_event.size.height);
    }
}

void Engine::logic()
{
}

void Engine::render()
{
    // Clear buffers
    glClearColor        ( 0.f, 0.f, 0.f, 0.f    );
    glClearDepth        ( 1.f					);
    glClear             ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Shader to use
    glUseProgram        ( m_shader  );

    // Cleanup
    glUseProgram      	( 0 );

    // Update data given to shader
    glUniformMatrix4fv	( m_viewProjUniform,	1, GL_FALSE, glm::value_ptr(m_viewProj)	    );

    // Draw the FPS meter with SFML
    m_window.pushGLStates();
    m_window.draw(m_fpsMeter);
    m_window.popGLStates();

    // Show the rendered frame
    m_window.display();
}

void Engine::resize(uint32_t in_uiWidth, uint32_t in_uiHeight)
{
    // Our OpenGl side
    m_perspective	= glm::perspective(
        70.f,
        in_uiWidth / (float) in_uiHeight,
        0.1f,
        100.f
    );

    m_viewProj = m_perspective * m_camera;

    glViewport(0, 0, (GLsizei) in_uiWidth, (GLsizei) in_uiHeight);

    // SFML side
    m_window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, in_uiWidth, in_uiHeight)));
}

void Engine::updateFpsCount()
{
    m_frameElapsedTime = m_frameClock.restart();
    if(m_bLockFrameRate)
    {
        m_frameActualTime = m_frameGoalTime - m_frameElapsedTime;
        if(m_frameActualTime.asMilliseconds() > 0)
        {
            sf::sleep(m_frameActualTime);
            m_fps = 1.f / m_frameGoalTime.asSeconds();
        }
        else
            m_fps = 1.f / m_frameElapsedTime.asSeconds();
    }
    else
        m_fps = 1.f / m_frameElapsedTime.asSeconds();

    std::ostringstream oss;
    oss << (int) m_fps;
    m_fpsMeter.setString(oss.str());
}

GLuint Engine::createShader(GLenum in_eShaderType, const std::string in_path)
{
    // Load shader
    GLuint		shader	= glCreateShader(in_eShaderType);
    std::string	shaderCode;
    loadShader(in_path, shaderCode);
    const char*	cShaderCode	= shaderCode.c_str();
    glShaderSource(shader, 1, &cShaderCode, NULL);

    // Compile it
    glCompileShader(shader);

    // Check errors and print if any
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

        const char *strShaderType = NULL;
        switch(in_eShaderType)
        {
            case GL_VERTEX_SHADER:		strShaderType = "vertex";	break;
            case GL_GEOMETRY_SHADER:	strShaderType = "geometry"; break;
            case GL_FRAGMENT_SHADER:	strShaderType = "fragment"; break;
        }

        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        delete[] strInfoLog;
    }

    return shader;
}

GLuint Engine::createProgram(std::vector<GLuint>& in_shaders)
{
    GLuint program = glCreateProgram();

    for(size_t iLoop = 0; iLoop < in_shaders.size(); iLoop++)
        glAttachShader(program, in_shaders[iLoop]);

    glLinkProgram(program);

    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }

    for(size_t iLoop = 0; iLoop < in_shaders.size(); iLoop++)
        glDetachShader(program, in_shaders[iLoop]);

    return program;
}

void Engine::loadShader(const std::string in_path, std::string& out_code)
{
    std::ifstream file(in_path.c_str(), std::ios::in);
    std::string line = "";

    if(file.is_open())
    {
        while(getline(file, line))
            out_code += "\n" + line;
        file.close();
    }
    else
    {
        std::cerr << "Unable to open " << in_path << std::endl;
        getchar();
    }
}
