//==============================================================
//==============================================================
//= brute_force.cpp ============================================
//= Original coders: Trent Polack (trent@voxelsoft.com)		   =
//==============================================================
//= This file (along with brute_force.h) contains the		   =
//= information for a brute force terrain implementation.	   =
//==============================================================
//==============================================================


//--------------------------------------------------------------
//--------------------------------------------------------------
//- HEADERS AND LIBRARIES --------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#include <stdio.h>
#include <GLUT/GLUT.h>
#include "brute_force.h"


//--------------------------------------------------------------
//--------------------------------------------------------------
//- DEFINITIONS ------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
// Name:			CBRUTE_FORCE::Render - public
// Description:		Render the terrain height field
// Arguments:		None
// Return Value:	None
//--------------------------------------------------------------
void CBRUTE_FORCE::Render( void )
{
	//unsigned char ucColor;
	//int	iZ;
	//int iX;
  //  GLfloat mat_specular_terreno[] = { 0.7, 0.7, 0.7, 1.0};
   // GLfloat mat_reflexao[] = { 20.0 };
    float fTexLeft, fTexBottom, fTexTop;
    int    x, z;
    
    //if the texture is loaded then enable texturing and bind our texture
    if( m_bTextureMapping )
    {
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, m_texture.GetID() );
    }
    
// glPushMatrix();
	//cull non camera-facing polygons
	glEnable( GL_CULL_FACE );

    //loop through the Z-axis of the terrain
    for( z=0; z<m_iSize-1; z++ )
    {
        //begin a new triangle strip
        glBegin( GL_TRIANGLE_STRIP );
        
        //loop through the X-axis of the terrain
        //this is where the triangle strip is constructed
        for( x=0; x<m_iSize-1; x++ )
        {
            //calculate the texture coordinates
            fTexLeft  = ( float )x/m_iSize;
            fTexBottom= ( float )z/m_iSize;
            fTexTop      = ( float )( z+1 )/m_iSize;
            
            //set the color with OpenGL, and render the point
            glColor3ub( 255, 255, 255 );
            glTexCoord2f( fTexLeft, fTexBottom );
            glVertex3f( ( float )x, GetScaledHeightAtPoint( x, z ), ( float )z );
            
            //set the color with OpenGL, and render the point
            glColor3ub( 255, 255, 255 );
            glTexCoord2f( fTexLeft, fTexTop );
            glVertex3f( ( float )x, GetScaledHeightAtPoint( x, z+1 ), ( float )z+1 );
            
        }
        
        //precisa disso ????
        m_texture.Unload();
		//end the triangle strip
		glEnd( );
	}
   // glPopMatrix();
}
