//  main.cpp
//  terrainGenerator
//  Created by User on 17/01/19.
//  Copyright © 2019 mickael. All rights reserved.
//  Estudo sobre renderização de terrenos em OpenGL e c++

//----------------------------------------
//------------- Includes -----------------
//----------------------------------------
#include <iostream>
#include <GLUT/GLUT.h>
#include "brute_force.h"
#include "terrain.h"
#include "quadtree.h"
#include "math_ops.h"
#include "camera.h"
#include "ROAM.h"
#include "water.h"
#include "skybox.h"
#include "skydome.h"

using namespace std;

//-----------------------------------------
//------ Variaveis globais ----------------
//-----------------------------------------
enum FRACTAL_ALGORITHM
{
    FAULT_FORMATION= 0,
    MIDPOINT_DISPLACEMENT
};
FRACTAL_ALGORITHM g_iFractalAlgo;
//forca bruta
CBRUTE_FORCE g_bruteForce;
//quadtree
CQUADTREE g_quadtree;
//ROAM
CCAMERA g_camera;
CROAM g_ROAM;
//Water
CWATER g_water;
//skybox
CSKYBOX g_skybox;
//skydome
CSKYDOME g_skydome;

int g_iCurrentHeightmap;
int g_iLevel= 45;
bool g_bTexture= true;
bool g_bDetail = true;
float g_fDetailLevel  = 50.0f;
float g_fMinResolution= 10.0f;
GLfloat alturaColisao, offset = 10.0f, offset_camera_player = 20.0f;
GLfloat esferaX = 129.0f, esferaY, esferaZ = 519.0f;
GLfloat cameraX = esferaX - 1.0, cameraY , cameraZ = esferaZ - 5.0;

//-----------------------------------------
//------ funções --------------------------
//-----------------------------------------

void desenhaEsfera(GLfloat x, GLfloat y, GLfloat z){
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity( );
    //setup the viewing matrix
    g_camera.ComputeViewMatrix( );
    g_camera.SetViewMatrix( );
    g_camera.CalculateViewFrustum( );
    /*
    //------------FORCA BRUTA---------------------
    g_bruteForce.DoTextureMapping( g_bTexture );
    g_bruteForce.DoDetailMapping( g_bDetail, 8 );
    glPushMatrix( );
    glScalef( 10.0f, 10.0f, 10.0f );
    g_bruteForce.Render( );
    glPopMatrix( );
    */
 
    //-----------QUADTREE--------------------------
    /*g_quadtree.SetDetailLevel( g_fDetailLevel );
    g_quadtree.SetMinResolution( g_fMinResolution );
    
    g_quadtree.DoTextureMapping( g_bTexture );
    g_quadtree.DoDetailMapping( g_bDetail, 16 );
    g_quadtree.Update( &g_camera );
    
    //render the simple terrain!
    glPushMatrix( );
    g_quadtree.Scale( 8.0f, 5.0f, 8.0f );
    g_quadtree.Render( );
    glPopMatrix( );*/
    
    //------------ROAM---------------------
    g_ROAM.DoTextureMapping( g_bTexture );
    g_ROAM.DoDetailMapping( g_bDetail, 16 );
    glDisable( GL_CULL_FACE );
    glDisable( GL_DEPTH_TEST );
    glDepthMask( GL_FALSE );
    //skybox
    // g_skybox.Set( cameraX, cameraY, cameraZ, 1024.0f );
    // g_skybox.Render( );
    //skydome
    g_skydome.Set( g_camera.m_vecEyePos[0], g_camera.m_vecEyePos[1]-400.0f, g_camera.m_vecEyePos[2] );
    g_skydome.Render( 0.009f, true );
    glDepthMask( GL_TRUE );
    glEnable( GL_DEPTH_TEST );
    //render the simple terrain!
    g_ROAM.DoTextureMapping( true );
    glFrontFace( GL_CW );
    glEnable( GL_CULL_FACE );
    g_ROAM.Scale( 5.0f, 5.0f, 5.0f );
    g_ROAM.Update( );
    
    //update the water's vertices and re-calculate polygon normals
    g_water.Update( 0.001f );
    g_water.CalcNormals( );
    
    //render the terrain mesh
    glPushMatrix( );
    g_ROAM.Render( );
    glPopMatrix( );

    //render the water mesh
    glPushMatrix( );
    glTranslatef( 0.0f, 75.0f, 0.0f );
    
    glDepthMask( GL_FALSE );
    g_water.Render( true );
    glDepthMask( GL_TRUE );
    glPopMatrix( );
    
    //render da esfera
    glPushMatrix();
    desenhaEsfera(esferaX, esferaY, esferaZ);
    glPopMatrix();
   
    //print render
    glutSwapBuffers();
}

void init(void){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    // fog
    GLfloat fogColor[] = { 0.7f, 0.7f, 0.7f, 1.0f};
    glFogi (GL_FOG_MODE, GL_LINEAR);
    glFogfv (GL_FOG_COLOR, fogColor);
    glFogf( GL_FOG_DENSITY, 1.0f );
    glFogf( GL_FOG_START, 100.0f );
    glFogf( GL_FOG_END, 1000.0f );
    glHint (GL_FOG_HINT, GL_FASTEST);
    glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);
    glEnable(GL_FOG);
    
    //esconder partes de entidades não visíveis
    glEnable(GL_DEPTH_TEST);

    /*
     //-------Forca bruta---------------------------------------------------------------
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
    g_bruteForce.DoMultitexturing(true);*/
    
    
    //-------QUADTREE----------------------------------------------------------------
    //load the height map in
   /* g_quadtree.MakeTerrainFault( 513, 64, 0, 255, 0.25f );
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
    g_quadtree.Init();*/
    
    
    //initialize the ROAM system
    g_ROAM.MakeTerrainPlasma( 1024, 1.2f );
    g_ROAM.SetHeightScale( 10.0f );
    //set the terrain's lighting system up
    g_ROAM.SetLightingType( SLOPE_LIGHT );
    g_ROAM.SetLightColor( CVECTOR( 1.0f, 1.0f, 1.0f ) );
    g_ROAM.CustomizeSlopeLighting( 1, 1, 0.2f, 0.9f, 7 );
    g_ROAM.CalculateLighting( );
    //load the various terrain tiles
    char file_image1[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/lowestTile.tga" };
    g_ROAM.LoadTile( LOWEST_TILE, file_image1 );
    char file_image2[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/lowTile.tga" };
    g_ROAM.LoadTile( LOW_TILE,    file_image2);
    char file_image3[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/highTile.tga" };
    g_ROAM.LoadTile( HIGH_TILE,  file_image3 );
    char file_image4[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/highestTile.tga" };
    g_ROAM.LoadTile( HIGHEST_TILE,file_image4 );
    //load the terrain's detail map
    char file_image5[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/detailMap.tga" };
    g_ROAM.LoadDetailMap( file_image5 );
    g_ROAM.DoDetailMapping( g_bDetail, 16 );
    //make the texture map, and then save it
    g_ROAM.GenerateTextureMap( 512 );
    g_ROAM.DoTextureMapping( g_bTexture );
    g_ROAM.DoMultitexturing( true );
    //initialize the ROAM system
    g_ROAM.Init( g_iLevel, 65536, &g_camera );
    
     //-------WATER----------------------------------------------------------
    g_water.Init( 1024.0f );
     char file_image6[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/reflection_map.tga" };
    g_water.LoadReflectionMap( file_image6 );
    g_water.SetColor( 1.0f, 1.0f, 1.0f, 0.9f );
    
    //-------SKY-BOX----------------------------------------------------------
    /* char file_image7[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/skybox_front.tga" };
    g_skybox.LoadTexture( SBX_FRONT, file_image7 );
     char file_image8[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/skybox_back.tga" };
    g_skybox.LoadTexture( SBX_BACK,  file_image8 );
     char file_image9[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/skybox_right.tga" };
    g_skybox.LoadTexture( SBX_RIGHT,  file_image9 );
     char file_image10[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/skybox_left.tga" };
    g_skybox.LoadTexture( SBX_LEFT,  file_image10 );
     char file_image11[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/skybox_top.tga" };
    g_skybox.LoadTexture( SBX_TOP,   file_image11 );
     char file_image12[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/skybox_bottom.tga" };
    g_skybox.LoadTexture( SBX_BOTTOM,file_image12 );*/
    
   //-------SKY-DOME-----------------------------------------------------------
    g_skydome.Init( 10.0f, 10.0f, 512.0f );
     char file_image14[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/clouds.tga" };
    g_skydome.LoadTexture( file_image14 );
  

    //-------CAMERA----------------------------------------------------------
    alturaColisao = g_ROAM.GetTrueHeightAtPoint( g_camera.m_vecEyePos[0],
                                                g_camera.m_vecEyePos[2] );
    esferaY = alturaColisao + offset;
    cameraY = esferaY + offset_camera_player ;
    g_camera.SetPosition( 128.0f, cameraY, 514.0f );
    g_camera.m_fYaw  += 175;
    g_camera.m_fPitch-= 40;
}

void reshape(int w, int h){
    if( h==0 )  h= 1;   //Prevent a divide by zero (bad)
    glViewport( 0, 0, (GLfloat) w, (GLfloat) h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 90,(GLfloat) w/(GLfloat) h, 1.0f, 3000.0f );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
}

void Specialkey(int key, int x, int y)
{
    alturaColisao = g_ROAM.GetTrueHeightAtPoint( g_camera.m_vecEyePos[0],
                                                g_camera.m_vecEyePos[2] );
    switch(key)
    {
        case GLUT_KEY_UP:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            cameraY += 5.0;
            break;
        case GLUT_KEY_DOWN:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            if(esferaY >= alturaColisao){
            esferaY -= 20.0;
                if(esferaY < alturaColisao){
                     esferaY = alturaColisao + offset;
                    cameraY = esferaY + offset_camera_player;
                }
                else{
                      cameraY -= 20.0;
                }
            }
            else{
                 esferaY = alturaColisao + offset;
                 cameraY = esferaY + offset_camera_player;
            }
            break;
        case GLUT_KEY_LEFT:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaX += 20.0;
            cameraX += 20.0;
            if(esferaX >= 984.0) esferaX = 984.0;
            if(esferaY >= alturaColisao){
                esferaY -= 20.0;
                if(esferaY < alturaColisao){
                    esferaY = alturaColisao + offset;
                    cameraY = esferaY + offset_camera_player;
                }
                else{
                    cameraY -= 20.0;
                }
            }
            else{
                esferaY = alturaColisao + offset;
                cameraY = esferaY + offset_camera_player;
            }
            break;
        case GLUT_KEY_RIGHT:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaX -= 20.0;
            cameraX -= 20.0;
            if(esferaX <= 60.0) esferaX = 60.0;
            if(esferaY >= alturaColisao){
                esferaY -= 20.0;
                if(esferaY < alturaColisao){
                    esferaY = alturaColisao + offset;
                    cameraY = esferaY + offset_camera_player;
                }
                else{
                    cameraY -= 20.0;
                }
            }
            else{
                 esferaY = alturaColisao + offset;
                cameraY = esferaY + offset_camera_player;
            }
            break;
    }
    if(cameraX >= 984.0) cameraX = 984.0;
    if(cameraX <= 60.0) cameraX = 60.0;
    if(cameraZ >= 924.0) cameraZ = 924.0;
    if(cameraZ <= 0.0) cameraZ = 0.0;
    g_camera.SetPosition( cameraX, cameraY, cameraZ );
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    alturaColisao = g_ROAM.GetTrueHeightAtPoint( g_camera.m_vecEyePos[0],
                                                g_camera.m_vecEyePos[2] );
    switch (key) {
        case 'z':
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaZ += 20.0;
            cameraZ += 20.0;
            if(esferaZ >= 924.0) esferaZ = 924;
            if(esferaY >= alturaColisao){
                esferaY -= 20.0;
                if(esferaY < alturaColisao){
                     esferaY = alturaColisao + offset;
                    cameraY = esferaY + offset_camera_player;
                }
                else{
                    cameraY -= 20.0;
                }
            }
            else{
                esferaY = alturaColisao + offset;
                cameraY = esferaY + offset_camera_player;
            }
            break;
        case 'Z':
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaZ -= 20.0;
            cameraZ -= 20.0;
            if(esferaZ <= 0) esferaZ = 0;
            
            if(esferaY >= alturaColisao){
                esferaY -= 20.0;
                if(esferaY < alturaColisao){
                     esferaY = alturaColisao + offset;
                    cameraY = esferaY + offset_camera_player;
                }
                else{
                    cameraY -= 20.0;
                }
            }
            else{
                esferaY = alturaColisao + offset;
                cameraY = esferaY + offset_camera_player;
            }
            break;
        case 'p':
           g_camera.m_fPitch-= 5;
            break;
        case 'P':
           g_camera.m_fPitch+= 5;
            break;
        default:
            break;
    }
    if(cameraX >= 984.0) cameraX = 984.0;
    if(cameraX <= 60.0) cameraX = 60.0;
    if(cameraZ >= 924.0) cameraZ = 924.0;
    if(cameraZ <= 0.0) cameraZ = 0.0;
    g_camera.SetPosition( cameraX, cameraY, cameraZ );
    glutPostRedisplay();
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

void mouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN){
        switch (button) {
            case GLUT_LEFT_BUTTON:
               g_camera.m_fYaw  += 5;
                break;
            case GLUT_MIDDLE_BUTTON:
                g_camera.m_fYaw  -= 5;
                break;
            default:
                break;
        }
    }
    glutPostRedisplay();
}

void menu(int item){
    if(item == 1){
       /*g_bruteForce.UnloadTexture();
        g_bruteForce.UnloadLightMap();
        g_bruteForce.UnloadHeightMap();
        g_bruteForce.UnloadAllTiles();*/
       /*g_quadtree.UnloadAllTiles( );
        g_quadtree.UnloadTexture( );
        g_quadtree.UnloadHeightMap( );
        g_quadtree.Shutdown( );*/
        g_ROAM.Shutdown( );
        g_ROAM.UnloadAllTiles( );
        g_ROAM.UnloadTexture( );
        g_ROAM.UnloadHeightMap( );
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
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(Specialkey);
    
    //pop up menu para fechar o programa
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

