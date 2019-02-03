//==============================================================
//==============================================================
//= skydome.h ==================================================
//= Original coders: Trent Polack (trent@voxelsoft.com)		   =
//==============================================================
//= Routines for a skydome (half-sphere) system				   =
//==============================================================
//==============================================================
#ifndef __SKYDOME_H__
#define __SKYDOME_H__


//--------------------------------------------------------------
//--------------------------------------------------------------
//- HEADERS AND LIBRARIES --------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#include "math_ops.h"
#include "image.h"


//--------------------------------------------------------------
//--------------------------------------------------------------
//- CLASS ------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
class CSKYDOME
{
	private:
		float* m_fVertices;
		float* m_fTexCoords;

		CVECTOR m_vecCenter;

		int m_iNumVertices;

    unsigned int m_uiTexID;
		CIMAGE m_texture;
   
    float CosineInterpolation( float fNum1, float fNum2, float x );
    float RangedRandom( int x, int y );
    float RangedSmoothRandom( int x, int y );
    float Noise( float x, float y );
    float FBM( float x, float y, float fOctaves, float fAmplitude, float fFrequency, float fH );
    
    void NormalizeFractal( float* fpData, int iSize );
    void BlurBand( float* fpBand, int iStride, int iCount, float fFilter );
    void Blur( float* fpData, int iSize, float fFilter  );
	
public:

	void Init( float fTheta, float fPhi, float fRadius );
	void Shutdown( void );

	void Render( float fDelta, bool bRotate );

    void GenCloudTexture( int size, float fBlur, float fOctaves, float fAmplitude, float fFrequency, float fH );
    
    void LoadTexture( char* szFilename )
	{	m_texture.Load( szFilename, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true );	}

	void Set( float fCX, float fCY, float fCZ )
	{	m_vecCenter.Set( fCX, fCY, fCZ );	}

	int GetNumVertices( void )
	{	return m_iNumVertices;	}

	int GetNumTriangles( void )
	{	return m_iNumVertices-1;	}

	CSKYDOME( void )
	{	}
	~CSKYDOME( void )
	{	}
};


#endif	//__SKYDOME_H__
