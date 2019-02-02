//  main.cpp
//  terrainGenerator
//  Created by User on 17/01/19.
//  Copyright © 2019 mickael. All rights reserved.
//  main.cpp
//  Created by User on 26/11/18.
//  Copyright © 2018 mickael. All rights reserved.


//----------------------------------------
//------------- Includes -----------------
//----------------------------------------
#include <iostream>
#include <GLUT/GLUT.h>
#include "light.hpp"
#include "keyboard.hpp"
#include "brute_force.h"
#include "terrain.h"
#include "quadtree.h"

using namespace std;

//-----------------------------------------
//------ Variaveis globais ----------------
//-----------------------------------------
enum FRACTAL_ALGORITHM
{
    FAULT_FORMATION= 0,
    MIDPOINT_DISPLACEMENT
};

CBRUTE_FORCE g_bruteForce;

CQUADTREE g_quadtree;

FRACTAL_ALGORITHM g_iFractalAlgo;
int g_iCurrentHeightmap= 0;

bool g_bTexture= true;
bool g_bDetail = true;
float g_fDetailLevel  = 50.0f;
float g_fMinResolution= 10.0f;


//-----------------------------------------
//------ funções --------------------------
//-----------------------------------------

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //agua
    glBegin(GL_QUADS);
    glColor3f(0.0 , 0.0, 1.0);
    glVertex3d(0, 90, 0);
    glVertex3d(0, 90, 4104);
    glVertex3d(4104, 90, 4104);
    glVertex3d(4104, 90, 0);
    glEnd();
    
    /*
    //render the simple terrain!
    g_bruteForce.DoTextureMapping( g_bTexture );
    g_bruteForce.DoDetailMapping( g_bDetail, 8 );
    glPushMatrix( );
    glScalef( 10.0f, 10.0f, 10.0f );
    g_bruteForce.Render( );
    glPopMatrix( );
    */
 
    //update the terrain
    g_quadtree.SetDetailLevel( g_fDetailLevel );
    g_quadtree.SetMinResolution( g_fMinResolution );
    
    g_quadtree.DoTextureMapping( g_bTexture );
    g_quadtree.DoDetailMapping( g_bDetail, 16 );
    g_quadtree.Update( );
    
    //render the simple terrain!
    glPushMatrix( );
    g_quadtree.Scale( 8.0f, 5.0f, 8.0f );
    g_quadtree.Render( );
    glPopMatrix( );
    
    //render da esfera
    desenhaEsfera(esferaX, esferaY, esferaZ);
    
    //render
    glutSwapBuffers();
}

void init(void){
    //plano de fundo
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    
    //light
    //lightInit();
    
    // fog
   /* GLfloat fogColor[] = { 0.1, 0.1, 0.1, 0.5};
    glEnable(GL_FOG);
    {
    glFogi (GL_FOG_MODE, GL_LINEAR);
    glFogfv (GL_FOG_COLOR, fogColor);
    glFogf (GL_FOG_DENSITY, 0.35);
    glFogf(GL_FOG_START, 2000);
    glFogf(GL_FOG_END, 2010);
    glHint (GL_FOG_HINT, GL_FASTEST);
    glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);
}*/
    //esconder partes de entidades não visíveis
    glEnable(GL_DEPTH_TEST);
    
    /*
    
    g_bruteForce.MakeTerrainPlasma(256, 1.2f );
    g_bruteForce.SetHeightScale( 0.25f );
    
    //set the terrain's lighting system up
    g_bruteForce.SetLightingType( SLOPE_LIGHT );
    g_bruteForce.SetLightColor( CVECTOR( 1.0f, 1.0f, 1.0f ) );
    g_bruteForce.CustomizeSlopeLighting( 1, 1, 0.2f, 0.9f, 15 );
    g_bruteForce.CalculateLighting( );
    
    //load the various terrain tiles
   char file_image1[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/lowestTile.tga"};
    g_bruteForce.LoadTile( LOWEST_TILE, file_image1 );
    char file_image2[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/lowTile.tga"};
    g_bruteForce.LoadTile( LOW_TILE, file_image2 );
    char file_image3[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/HighTile.tga"};
    g_bruteForce.LoadTile( HIGH_TILE, file_image3 );
    char file_image4[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/highestTile.tga" };
    g_bruteForce.LoadTile( HIGHEST_TILE,file_image4);
    
    //load the terrain's detail map
    char file_image5[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/detailMap.tga" };
    g_bruteForce.LoadDetailMap(file_image5);
    g_bruteForce.DoDetailMapping( g_bDetail, 8 );
    
    //make the texture map, and then save it
    g_bruteForce.GenerateTextureMap( 256 );
    g_bruteForce.DoTextureMapping( g_bTexture );
    g_bruteForce.DoMultitexturing(true);
    
    */
    
    //load the height map in
    g_quadtree.MakeTerrainFault( 513, 64, 0, 255, 0.25f );
    g_quadtree.SetHeightScale( 1.5f );
    
    //set the terrain's lighting system up
    g_quadtree.SetLightingType( SLOPE_LIGHT );
    g_quadtree.SetLightColor( CVECTOR( 1.0f, 1.0f, 1.0f ) );
    g_quadtree.CustomizeSlopeLighting( 1, 1, 0.1f, 0.9f, 5 );
    g_quadtree.CalculateLighting( );
    
    //load the various terrain tiles
    char file_image1[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/lowestTile.tga" };
    g_quadtree.LoadTile( LOWEST_TILE,  file_image1 );
     char file_image2[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/lowTile.tga" };
    g_quadtree.LoadTile( LOW_TILE,  file_image2 );
     char file_image3[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/highTile.tga" };
    g_quadtree.LoadTile( HIGH_TILE,  file_image3);
     char file_image4[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/highestTile.tga" };
    g_quadtree.LoadTile( HIGHEST_TILE, file_image4 );
    
    //load the terrain's detail map
     char file_image5[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/detailMap.tga" };
    g_quadtree.LoadDetailMap( file_image5 );
    g_quadtree.DoDetailMapping( g_bDetail, 16 );
    
    //make the texture map, and then save it
    g_quadtree.GenerateTextureMap( 256 );
    g_quadtree.DoTextureMapping( g_bTexture );
    g_quadtree.DoMultitexturing( true );
    
    //initiate the geomipmapping system
    g_quadtree.SetDetailLevel( g_fDetailLevel );
    g_quadtree.SetMinResolution( g_fMinResolution );
    
    g_quadtree.Init( );
    
}

void reshape(int w, int h){
    if( h==0 )    //Prevent a divide by zero (bad)
        h= 1;    //Making height equal one
    
    glViewport( 0, 0, (GLfloat) w, (GLfloat) h );   //Reset the current viewport
    glMatrixMode( GL_PROJECTION );   //Select the projection matrix
    glLoadIdentity( );     //Reset the projection matrix
    //Calculate the aspect ratio of the window
    gluPerspective( 90,(GLfloat) w/(GLfloat) h, 1.0f, 3000.0f );
    glMatrixMode( GL_MODELVIEW );  //Select the modelview matrix
    glLoadIdentity( );
    //posicionando a camera
    gluLookAt(cameraX, cameraY, cameraZ, esferaX, esferaY, esferaZ, 0.0, 1.0, 0.0);
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
    if(item == 1){
//g_bruteForce.UnloadTexture();
       // g_bruteForce.UnloadLightMap();
      //  g_bruteForce.UnloadHeightMap();
     //   g_bruteForce.UnloadAllTiles();
        g_quadtree.UnloadAllTiles( );
        g_quadtree.UnloadTexture( );
        g_quadtree.UnloadHeightMap( );
        g_quadtree.Shutdown( );
        exit(0);
    }
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

