
#include <cassert>
#include <iostream>
#include <fstream>

#include "Engine.hpp"

#include "IndexedDrawNode.hpp"
#include "ArrayDrawNode.hpp"
#include "ObjMeshNode.hpp"

/******************************************************************************/

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
    m_modelViewProjectionUniform	= glGetUniformLocation(m_shader, "modelViewProjection");
    m_modelViewUniform				= glGetUniformLocation(m_shader, "modelView");
    m_lightColorUniform				= glGetUniformLocation(m_shader, "lightColor");
    m_lightDirectionUniform			= glGetUniformLocation(m_shader, "lightDirection");
    m_ambiantLightColorUniform		= glGetUniformLocation(m_shader, "ambiantLightColor");

    // Camera & Perspective
    m_camPosition   = glm::vec3(0.0f, 2.0f, 7.0f);
    m_camera		= glm::lookAt(
        m_camPosition,
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f)
    );

    // Lighting
    m_lightColor			= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    m_lightDirection		= m_camera * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    m_ambiantLightColor		= glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

    glUseProgram	( m_shader );
    glUniform4fv	( m_lightColorUniform,			1, glm::value_ptr(m_lightColor)			);
    glUniform4fv	( m_lightDirectionUniform,		1, glm::value_ptr(m_lightDirection)		);
    glUniform4fv	( m_ambiantLightColorUniform,	1, glm::value_ptr(m_ambiantLightColor)	);
    glUseProgram	( 0 );

    SceneNode::setModelViewProjectionUniform	( m_modelViewProjectionUniform	);
    SceneNode::setModelViewUniform				( m_modelViewUniform			);
    SceneNode::setLightColorUniform				( m_lightColorUniform 			);
    SceneNode::setLightDirectionUniform			( m_lightDirectionUniform		);
    SceneNode::setPositionShader				( 0	);
    SceneNode::setNormalShader					( 1	);
    SceneNode::setColorShader					( 2 );
    SceneNode::setUvShader						( 3	);
    SceneNode::setViewMatrix					( &m_camera	);
    SceneNode::setProjectionMatrix				( &m_perspective	);

    resize(m_window.getSize().x, m_window.getSize().y);

    // Z Buffer
    glEnable		( GL_DEPTH_TEST );
    glDepthMask		( GL_TRUE		);
    glDepthFunc 	( GL_LESS       );
    glDepthRange	( 0.0f, 1.0f	);

    // Face culling
    glEnable		( GL_CULL_FACE	);
    glCullFace		( GL_BACK		);
    glFrontFace		( GL_CW			);

    // Set scene content
    m_pSceneRoot	= new SceneNode			();
    m_pScenePyramid	= new ObjMeshNode	( false	);
    m_pSceneCube	= new ObjMeshNode	( true	);

    m_pScenePyramid	->setTranslation	( glm::vec3( 2.0f, 0.0f, 0.0f)	);
    m_pSceneCube	->setTranslation	( glm::vec3(-2.0f, 0.0f, 0.0f)	);

    m_pScenePyramid	->setScale	( glm::vec3(1.0f)	);
    m_pSceneCube	->setScale	( glm::vec3(1.0f)	);

    m_pSceneRoot	->addChild	( m_pScenePyramid	);
    m_pSceneRoot	->addChild	( m_pSceneCube		);

    m_rotation		= 0.0f;
}

void Engine::release()
{
    glDeleteProgram	( m_shader	);

    delete m_pSceneRoot;

    m_pSceneRoot	= NULL;
    m_pScenePyramid	= NULL;
    m_pSceneCube	= NULL;
}

/******************************************************************************/

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

/******************************************************************************/

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
    // update position
    m_rotation		= 1.0f * m_frameElapsedTime.asSeconds();

    m_pSceneRoot	->rotate	( -m_rotation,	glm::vec3(0.0f, 1.0f, 0.0f)	);

    m_pScenePyramid	->rotate	( -m_rotation,	glm::vec3(0.0f, 1.0f, 0.0f)	);
    m_pSceneCube	->rotate	( -m_rotation,	glm::vec3(0.0f, 1.0f, 0.0f)	);
}

void Engine::render()
{
    // Clear buffers
    glClearColor        	( 0.f, 0.f, 0.f, 0.f    );
    glClearDepth        	( 1.f					);
    glClear             	( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Shader to use
    glUseProgram        	( m_shader  );

    // Draw
    m_pSceneRoot->render	( m_matrixStack	);

    // Cleanup
    glUseProgram  			( 0 );

    // Draw the FPS meter with SFML
    //m_window.pushGLStates();
    //m_window.draw(m_fpsMeter);
    //m_window.popGLStates();

    // Show the rendered frame
    m_window.display();
}

/******************************************************************************/

void Engine::resize(uint32_t in_uiWidth, uint32_t in_uiHeight)
{
    // Our OpenGl side
    m_perspective	= glm::perspective(
        70.f,
        in_uiWidth / (float) in_uiHeight,
        0.1f,
        100.f
    );

    glViewport(0, 0, (GLsizei) in_uiWidth, (GLsizei) in_uiHeight);

    // SFML side
    m_window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, in_uiWidth, in_uiHeight)));
}

/******************************************************************************/

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

/******************************************************************************/

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

/******************************************************************************/
