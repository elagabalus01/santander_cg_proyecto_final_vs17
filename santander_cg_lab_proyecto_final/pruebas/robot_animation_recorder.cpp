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
int num_steps = 50;
//Head Animation
//Animation object
KeyFrameAnimation head_animation = KeyFrameAnimation(num_steps);
//Animation path
char* head_animation_file = (char*) "animaciones/robot/cabeza.animacion";
//Head position
float head_rot_y=0.0f;

//body Animation
//Animation object
KeyFrameAnimation body_animation = KeyFrameAnimation(num_steps);
//Animation path
char* body_animation_file = (char*) "animaciones/robot/cuerpo.animacion";
//Body position
float body_rot_y = 0.0f;
float body_pos_x = 0.0f;
float body_pos_z = 0.0f;

//Right arm animation
KeyFrameAnimation right_arm_animation = KeyFrameAnimation(num_steps);
//Animation path
char* right_arm_animation_file = (char*) "animaciones/robot/brazo_derecho.animacion";
//Arm position
float righ_arm_rot_z = 0.0f;

//Left arm animation
KeyFrameAnimation left_arm_animation = KeyFrameAnimation(num_steps);
//Animation path
char* left_arm_animation_file = (char*) "animaciones/robot/brazo_izquiedo.animacion";
//Arm position
float left_arm_rot_z = 0.0f;


//Right leg animation
KeyFrameAnimation right_leg_animation = KeyFrameAnimation(num_steps);
//Animation path
char* right_leg_animation_file = (char*) "animaciones/robot/pierna_derecha.animacion";
//leg position
float righ_leg_rot_z = 0.0f;

//Left leg animation
KeyFrameAnimation left_leg_animation = KeyFrameAnimation(num_steps);
//Animation path
char* left_leg_animation_file = (char*) "animaciones/robot/pierna_izquieda.animacion";
//Leg position
float left_leg_rot_z = 0.0f;

int frame=0;
void setFrame();
int main( ){
    //animacion.loadAnimation(path_animation);
    
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
    Shader material_shader("Shaders/pruebas/cel_material_dirlight.vs", "Shaders/pruebas/cel_material_dirlight.frag");
    Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
    
	//Cargando modelos
    Model room = (char*) "Models/proyecto/room/room.obj";
    //Modelo_Material modelo = modelo_path;
    Modelo_Material cuerpo = (char*) "Models/proyecto/robot/body.obj";
    Modelo_Material cabeza = (char*) "Models/proyecto/robot/head.obj";
    Modelo_Material brazo = (char*) "Models/proyecto/robot/right_arm.obj";
    Modelo_Material pierna = (char*) "Models/proyecto/robot/leg.obj";
    
    //Otros modelos
    Model escritorio = (char*) "Models/proyecto/escritorio/escritorio.obj";
    Model silla = (char*) "Models/proyecto/silla/silla.obj";
    Model cama = (char*) "Models/proyecto/cama/cama.obj";
    //Model modelo = modelo_path;

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
        model = glm::translate(model, glm::vec3(4.51f, 0.0f, 4.0f)); // Translate it down a bit so it's at the center of the scene
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        escritorio.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.5f, 0.0f, 4.0f)); // silla
        
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        silla.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.30f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cama.Draw(shader);
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
        //Posicion por defecto
        //model = glm::translate(model, glm::vec3(0.0f, 6.5f, 0.0f)); //Nave
        //model = glm::translate(model, glm::vec3(-5.70f, 0.0f, -1.4f)); //puerta
        //model = glm::translate(model, glm::vec3(2.5f, 0.0f, 4.0f)); // silla

        /* //CONTROL
        model = glm::translate(model, glm::vec3(pos_x,pos_y,pos_z));
        model = glm::rotate(model,glm::radians(rot_x),glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
        */
        /*
        if (animacion.play) {
            animacion.animacion(&model);
        }
        */
        model = glm::translate(model, glm::vec3(3.29f, 2.30f, 0.118f));
        model = glm::translate(model, glm::vec3(body_pos_x, 0.0f, body_pos_z));
        model = glm::rotate(model, glm::radians(body_rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
        if (body_animation.play) {
            body_animation.animacion(&model);
        }
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cuerpo.Draw(material_shader);

        glm::mat4 modelAux = model;
        
        model = modelAux;
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
        model = glm::rotate(model, glm::radians(head_rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
        if (head_animation.play) {
            head_animation.animacion(&model);
        }
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cabeza.Draw(material_shader);

        //Brazo derecho
        model = modelAux;
        model = glm::translate(model, glm::vec3(0.0f, 1.34f, 0.93f));
        model = glm::rotate(model, glm::radians(righ_arm_rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
        if (right_arm_animation.play) {
            right_arm_animation.animacion(&model);
        }
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        brazo.Draw(material_shader);

        //Brazo izquierdo
        model = modelAux;
        model = glm::translate(model, glm::vec3(0.0f, 1.34f, -0.93f));
        model = glm::scale(model, glm::vec3(-1.0f, -1.0f, -1.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(left_arm_rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
        if (left_arm_animation.play) {
            left_arm_animation.animacion(&model);
        }
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        brazo.Draw(material_shader);

        //Piernas
        //Izquieda
        model = modelAux;
        model = glm::translate(model, glm::vec3(0.0f, -0.21f, -0.45f));
        model = glm::rotate(model, glm::radians(left_leg_rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
        if (left_leg_animation.play) {
            left_leg_animation.animacion(&model);
        }
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pierna.Draw(material_shader);

        //Derecha
        model = modelAux;
        model = glm::translate(model, glm::vec3(0.0f, -0.21f, 0.45f));
        model = glm::scale(model, glm::vec3(-1.0f, -1.0f, -1.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(righ_leg_rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
        if (right_leg_animation.play) {
            right_leg_animation.animacion(&model);
        }
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pierna.Draw(material_shader);

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
        //animacion.saveFrame(path_animation,pos_x, pos_y, pos_z, rot_x, rot_y, rot_z);
        head_animation.saveFrame(head_animation_file,0.0f, 0.0f, 0.0f, 0.0f, head_rot_y, 0.0f);
        body_animation.saveFrame(body_animation_file, body_pos_x, 0.0f,body_pos_z, 0.0f, body_rot_y, 0.0f);
        right_arm_animation.saveFrame(right_arm_animation_file, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,righ_arm_rot_z);
        left_arm_animation.saveFrame(left_arm_animation_file, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, left_arm_rot_z);
        right_leg_animation.saveFrame(right_leg_animation_file, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, righ_leg_rot_z);
        left_leg_animation.saveFrame(left_leg_animation_file, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, left_leg_rot_z);
    }
    if (keys[GLFW_KEY_2])
    {
        head_animation.play=true;
        body_animation.play=true;
        right_arm_animation.play=true;
        left_arm_animation.play=true;
        right_leg_animation.play=true;
        left_leg_animation.play=true;
        /*
        animacion.play = !animacion.play;
        if (animacion.play) {
            animacion.reset();
            animacion.interpolation();
            puts("Comienza a correr la animacion");
        }
        */
        head_animation.start();
        body_animation.start();
        right_arm_animation.start();
        left_arm_animation.start();
        right_leg_animation.start();
        left_leg_animation.start();

    }
    float speed_rot=5.0;
    float speed_move = 0.5;
    if (GLFW_KEY_Q == key)
    {
        camera.rotateYaw(-5.0f);
    }
    if (GLFW_KEY_E == key)
    {
        camera.rotateYaw(5.0f);
    }
    //Control del modelo
    //HEAD
    if (keys[GLFW_KEY_U]) {
        head_rot_y += speed_rot;
    }
    if (keys[GLFW_KEY_O]) {
        head_rot_y -= speed_rot;
    }
    //BODY
    if (keys[GLFW_KEY_KP_8]) {
        body_pos_x += speed_move;
    }
    if (keys[GLFW_KEY_KP_5]) {
        body_pos_x -= speed_move;
    }
    if (keys[GLFW_KEY_KP_6]) {
        body_pos_z += speed_move;
    }
    if (keys[GLFW_KEY_KP_4]) {
        body_pos_z -= speed_move;
    }
    if (keys[GLFW_KEY_KP_9]) {
        body_rot_y += speed_rot;
    }
    if (keys[GLFW_KEY_KP_7]) {
        body_rot_y -= speed_rot;
    }

    if (keys[GLFW_KEY_PAGE_UP]) {
        righ_arm_rot_z += speed_rot;
    }
    if (keys[GLFW_KEY_PAGE_DOWN]) {
        righ_arm_rot_z -= speed_rot;
    }
    if (keys[GLFW_KEY_INSERT]) {
        left_arm_rot_z += speed_rot;
    }
    if (keys[GLFW_KEY_DELETE]) {
        left_arm_rot_z -= speed_rot;
    }

    if (keys[GLFW_KEY_Y]) {
        righ_leg_rot_z += speed_rot;
    }
    if (keys[GLFW_KEY_H]) {
        righ_leg_rot_z -= speed_rot;
    }
    if (keys[GLFW_KEY_T]) {
        left_leg_rot_z += speed_rot;
    }
    if (keys[GLFW_KEY_G]) {
        left_leg_rot_z -= speed_rot;
    }
    if (keys[GLFW_KEY_C]) {
        if (body_animation.KeyFrame.size() > 0) {
            frame += 1;
            if (frame > body_animation.KeyFrame.size() - 1) {
                frame = 0;
            }
            setFrame();
        }
        
    }
    if (keys[GLFW_KEY_Z]) {
        if (body_animation.KeyFrame.size() > 0) {
            frame -= 1;
            if (frame < 0) {
                frame = body_animation.KeyFrame.size() - 1;
            }
            setFrame();
        }
        
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
void setFrame() {
    head_rot_y = head_animation.KeyFrame[frame].rotY;
    body_rot_y = body_animation.KeyFrame[frame].rotY;
    body_pos_x = body_animation.KeyFrame[frame].posX;
    body_pos_z = body_animation.KeyFrame[frame].posZ;
    righ_arm_rot_z = right_arm_animation.KeyFrame[frame].rotZ;
    left_arm_rot_z = left_arm_animation.KeyFrame[frame].rotZ;
    righ_leg_rot_z = right_leg_animation.KeyFrame[frame].rotZ;
    left_leg_rot_z = left_leg_animation.KeyFrame[frame].rotZ;
}
