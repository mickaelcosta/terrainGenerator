//
//  keyboard.cpp
//  aprendizado
//
//  Created by User on 15/01/19.
//  Copyright © 2019 mickael. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <iostream>
#include "keyboard.hpp"

using namespace std;

//definição das variáveis
GLfloat esferaX = 100.0, esferaY = 85.0, esferaZ = 125.0;
GLfloat cameraX = esferaX + 3.0, cameraY = esferaY + 5.0 , cameraZ = esferaZ + 5.0;

void desenhaEsfera(GLfloat x, GLfloat y, GLfloat z){
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(1, 10, 10);
    cout << "Posição esfera " << esferaX <<" " << esferaY << " " << esferaZ <<endl;
    glPopMatrix();
}

//implementação das funções
void Specialkey(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaY += 5.0;
            cameraY += 5.0;
            gluLookAt(cameraX, cameraY, cameraZ, esferaX, esferaY, esferaZ, 0.0, 1.0, 0.0);
            break;
        case GLUT_KEY_DOWN:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaY -= 5.0;
            cameraY -= 5.0;
            gluLookAt(cameraX, cameraY, cameraZ, esferaX, esferaY, esferaZ, 0.0, 1.0, 0.0);
            break;
        case GLUT_KEY_LEFT:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaX -= 5.0;
            cameraX -= 5.0;
            gluLookAt(cameraX, cameraY, cameraZ, esferaX, esferaY, esferaZ, 0.0, 1.0, 0.0);
            break;
        case GLUT_KEY_RIGHT:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaX += 5.0;
            cameraX += 5.0;
            gluLookAt(cameraX, cameraY, cameraZ, esferaX, esferaY, esferaZ, 0.0, 1.0, 0.0);
            break;
    }
    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'z':
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaZ += 5.0;
            cameraZ += 5.0;
            gluLookAt(cameraX, cameraY, cameraZ, esferaX, esferaY, esferaZ, 0.0, 1.0, 0.0);
            break;
        case 'Z':
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaZ -= 5.0;
            cameraZ -= 5.0;
           gluLookAt(cameraX, cameraY, cameraZ, esferaX, esferaY, esferaZ, 0.0, 1.0, 0.0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}


