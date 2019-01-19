//
//  main.cpp
//  terrainGenerator
//
//  Created by User on 17/01/19.
//  Copyright © 2019 mickael. All rights reserved.
//

//
//  main.cpp
//  aprendizado
//
//  Created by User on 26/11/18.
//  Copyright © 2018 mickael. All rights reserved.

#include <iostream>
#include <GLUT/GLUT.h>
#include "light.hpp"
#include "keyboard.hpp"

using namespace std;


void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    desenhaEsfera(esferaX, esferaY, esferaZ);
    glutSwapBuffers();
}

void init(void){
    GLfloat fogColor[] = { 1.0, 1.0, 1.0, 1 };
    
    //plano de fundo
    glClearColor(1, 1, 1, 0.0);
    glShadeModel(GL_SMOOTH);
    
    //light
    lightInit();
    
    // fog
    glEnable(GL_FOG);
    glFogi (GL_FOG_MODE, GL_LINEAR);
    glFogfv (GL_FOG_COLOR, fogColor);
    glFogf (GL_FOG_DENSITY, 0.35);
    glFogf(GL_FOG_START, 10.0);
    glFogf(GL_FOG_END, 50.0);
    glHint (GL_FOG_HINT, GL_DONT_CARE);
    glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);
    
    //esconder partes de entidades não visíveis
    glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h){
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (GLfloat) w/(GLfloat) h, 1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void selectColor(int item){
    switch (item) {
        case 1:
            glClearColor(0.0, 1.0, 0.0, 0.0);
            break;
        case 2:
            glClearColor(1.0, 0.0, 0.0, 0.0);
            break;
        case 3:
            glClearColor(0.0, 0.0, 1.0, 0.0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void menu(int item){
    if(item == 1)
        exit(0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Terrain Renderer");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(Specialkey);
    
    //pop up menu experimental
    int submenu2;
    submenu2 = glutCreateMenu(selectColor);
    glutAddMenuEntry("Green", 1);
    glutAddMenuEntry("Red", 2);
    glutAddMenuEntry("Blue", 3);
    glutCreateMenu(menu);
    glutAddMenuEntry("Quit Program", 1);
    glutAddSubMenu("Color", submenu2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    init();
    glutMainLoop();
    

    return 0;
}
