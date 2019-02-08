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

//forca bruta
CBRUTE_FORCE g_bruteForce;
//camera
CCAMERA g_camera;
//Water
CWATER g_water;
//skydome
CSKYDOME g_skydome;

int g_iCurrentHeightmap = 0;
int g_iLevel= 45;
int map_size = 128;
bool g_bTexture= true;
bool g_bDetail = true;
float g_fDetailLevel  = 50.0f;
float g_fMinResolution= 10.0f;
GLfloat alturaColisao, offset = 2.0f, offset_camera_player = 10.0f, scale_map = 4.0f;
GLfloat esferaX = 65.0f, esferaY, esferaZ = 95.0f;
GLfloat cameraX = esferaX, cameraY , cameraZ = esferaZ + 10.0;
int g_iScreenHeight , g_iScreenWidth;

//-----------------------------------------
//------ funções --------------------------
//-----------------------------------------

void desenhaEsfera(GLfloat x, GLfloat y, GLfloat z){
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();
    cout << "esfera: x "<<esferaX <<" y " << esferaY <<" z " << esferaZ <<endl;
    cout << "camera: x "<<cameraX <<" y " << cameraY <<" z " << cameraZ <<endl;
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity( );
    //go back to perspective mode
    glMatrixMode(GL_PROJECTION);       //select the projection matrix
    glPopMatrix();                     //restore the old projection matrix
    glMatrixMode(GL_MODELVIEW);        //select the modelview matrix
    
    //setup the viewing matrix
    g_camera.ComputeViewMatrix( );
    g_camera.SetViewMatrix( );
    g_camera.CalculateViewFrustum( );
    
    //------------RENDER SKYDOME---------------------
    glDisable( GL_CULL_FACE );
    glDisable( GL_DEPTH_TEST );
    glDepthMask( GL_FALSE );
    g_skydome.Set( g_camera.m_vecEyePos[0], g_camera.m_vecEyePos[1]-400.0f, g_camera.m_vecEyePos[2] );
    g_skydome.Render( 0.009f, true );
    glDepthMask( GL_TRUE );
    glEnable( GL_DEPTH_TEST );
    
    //------------TERRENO POR FORCA BRUTA---------------------
    glFrontFace( GL_CCW );
    g_bruteForce.DoTextureMapping( g_bTexture );
    g_bruteForce.DoDetailMapping( g_bDetail, 8 );
    glPushMatrix( );
    glScalef( scale_map, 1.0f, scale_map );
    g_bruteForce.Render( );
    glPopMatrix( );
    
    //------------RENDER WATER---------------------
    g_water.Update( 0.001f );
    g_water.CalcNormals( );
    glFrontFace( GL_CW );
    glEnable( GL_CULL_FACE );
    glPushMatrix( );
    glTranslatef( -250.0f, 10.0f, -250.0f );
    glDepthMask( GL_FALSE );
    g_water.Render( true );
    glDepthMask( GL_TRUE );
    glPopMatrix( );
    glDisable(GL_CULL_FACE);
    
    //------------ ESFERA ---------------------
    glPushMatrix();
    desenhaEsfera(esferaX, esferaY, esferaZ);
    glPopMatrix();
    
    //------------ RENDER MINI MAPA ---------------------
    glLoadIdentity();
    glMatrixMode( GL_PROJECTION );  //select the projection matrix
    glPushMatrix( );                //store the projection matrix
    glLoadIdentity( );
    glOrtho( 0, g_iScreenWidth, 0, g_iScreenHeight, -1, 1 );//set up an ortho screen
    glMatrixMode( GL_MODELVIEW );        //select the modelview matrix
    
    //render the height map "minimap"
    glBegin( GL_POINTS );
    for( auto z=0; z<g_bruteForce.m_iSize; z++ )
    {
        for(auto x=0; x<g_bruteForce.m_iSize; x++ )
        {
            glColor3ub( g_bruteForce.GetTrueHeightAtPoint( x, z ),
                       g_bruteForce.GetTrueHeightAtPoint( x, z ),
                       g_bruteForce.GetTrueHeightAtPoint( x, z ) );
            glVertex2d( x, g_iScreenHeight-z );
            //  if(esferaX == x && esferaZ == z){
            //  glColor3f(1.0, 0.0, 0.0);
            //glVertex2d(x, z);
            // }
        }
    }
    glEnd();

    glutSwapBuffers();
}

void init(void){
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glDisable( GL_TEXTURE_2D );       //disable two dimensional texture mapping
    glDisable( GL_LIGHTING );         //disable lighting
    glDisable( GL_BLEND );            //disable blending
    glEnable( GL_DEPTH_TEST );        //enable depth testing
    
    glShadeModel( GL_SMOOTH );        //enable smooth shading
    glClearDepth( 1.0 );              //depth buffer setup
    glDepthFunc( GL_LEQUAL );          //set the type of depth test
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );    //the nicest perspective look

     //-------   Fog  -------------------------
    GLfloat fogColor[] = { 0.8f, 0.8f, 0.8f, 0.7f};
    glFogi (GL_FOG_MODE, GL_LINEAR);
    glFogfv (GL_FOG_COLOR, fogColor);
    glFogf( GL_FOG_DENSITY, 0.2f );
    glFogf( GL_FOG_START, 100.0f );
    glFogf( GL_FOG_END, 350.0f );
    glHint (GL_FOG_HINT, GL_NICEST);
    glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);
    glEnable(GL_FOG);
    
    //esconder partes de entidades não visíveis
    glEnable(GL_DEPTH_TEST);

    
     //-------Forca bruta-------------------------
    char file_image100[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/height128.raw"};
    g_bruteForce.LoadHeightMap(file_image100, map_size);
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
    
    //-------WATER----------------------------------------------------------
    g_water.Init( 1024.0f );
     char file_image6[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/reflection_map.tga" };
    g_water.LoadReflectionMap( file_image6 );
    g_water.SetColor( 1.0f, 1.0f, 1.0f, 0.9f );
    
    //-------SKY-DOME-----------------------------------------------------------
    g_skydome.Init( 10.0f, 10.0f, 512.0f );
    char file_image14[] = {"/Volumes/HD Mac/Workspaces/C++/xcode/terrainGenerator/terrainGenerator/data/clouds.tga" };
    g_skydome.LoadTexture( file_image14 );
    
    //-------CAMERA----------------------------------------------------------
    alturaColisao = g_bruteForce.GetScaledHeightAtPoint( esferaX/scale_map,
                                                esferaZ/scale_map );
    esferaY = alturaColisao + offset;
    cameraY = esferaY + offset_camera_player ;
    //set the camera's position
    g_camera.SetPosition( cameraX, cameraY, cameraZ );
    g_camera.m_fPitch -= 45;
}

void reshape(int w, int h){
    if( h==0 )  h= 1;   //Prevent a divide by zero (bad)
    g_iScreenWidth = w;
    g_iScreenHeight = h;
    glViewport( 0, 0, (GLfloat) w, (GLfloat) h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 90,(GLfloat) w/(GLfloat) h, 1.0f, 3000.0f );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
}

void Specialkey(int key, int x, int y)
{
    auto temp = alturaColisao;
    alturaColisao = g_bruteForce.GetScaledHeightAtPoint( esferaX/scale_map,
                                                        esferaZ/scale_map );
    temp-=alturaColisao;
    switch(key)
    {
        case GLUT_KEY_UP:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            cameraY += 1.0;
            break;
        case GLUT_KEY_DOWN:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            cameraY -= 1.0;
            break;
        case GLUT_KEY_LEFT:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaX -= 1.0;
            cameraX -= 1.0;
            if(esferaX <= 12.0){
                esferaX = 12.0;
                cameraX = 12.0;
            }
            break;
        case GLUT_KEY_RIGHT:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaX += 1.0;
            cameraX += 1.0;
            if(esferaX >= ((map_size*scale_map)-12)){
                esferaX = ((map_size*scale_map)-12);
                cameraX = ((map_size*scale_map)-12);
            }
            break;
    }
    esferaY = alturaColisao + offset;
    cameraY-=temp;
    g_camera.SetPosition( cameraX, cameraY, cameraZ );
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    auto temp = alturaColisao;
    alturaColisao = g_bruteForce.GetScaledHeightAtPoint( esferaX/scale_map,
                                                esferaZ/scale_map );
    temp-=alturaColisao;
    switch (key) {
        case 'z':
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaZ += 1.0;
            cameraZ += 1.0;
            if(esferaZ >= ((map_size*scale_map)-12)){
            esferaZ = ((map_size*scale_map)-12);
            cameraZ = esferaZ + offset_camera_player;
            }
            break;
        case 'Z':
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            esferaZ -= 1.0;
            cameraZ -= 1.0;
            if(esferaZ <= 12) {
                esferaZ = 12;
                cameraZ = esferaZ + offset_camera_player;
            }
            break;
        case 'p':
           g_camera.m_fPitch-= 1;
            break;
        case 'P':
           g_camera.m_fPitch+= 1;
            break;
        default:
            break;
    }
    esferaY = alturaColisao + offset;
    cameraY-=temp;
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
               g_camera.m_fYaw  += 1;
                break;
            case GLUT_MIDDLE_BUTTON:
                g_camera.m_fYaw  -= 1;
                break;
            default:
                break;
        }
    }
    glutPostRedisplay();
}

void menu(int item){
    if(item == 1){
        g_bruteForce.UnloadLightMap();
        g_bruteForce.UnloadHeightMap();
        g_bruteForce.UnloadAllTiles();
        g_bruteForce.UnloadTexture( );
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
    glutCreateMenu(menu);
    glutAddMenuEntry("Quit Program", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    init();
    glutMainLoop();
    return 0;
}

