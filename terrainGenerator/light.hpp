//
//  light.hpp
//  aprendizado
//
//  Created by User on 15/01/19.
//  Copyright © 2019 mickael. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#include <stdio.h>
#include <GLUT/GLUT.h>

extern GLfloat light_position[];
extern GLfloat white_light[];
extern GLfloat lmodel_ambient[];

void lightInit(void);

#endif /* light_hpp */
