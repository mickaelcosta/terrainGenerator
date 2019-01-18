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
GLfloat esferaX = 0.0, esferaY = 0.0, esferaZ = 0.0;
GLfloat cameraX = 3.0, cameraY = 5.0 , cameraZ = 5.0;

void desenhaEsfera(GLfloat x, GLfloat y, GLfloat z){
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(1, 10, 10);
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
            esferaY += 0.25;
            cameraY += 0.25;
            gluLookAt(cameraX, cameraY, cameraZ, esferaX, esferaY, esferaZ, 0.0, 1.0, 0.0);
            break;
        case GLUT_KEY_DOWN:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaY -= 0.25;
            cameraY -= 0.25;
            gluLookAt(cameraX, cameraY, cameraZ, esferaX, esferaY, esferaZ, 0.0, 1.0, 0.0);
            break;
        case GLUT_KEY_LEFT:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaX -= 0.25;
            cameraX -= 0.25;
            gluLookAt(cameraX, cameraY, cameraZ, esferaX, esferaY, esferaZ, 0.0, 1.0, 0.0);
            break;
        case GLUT_KEY_RIGHT:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaX += 0.25;
            cameraX += 0.25;
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
            esferaZ += 0.25;
            cameraZ += 0.25;
            gluLookAt(cameraX, cameraY, cameraZ, esferaX, esferaY, esferaZ, 0.0, 1.0, 0.0);
            break;
        case 'Z':
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaZ -= 0.25;
            cameraZ -= 0.25;
           gluLookAt(cameraX, cameraY, cameraZ, esferaX, esferaY, esferaZ, 0.0, 1.0, 0.0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}


