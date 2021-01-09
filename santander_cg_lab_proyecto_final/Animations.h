#pragma once
// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

vector<string> splitString(string cadena, char delimiter) {
    vector <string> tokens;
    stringstream stream_cadena(cadena);
    string aux;

    while (getline(stream_cadena, aux, delimiter))
    {
        tokens.push_back(aux);
    }
    return tokens;
}

class Animacion {
public:
    GLfloat traslacion_x = 0.0f, traslacion_z = 0.0f, rotation = 0.0f, velocidad = 0.1f;
    GLint state = 1;
    GLboolean play = false;

    Animacion() {
        reset();
    }
    
    void toggle() {
        play = !play;
    }
    void reset() {
        traslacion_x = 0.0f, traslacion_z = 0.0f, rotation = 0.0f, state = 1;
    }
    void getState() {
        printf("Estado %i\n", state);
    }
    virtual void run(glm::mat4 *model) {};

};

class AnimationCircuit: public Animacion {
public:
    AnimationCircuit() {
        reset();
    }
    void run(glm::mat4 *model) {
        if (play) {
            switch (state){
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
                    break;
            }
            //printf("Nave\nEstado %i vec3(%.2f,0,%.2f) rot(%.2f)\n", state, traslacion_x, traslacion_z, rotation);
        }
        
        *model = glm::translate(*model, glm::vec3(this->traslacion_x, 0, this->traslacion_z));
        *model = glm::rotate(*model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    }
};

//ANIMACION DE LA SILLA
class AnimationSilla: public Animacion{
public:
    AnimationSilla() {
        reset();
    }
    void run(glm::mat4 *model) {
        if (play) {
            switch (state){
                case 1:
                    if (rotation <= 55.0f)
                        rotation += 10 * velocidad;
                    else
                        state = 2;
                    break;
                case 2:
                    if (traslacion_z <= 0.8f || rotation <= 60) {
                        if (traslacion_z <= 0.8f)
                            traslacion_z += velocidad;
                        if (rotation <= 60)
                            rotation += 10 * velocidad;
                    }
                    else
                        state = 3;
                    break;
                case 3:
                    if (traslacion_z > 0.0f || rotation > 0.0f) {
                        if (traslacion_z > 0.0f)
                            traslacion_z -= velocidad;
                        if (rotation > 0.0f)
                            rotation -= 10 * velocidad;
                    }
                    else{
                        state = 1;
                        reset();
                        play = false;
                    }
                    break;
                default:
                    puts("Nunca debe ejecutarse esta funcion");
                    break;
            }
        
            //printf("Silla\nEstado %i vec3(%.2f,0,%.2f) rot(%.2f)\n", state, traslacion_x, traslacion_z, rotation);
            *model = glm::translate(*model, glm::vec3(this->traslacion_x, 0, this->traslacion_z));
            *model = glm::rotate(*model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }
};
//ANIMACION DE LA PUERTA
class AnimationPuerta: public Animacion {
public:
    AnimationPuerta() {
        reset();
    }
    void run(glm::mat4 *model) {
        if (play) {
            switch (state) {
                case 1:
                    if (rotation < 90)
                        rotation += 10 * velocidad;
                    else
                        state = 2;
                    break;
                case 2:
                    if (rotation > 0)
                        rotation -= 10 * velocidad;
                    else{
                        state = 1;
                        reset();
                        play = false;
                    }
                    break;
                default:
                    break;
            }
            //printf("Puerta\nEstado %i vec3(%.2f,0,%.2f) rot(%.2f)\n", state, traslacion_x, traslacion_z, rotation);
            *model = glm::rotate(*model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        }
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
    float rotX;
    float rotXInc;
    float rotY;
    float rotYInc;
    float rotZ;
    float rotZInc;
    _frame(float _posX, float _posY, float _posZ, float _rotX,float _rotY,float _rotZ) {
        posX = _posX;
        posY = _posY;
        posZ = _posZ;
        rotX = _rotX;
        rotY = _rotY;
        rotZ = _rotZ;
    }

}FRAME;

class KeyFrameAnimation {
private:
    int i_max_steps;
    int i_curr_steps = 0;
    int playIndex = 0;
public:
    float posX, posY, posZ, incX, incY, incZ, rotX, rotXInc, rotY, rotYInc, rotZ, rotZInc;
    vector<FRAME> KeyFrame = vector<FRAME>();
    GLint FrameIndex = 0;
    bool play = false;
    KeyFrameAnimation(int _i_max_steps = 190) {
        //Initializing the animation
        i_max_steps = _i_max_steps;
        for (int i = 0; i < KeyFrame.size(); i++)
        {
            KeyFrame[i].posX = 0;
            KeyFrame[i].incX = 0;
            KeyFrame[i].incY = 0;
            KeyFrame[i].incZ = 0;
            KeyFrame[i].rotX = 0;
            KeyFrame[i].rotXInc = 0;
            KeyFrame[i].rotY = 0;
            KeyFrame[i].rotYInc = 0;
            KeyFrame[i].rotZ = 0;
            KeyFrame[i].rotZInc = 0;
        }
    }
    void start() {
        if (play) {
            reset();
            interpolation();
            puts("Comienza a correr la animacion");
        }
    }
    void loadAnimation(char* path) {
        string line;
        vector<string> row;
        ifstream file(path);
        while (getline(file, line)) {
            row = splitString(line, ',');
            KeyFrame.push_back(
                FRAME(
                    stof(row[0]), stof(row[1]), stof(row[2]), stof(row[3]), stof(row[4]), stof(row[5])
                )
            );
            //printf("Frame tras(%.2f,%.2f,%.2f) rot(%.2f)\n", stof(row[0]), stof(row[1]), stof(row[2]), stof(row[3]));
        }
        file.close();
        //cout << "Numero de frames " << KeyFrame.size() << endl;
    }
    void saveFrame(char *path,float pos_x,float pos_y,float pos_z,float rot_x, float rot_y, float rot_z)
    {
        ofstream file;
        KeyFrame.push_back(FRAME(pos_x,pos_y,pos_z,rot_x, rot_y, rot_z));
        file.open(path, ios_base::app);
        file<<pos_x<<","<<pos_y<<","<<pos_z<<","<<rot_x<< ","<< rot_y << ","<<rot_z << endl;
        file.close();
    }
    void resetElements(void)
    {
        posX = KeyFrame[0].posX;
        posY = KeyFrame[0].posY;
        posZ = KeyFrame[0].posZ;

        rotX = KeyFrame[0].rotX;
        rotY = KeyFrame[0].rotY;
        rotZ = KeyFrame[0].rotZ;

    }

    void interpolation(void){

        KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
        KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
        KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

        KeyFrame[playIndex].rotXInc = (KeyFrame[playIndex + 1].rotX - KeyFrame[playIndex].rotX) / i_max_steps;
        KeyFrame[playIndex].rotYInc = (KeyFrame[playIndex + 1].rotY - KeyFrame[playIndex].rotY) / i_max_steps;
        KeyFrame[playIndex].rotZInc = (KeyFrame[playIndex + 1].rotZ - KeyFrame[playIndex].rotZ) / i_max_steps;
    }
    void reset() {
        playIndex = 0;
        i_curr_steps = 0;
        resetElements();
    }
    void animacion(glm::mat4 *model){
        //Movimiento del personaje
        if (play) {
            //cout <<"Index: "<< playIndex << "Frame: " << i_curr_steps << endl;
            if (i_curr_steps >= i_max_steps){
                playIndex++;
                if (playIndex > KeyFrame.size()-2)	//end of total animation?
                {
                    //printf("termina anim\n");
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

                rotX += KeyFrame[playIndex].rotXInc;
                rotY += KeyFrame[playIndex].rotYInc;
                rotZ += KeyFrame[playIndex].rotZInc;

                i_curr_steps++;
            }
            *model = glm::translate(*model, glm::vec3(posX, posY, posZ));
            *model = glm::rotate(*model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
            *model = glm::rotate(*model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
            *model = glm::rotate(*model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        
    }
};