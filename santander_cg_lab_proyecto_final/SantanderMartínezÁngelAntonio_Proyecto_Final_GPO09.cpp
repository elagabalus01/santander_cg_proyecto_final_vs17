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
#include "Modelo_Material.h"

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
const GLuint WIDTH = 850, HEIGHT = 480;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Camera
Camera camera( glm::vec3( -25.0f, 5.0f, 0.0f ), glm::vec3(0.0f,1.0f,0.0f));
bool keys[1024];
double lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
int num_steps_robot_animation = 40;
//Animation variables
GLboolean active_nave_animation = false;
AnimationCircuit nave_animation = AnimationCircuit();

KeyFrameAnimation puerta_animacion = KeyFrameAnimation(250);
KeyFrameAnimation silla_animacion = KeyFrameAnimation(60);
KeyFrameAnimation nave_animacion = KeyFrameAnimation(30);

//Head Animation
//Animation object
KeyFrameAnimation head_animation = KeyFrameAnimation(num_steps_robot_animation);
//Animation path
char* head_animation_file = (char*) "animaciones/robot/cabeza.animacion";

//body Animation
//Animation object
KeyFrameAnimation body_animation = KeyFrameAnimation(num_steps_robot_animation);
//Animation path
char* body_animation_file = (char*) "animaciones/robot/cuerpo.animacion";

//Right arm animation
KeyFrameAnimation right_arm_animation = KeyFrameAnimation(num_steps_robot_animation);
//Animation path
char* right_arm_animation_file = (char*) "animaciones/robot/brazo_derecho.animacion";

//Left arm animation
KeyFrameAnimation left_arm_animation = KeyFrameAnimation(num_steps_robot_animation);
//Animation path
char* left_arm_animation_file = (char*) "animaciones/robot/brazo_izquiedo.animacion";

//Right leg animation
KeyFrameAnimation right_leg_animation = KeyFrameAnimation(num_steps_robot_animation);
//Animation path
char* right_leg_animation_file = (char*) "animaciones/robot/pierna_derecha.animacion";

//Left leg animation
KeyFrameAnimation left_leg_animation = KeyFrameAnimation(num_steps_robot_animation);
//Animation path
char* left_leg_animation_file = (char*) "animaciones/robot/pierna_izquieda.animacion";

int main( )
{
    //Configurando las animaciones
    
    puerta_animacion.loadAnimation((char*) "animaciones/puerta.animacion");
    
    silla_animacion.loadAnimation((char*) "animaciones/silla.animacion");
    
    nave_animacion.loadAnimation((char*) "animaciones/nave.animacion");

    //Animacion robot
    head_animation.loadAnimation(head_animation_file);
    body_animation.loadAnimation(body_animation_file);
    right_arm_animation.loadAnimation(right_arm_animation_file);
    left_arm_animation.loadAnimation(left_arm_animation_file);
    right_leg_animation.loadAnimation(right_leg_animation_file);
    left_leg_animation.loadAnimation(left_leg_animation_file);
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
    Shader material_shader("Shaders/pruebas/cel_material_dirlight.vs", "Shaders/pruebas/cel_material_dirlight.frag");
	
    //Cargando modelos
	Model casa = (char*) "Models/proyecto/casa/casa.obj";
	Model lampara = (char*) "Models/proyecto/lampara/lampara.obj";
	Model compu = (char*) "Models/proyecto/compu/pantalla.obj";
	Model escritorio = (char*) "Models/proyecto/escritorio/escritorio.obj";
    Model silla = (char*) "Models/proyecto/silla/silla.obj";
	Model cama = (char*) "Models/proyecto/cama/cama.obj";
    Model room = (char*) "Models/proyecto/room/room.obj";
    Model puerta = (char*) "Models/proyecto/puerta/puerta.obj";
    Model teclado = (char*) "Models/proyecto/compu/teclado.obj";
    Model nave = (char*) "Models/proyecto/nave/nave.obj";
    Model piso = (char*) "Models/proyecto/casa/piso.obj";

    //MODELO DEL ROBOT
    Modelo_Material cuerpo = (char*) "Models/proyecto/robot/body.obj";
    Modelo_Material cabeza = (char*) "Models/proyecto/robot/head.obj";
    Modelo_Material brazo = (char*) "Models/proyecto/robot/right_arm.obj";
    Modelo_Material pierna = (char*) "Models/proyecto/robot/leg.obj";

    //SKYBOx
    SkyBox fondo((char*)"SkyBox/right.jpg", (char*)"SkyBox/left.jpg",(char*)"SkyBox/top.jpg",
        (char*)"SkyBox/bottom.jpg", (char*)"SkyBox/back.jpg", (char*)"SkyBox/front.jpg",
        skyboxVertices_1,sizeof(skyboxVertices_1));

    Poster atomo = Poster((char*)"Models/proyecto/poster/atomo.png", vertices, indices, sizeof(vertices), sizeof(indices));

    Poster einstein = Poster((char*)"Models/proyecto/poster/einstein.png", vertices2, indices2, sizeof(vertices2), sizeof(indices2));

    // Draw in wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    // Game loop
    while( !glfwWindowShouldClose( window ) ){
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
        
        // Draw the loaded model
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        casa.Draw(shader);
        piso.Draw(shader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.9f, 2.8f, 5.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		lampara.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.37f, 2.81f, 2.34f));
        model = glm::rotate(model, glm::radians(20.0f),glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        compu.Draw(shader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.51f, 0.0f, 4.0f)); // Translate it down a bit so it's at the center of the scene
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		escritorio.Draw(shader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 4.0f)); // silla
        silla_animacion.animacion(&model); //Asociar animacion
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
        
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-5.70f, 0.0f, -1.4f));
        puerta_animacion.animacion(&model); //Asociar animacion
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        puerta.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.10f, 1.63f, 4.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        teclado.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 6.5f, 0.0f));
        if (active_nave_animation) {
            nave_animation.animacionCircuito(&model);
        }
        nave_animacion.animacion(&model);

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        nave.Draw(shader);

		//DIBUJANDO EL ROBOT
        //USING OTHER SHADER
        material_shader.Use();
        glUniform3f(glGetUniformLocation(material_shader.Program, "light.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(material_shader.Program, "light.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(material_shader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

        glUniform3f(glGetUniformLocation(material_shader.Program, "light.direction"), -1.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(material_shader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.29f, 2.30f, 0.118f));
        body_animation.animacion(&model);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cuerpo.Draw(material_shader);

        glm::mat4 modelAux = model;

        model = modelAux;
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
        head_animation.animacion(&model);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cabeza.Draw(material_shader);

        //Brazo derecho
        model = modelAux;
        model = glm::translate(model, glm::vec3(0.0f, 1.34f, 0.93f));
        right_arm_animation.animacion(&model);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        brazo.Draw(material_shader);

        //Brazo izquierdo
        model = modelAux;
        model = glm::translate(model, glm::vec3(0.0f, 1.34f, -0.93f));
        model = glm::scale(model, glm::vec3(-1.0f, -1.0f, -1.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        left_arm_animation.animacion(&model);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        brazo.Draw(material_shader);

        //Piernas
        //Izquieda
        model = modelAux;
        model = glm::translate(model, glm::vec3(0.0f, -0.21f, -0.45f));
        left_leg_animation.animacion(&model);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pierna.Draw(material_shader);

        //Derecha
        model = modelAux;
        model = glm::translate(model, glm::vec3(0.0f, -0.21f, 0.45f));
        model = glm::scale(model, glm::vec3(-1.0f, -1.0f, -1.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        right_leg_animation.animacion(&model);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pierna.Draw(material_shader);

        glBindVertexArray(0);
        //SKYBOX
        SkyBoxshader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
        glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        fondo.Draw();
        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
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
        active_nave_animation = !active_nave_animation;
        if (!active_nave_animation) {
            nave_animation.reset();
        }
    }
    if (keys[GLFW_KEY_2])
    {
        puerta_animacion.play = !puerta_animacion.play;
        if (puerta_animacion.play) {
            puerta_animacion.reset();
            puerta_animacion.interpolation();
        }
    }
    if (keys[GLFW_KEY_3]) {
        nave_animacion.play = !nave_animacion.play;
        if (nave_animacion.play) {
            nave_animacion.reset();
            nave_animacion.interpolation();
        }
    }
    if (keys[GLFW_KEY_4]) {
        silla_animacion.play = !silla_animacion.play;
        if (silla_animacion.play) {
            silla_animacion.reset();
            silla_animacion.interpolation();
        }
    }
    if (keys[GLFW_KEY_5]) {
        head_animation.play = !head_animation.play;
        body_animation.play = !body_animation.play;
        right_arm_animation.play = !right_arm_animation.play;
        left_arm_animation.play = !left_arm_animation.play;
        right_leg_animation.play = !right_leg_animation.play;
        left_leg_animation.play = !left_leg_animation.play;

        if (body_animation.play) {
            head_animation.start();
            body_animation.start();
            right_arm_animation.start();
            left_arm_animation.start();
            right_leg_animation.start();
            left_leg_animation.start();
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
