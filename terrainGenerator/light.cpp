//
//  light.cpp
//  aprendizado
//
//  Created by User on 15/01/19.
//  Copyright © 2019 mickael. All rights reserved.
//

#include "light.hpp"

GLfloat light_position[] = { 1.0, 30.0, 1.0, 1.0 };
GLfloat white_light[] = { 0.9, 0.9, 0.9, 1 };
GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };

void lightInit(void){
//iniciando iluminação
glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
}
