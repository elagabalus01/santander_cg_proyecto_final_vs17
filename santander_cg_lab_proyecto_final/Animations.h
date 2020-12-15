#pragma once
// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define MAX_FRAMES 4
class AnimationCircuit {
private:
    GLfloat traslacion_x = 0.0f, traslacion_z = 0.0f, rotation = 0.0f, velocidad = 0.02f;
    GLint state = 1;
    
public:
    AnimationCircuit() {

        reset();
    }
    void reset() {
        traslacion_x = 0.0f, traslacion_z = 0.0f, rotation = 0.0f, state=1;
    }
    void getState() {
        printf("Estado %i\n",state);
    }
    void animacionCircuito(glm::mat4 *model) {
        puts("Ejecutando la animacion");
        switch (state)
        {
        case 1:
            rotation = 0;
            if (traslacion_x > -5.0f)
                traslacion_x -= velocidad;
            else
                state = 2;
            break;
        case 2:
            rotation = 90;
            if (traslacion_z < 5.0f)
                traslacion_z += velocidad;
            else
                state = 3;
            break;
        case 3:
            rotation = 180;
            if (traslacion_x < 5.0f)
                traslacion_x += velocidad;
            else
                state = 4;
            break;
        case 4:
            rotation = 270;
            if (traslacion_z > -5.0f)
                traslacion_z -= velocidad;
            else
                state = 1;
            break;
        default:
            puts("Nunca debe ejecutarse esta funcion");
            //state = 1;
            break;
        }
        printf("Estado %i vec3(%.2f,0,%.2f) rot(%.2f)\n",state,traslacion_x,traslacion_z,rotation);
        *model = glm::translate(*model, glm::vec3(this->traslacion_x, 0, this->traslacion_z));
        *model = glm::rotate(*model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    }
};

typedef struct _frame
{
    //Variables para GUARDAR Key Frames
    float posX;		//Variable para PosicionX
    float posY;		//Variable para PosicionY
    float posZ;		//Variable para PosicionZ
    float incX;		//Variable para IncrementoX
    float incY;		//Variable para IncrementoY
    float incZ;		//Variable para IncrementoZ
    float rotIzq;
    float rotIzqInc;

}FRAME;

class KeyFrameAnimation {
private:
    GLfloat posX, posY, posZ, incX, incY, incZ, rotIzq, rotIzqInc;
    int i_max_steps = 190;
    int i_curr_steps = 0;
    //int num_freames;
    int playIndex = 0;
public:
    FRAME KeyFrame[MAX_FRAMES];
    GLint FrameIndex = 0;
    bool play = false;
    KeyFrameAnimation() {
        //Initializing the animation
        for (int i = 0; i < MAX_FRAMES; i++)
        {
            KeyFrame[i].posX = 0;
            KeyFrame[i].incX = 0;
            KeyFrame[i].incY = 0;
            KeyFrame[i].incZ = 0;
            KeyFrame[i].rotIzq = 0;
            KeyFrame[i].rotIzqInc = 0;
        }
    }
    /*
    void saveFrame(void)
    {

        printf("frameindex %d\n", FrameIndex);

        KeyFrame[FrameIndex].posX = posX;
        KeyFrame[FrameIndex].posY = posY;
        KeyFrame[FrameIndex].posZ = posZ;

        KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;


        FrameIndex++;
    }
    */
    void resetElements(void)
    {
        posX = KeyFrame[0].posX;
        posY = KeyFrame[0].posY;
        posZ = KeyFrame[0].posZ;

        rotIzq = KeyFrame[0].rotIzq;

    }

    void interpolation(void){

        KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
        KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
        KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

        KeyFrame[playIndex].rotIzqInc = (KeyFrame[playIndex + 1].rotIzq - KeyFrame[playIndex].rotIzq) / i_max_steps;
    }
    void reset() {
        playIndex = 0;
        i_curr_steps = 0;
        resetElements();
    }
    void animacion(glm::mat4 *model){
        //Movimiento del personaje
        if (play) {
            cout <<"Index: "<< playIndex << "Frame: " << i_curr_steps << endl;
            if (i_curr_steps >= i_max_steps){
                playIndex++;
                if (playIndex > FrameIndex-1)	//end of total animation?
                {
                    printf("termina anim\n");
                    playIndex = 0;
                    play = false;
                }
                else //Next frame interpolations
                {
                    i_curr_steps = 0; //Reset counter
                                        //Interpolation
                    interpolation();
                }

            }
            else {
                //Draw animation
                posX += KeyFrame[playIndex].incX;
                posY += KeyFrame[playIndex].incY;
                posZ += KeyFrame[playIndex].incZ;

                rotIzq += KeyFrame[playIndex].rotIzqInc;

                cout << "x "<< posX << " y " << posY << " Z " << posZ << endl;
                i_curr_steps++;
            }
            *model = glm::translate(*model, glm::vec3(posX, posY, posZ));
            *model = glm::rotate(*model, glm::radians(rotIzq), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        
    }
};
KeyFrameAnimation animacion_avanzada_nave = KeyFrameAnimation();
void setAnimation() {
    animacion_avanzada_nave.KeyFrame[0].posX = 0.0f;
    animacion_avanzada_nave.KeyFrame[0].posY = 0.0f;
    animacion_avanzada_nave.KeyFrame[0].posZ = 0.0f;
    animacion_avanzada_nave.KeyFrame[0].rotIzq = 0.0f;
    
    animacion_avanzada_nave.KeyFrame[1].posX = -5.0f;
    animacion_avanzada_nave.KeyFrame[1].posY = 0.0f;
    animacion_avanzada_nave.KeyFrame[1].posZ = 0.0f;
    animacion_avanzada_nave.KeyFrame[1].rotIzq = 0.0f;
    
    animacion_avanzada_nave.KeyFrame[2].posX = -5.0f;
    animacion_avanzada_nave.KeyFrame[2].posY = 0.0f;
    animacion_avanzada_nave.KeyFrame[2].posZ = 5.0f;
    animacion_avanzada_nave.KeyFrame[2].rotIzq = 90.0f;

    animacion_avanzada_nave.KeyFrame[3].posX = 5.0f;
    animacion_avanzada_nave.KeyFrame[3].posY = 0.0f;
    animacion_avanzada_nave.KeyFrame[3].posZ = 0.0f;
    animacion_avanzada_nave.KeyFrame[3].rotIzq = 180.0f;
    
    animacion_avanzada_nave.KeyFrame[4].posX = 0.0f;
    animacion_avanzada_nave.KeyFrame[4].posY = 0.0f;
    animacion_avanzada_nave.KeyFrame[4].posZ = 0.0f;
    animacion_avanzada_nave.KeyFrame[4].rotIzq = 270.0f;

    //animacion_avanzada_nave.FrameIndex = 4;
}
