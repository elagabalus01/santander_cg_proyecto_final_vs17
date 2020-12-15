// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
//#include "Modelo_Material.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL2/SOIL2.h>
#include "modelos.h"
#include "SkyBox.h"
#include "Animations.h"


// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Camera
Camera camera( glm::vec3( -5.0f, 4.0f, 0.0f ), glm::vec3(0.0f,1.0f,0.0f));
bool keys[1024];
double lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//Animation variables
KeyFrameAnimation animacion = KeyFrameAnimation();
//Model Position  for animation
float pos_x, pos_y, pos_z, rot_x,rot_y,rot_z;

int main( ){
    //animacion.loadAnimation((char*)"prueba.animacion");
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Santander Martinez Angel Antonio - Proyecto final", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    

    // Setup and compile our shaders
    //Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");
    Shader shader( "Shaders/pruebas/cel_dirlight.vs", "Shaders/pruebas/cel_dirlight.frag" );
    Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
    
	//Cargando modelos
    Model room = (char*) "Models/proyecto/room/room.obj";
    Model modelo = (char*) "Models/proyecto/nave/nave.obj";

    // Draw in wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
   

    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    // Game loop
    while( !glfwWindowShouldClose( window ) )
    {
        // Set frame time
        GLfloat currentFrame = (GLfloat)glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check and call events
        glfwPollEvents( );
        DoMovement( );
        
        // Clear the colorbuffer
        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        shader.Use( );

        glUniform3f(glGetUniformLocation(shader.Program, "light.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "light.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

        glUniform3f(glGetUniformLocation(shader.Program, "light.direction"), 1.0f,-1.0f,0.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        glm::mat4 model(1);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        room.Draw(shader);
        
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 6.5f, 0.0f));
        model = glm::translate(model, glm::vec3(pos_x,pos_y,pos_z));
        model = glm::rotate(model,glm::radians(rot_x),glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
        
        if (animacion.play) {
            animacion.animacion(&model);
        }
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        modelo.Draw(shader);

        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W])
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S])
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
    /*Agregando movimiento en el eje y*/

    if (keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
    if (keys[GLFW_KEY_1])
    {
        animacion.saveFrame((char*)"prueba.animacion",pos_x, pos_y, pos_z, rot_x, rot_y, rot_z);
    }
    if (keys[GLFW_KEY_2])
    {
        animacion.play = !animacion.play;
        if (animacion.play) {
            animacion.reset();
            animacion.interpolation();
            puts("Comienza a correr la animacion");
        }
        
    }
    
    if (GLFW_KEY_Q == key)
    {
        camera.rotateYaw(-5.0f);
    }
    if (GLFW_KEY_E == key)
    {
        camera.rotateYaw(5.0f);
    }
    if (keys[GLFW_KEY_I]) {
        pos_x += 0.2;
    }
    if (keys[GLFW_KEY_K]) {
        pos_x -= 0.2;
    }
    if (keys[GLFW_KEY_J]) {
        pos_z += 0.2;
    }
    if (keys[GLFW_KEY_L]) {
        pos_z -= 0.2;
    }

    if (keys[GLFW_KEY_U]) {
        pos_y += 0.2;
    }
    if (keys[GLFW_KEY_O]) {
        pos_y -= 0.2;
    }

}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

