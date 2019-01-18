//
//  grid.cpp
//  aprendizado
//
//  Created by User on 15/01/19.
//  Copyright © 2019 mickael. All rights reserved.
//

#include <GLUT/GLUT.h>
#include "grid.hpp"


//variaveis
static GLfloat mat_specular_grid[] = { 0.2, 0.2, 0.2, 1};
static GLfloat alturaGrid = -0.02;

//implementação das funções
void desenhaGrid(int HALF_GRID_SIZE){
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_grid);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular_grid);
    glBegin(GL_LINES);
    glColor3f(0.75f, 0.75f, 0.75f);
    for(int i=-HALF_GRID_SIZE;i<=HALF_GRID_SIZE;i++)
    {
        glVertex3f((float)i,alturaGrid,(float)-HALF_GRID_SIZE);
        glVertex3f((float)i,alturaGrid,(float)HALF_GRID_SIZE);
        glVertex3f((float)-HALF_GRID_SIZE,alturaGrid,(float)i);
        glVertex3f((float)HALF_GRID_SIZE,alturaGrid,(float)i);
    }
    glEnd();
}
