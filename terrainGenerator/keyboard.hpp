//
//  keyboard.hpp
//  aprendizado
//
//  Created by User on 15/01/19.
//  Copyright © 2019 mickael. All rights reserved.
//

#ifndef keyboard_hpp
#define keyboard_hpp

#include <stdio.h>
#include <GLUT/GLUT.h>

//variáveis
extern GLfloat cameraX, cameraY, cameraZ;
extern GLdouble spin;
extern GLfloat esferaX, esferaY, esferaZ;

//interface das funções
void desenhaEsfera(GLfloat x, GLfloat y, GLfloat z);
void Specialkey(int key, int x, int y);
GLdouble atualizaPosicaoLuz(void);
void keyboard(unsigned char key, int x, int y);

#endif /* keyboard_hpp */
