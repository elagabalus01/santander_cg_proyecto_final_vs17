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

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Camera
Camera camera( glm::vec3( -50.0f, 0.0f, 0.0f ), glm::vec3(0.0f,1.0f,0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//Animation variables
GLfloat doorRotation = 0.0f;

int main( )
{
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
    
	//Cargando modelos

	Model casa = (char*) "Models/proyecto/casa/casa.obj";
	Model silla = (char*) "Models/proyecto/silla/silla.obj";
	Model lampara = (char*) "Models/proyecto/lampara/lampara.obj";
	//Model compu = (char*) "Models/proyecto/compu/old_monitor.obj";
	Model escritorio = (char*) "Models/proyecto/escritorio/escritorio.obj";
	Model cama = (char*) "Models/proyecto/cama/cama.obj";
    Model room = (char*) "Models/proyecto/room/room.obj";

//    Model room = (char*) "Models/proyecto/room_test/room.obj";
    Model puerta = (char*) "Models/proyecto/puerta/puerta.obj";
    Model teclado = (char*) "Models/proyecto/compu/teclado.obj";
    Model nave = (char*) "Models/proyecto/nave/nave.obj";

    Poster atomo = Poster((char*)"Models/proyecto/poster/atomo.png",vertices,indices,sizeof(vertices),sizeof(indices));
    atomo.makePoster();

    Poster einstein = Poster((char*)"Models/proyecto/poster/einstein.png", vertices2, indices2, sizeof(vertices2), sizeof(indices2));
    einstein.makePoster();

    // Draw in wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    // Game loop
    while( !glfwWindowShouldClose( window ) )
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime( );
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
        
        // Draw the loaded model
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        casa.Draw(shader);

		
		//if (camera.position.x < -7.0f) {
		
		//}
		//else {
        
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(4.9f, 2.8f, 5.0f)); // Translate it down a bit so it's at the center of the scene
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			lampara.Draw(shader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			//compu.Draw(shader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(4.51f, 0.0f, 4.0f)); // Translate it down a bit so it's at the center of the scene
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			escritorio.Draw(shader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(2.5f, 0.0f, 4.0f)); // Translate it down a bit so it's at the center of the scene
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			silla.Draw(shader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.30f));
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			cama.Draw(shader);

			model = glm::mat4(1);
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

			room.Draw(shader);
			atomo.drawPoster();
            einstein.drawPoster();
        
		//}
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-5.70f, 0.0f, -1.4f));
        model = glm::rotate(model,glm::radians(doorRotation),glm::vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        puerta.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.10f, 1.63f, 4.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        teclado.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 6.5f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        nave.Draw(shader);
		////ourModel.Draw( shader );
		////Drawing models
		////ourModel2.Draw(shader);
		//
		//ourModel3.Draw(shader);
		//modelos[selection].Draw(shader);
		
		//chair.Draw(shader);
		//lampara.Draw(shader);
		
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
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
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
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

    if (GLFW_KEY_1 == key)
    {
        doorRotation = 90.0f;
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

