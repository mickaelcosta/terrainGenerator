//
//  terrain.cpp
//  terrainGenerator
//
//  Created by User on 17/01/19.
//  Copyright © 2019 mickael. All rights reserved.
//

#include "terrain.hpp"

class TERRAIN
{
protected:
    SHEIGHT_DATA m_heightData; //the height data
    float m_fHeightScale; //scaling variable
public:
    int m_iSize; //must be a power of two
   
    virtual void Render( void )= 0;
    
    bool LoadHeightMap( char* szFilename, int iSize );
    bool SaveHeightMap( char* szFilename );
    bool UnloadHeightMap( void );
    
    //———————————————————————————————
    // Name: SetHeightScale - public
    // Description: Set the height scaling variable
    // Arguments: -fScale: how much to scale the terrain
    // Return Value: None
    //———————————————————————————————
    inline void SetHeightScale( float fScale )
    { m_fHeightScale= fScale; }
   
    //———————————————————————————————
    // Name: SetHeightAtPoint - public
    // Description: Set the true height value at the given point
    // Arguments: -ucHeight: the new height value for the point
    // -iX, iZ: which height value to retrieve
    // Return Value: None
    //———————————————————————————————
    inline void SetHeightAtPoint( unsigned char ucHeight,
                                 int iX, int iZ)
    { m_heightData.m_pucData[( iZ*m_iSize )+iX] = ucHeight; }
  
    //———————————————————————————————
    // Name: GetTrueHeightAtPoint - public
    // Description: A function to get the true height
    // value (0-255) at a point
    // Arguments: -iX, iZ: which height value to retrieve
    // Return Value: An unsigned char value: the true height at
    // the given point
    //———————————————————————————————
    inline unsigned char GetTrueHeightAtPoint( int iX, int iZ )
    { return ( m_heightData.m_pucData[( iZ*m_iSize )+iX] ); }
    //———————————————————————————————
    // Name: GetScaledHeightAtPoint - public
    // Description: Retrieve the scaled height at a given point
    // Arguments: -iX, iZ: which height value to retrieve
    // Return Value: A float value: the scaled height at the given
    // point
    //———————————————————————————————
    inline float GetScaledHeightAtPoint( int iX, int iZ )
        { return ( ( m_heightData.m_pucData[( iZ*m_iSize )+iX]
                    )*m_fHeightScale ); }
                                      
  TERRAIN( void ){ }
 ~TERRAIN( void ){ }
                                      
};

bool TERRAIN::LoadHeightMap( char* szFilename, int iSize )
{
    FILE* pFile;
    //check to see if the data has been set
    if( m_heightData.m_pucData )
        UnloadHeightMap( );
    
    pFile = fopen(szFilename, "rb");
    
    if(pFile == NULL){
        printf( "Could not open file.%s\n", szFilename );
        return false;
    }
    //allocate the memory for our height data
    m_heightData.m_pucData= new unsigned char [iSize*iSize];
    //check to see whether the memory was successfully allocated
    if( m_heightData.m_pucData==NULL )
    {
        //the memory could not be allocated
        //something is seriously wrong here
        printf( "Could not allocate memory for%s\n", szFilename );
        return false;
    }
    //read the heightmap into context
    fread( m_heightData.m_pucData, 1, iSize*iSize, pFile );
    //close the file
    fclose( pFile );
    //set the size data
    m_heightData.m_iSize= iSize;
    m_iSize = m_heightData.m_iSize;
    //Yahoo! The heightmap has been successfully loaded!
    printf( "Loaded %s\n", szFilename );
    return true;
}

bool TERRAIN::UnloadHeightMap( void )
{
    //check to see if the data has been set
    if( m_heightData.m_pucData )
    {
        //delete the data
        delete[] m_heightData.m_pucData;
        //reset the map dimensions, also
        m_heightData.m_iSize= 0;
    }
    //the heightmap has been unloaded
    printf( "Successfully unloaded the heightmap\n" );
    return true;
}

bool TERRAIN::SaveHeightMap( char *szFilename )
{
    FILE* pFile;

    pFile = fopen(szFilename, "rb");
    
    if(pFile == NULL){
        printf( "Could not open file.%s\n", szFilename );
        return false;
    }
   
    //read the heightmap into context
    fwrite( m_heightData.m_pucData, 1, m_heightData.m_iSize, pFile );
    //close the file
    fclose( pFile );

    printf( "File saved %s\n", szFilename );
    return true;
}

class BF_TERRAIN: public TERRAIN
{
    void Render( void )
    {
        unsigned char ucColor;
        int iZ;
        int iX;
        //loop through the Z axis of the terrain
        for( iZ=0; iZ<m_iSize-1; iZ++ )
        {
            //begin a new triangle strip
            glBegin( GL_TRIANGLE_STRIP );
            //loop through the X axis of the terrain
            //this is where the triangle strip is constructed
            for( iX=0; iX<m_iSize-1; iX++ )
            {
                //Use height-based coloring. (High-points are
                //light, and low points are dark.)
                ucColor= GetTrueHeightAtPoint( iX, iZ );
                //set the color with OpenGL, and render the point
                glColor3ub( ucColor, ucColor, ucColor );
                glVertex3f( iX, GetScaledHeightAtPoint( iX, iZ ), iZ );
                //Use height-based coloring. (High-points are
                //light, and low points are dark.)
                ucColor= GetTrueHeightAtPoint( iX, iZ+1 );
                //set the color with OpenGL, and render the point
                glColor3ub( ucColor, ucColor, ucColor );
                glVertex3f( iX,
                           GetScaledHeightAtPoint( iX, iZ+1 ),
                           iZ+1 );
            }
            //end the triangle strip
            glEnd( );
        }
    }
    
    
};
