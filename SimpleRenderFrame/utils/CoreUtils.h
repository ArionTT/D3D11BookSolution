#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <d3dx11.h>
#include <xnamath.h>
#include <dxerr.h>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "../d3dx11effect.h"
/*======================================*/
/*                                           Define                                                  */
/*======================================*/

#define ReleaseCOM(x)  if(x){x->Release();x=0;}

#if defined (DEBUG)| defined(_DEBUG)
#ifndef HR																						
#define HR(x)																				\
{																										\
	HRESULT hr = (x);																		\
	if (FAILED(x))																				\
	{																									\
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true);		\
	}																									\
}
#endif // !HR

#else
	#ifndef HR
	#define HR(x) (x)
	#endif // !HR
#endif

/*======================================*/
/*                                         Math                                                      */
/*======================================*/

class CoreMath
{
public:
	template <typename T>
	static const T Min(T a, T b)
	{
		return a > b ? b : a;
	}
	template<typename T>
	static const T Max(T a, T b)
	{
		return a > b ? a : b;
	}
	//Return a*(1-t)+b*t
	template<typename T>
	static const T Lerp(T a,T b,float t)
	{
		return (a + b)*t + a;
	}
	//If in<low return low ,if in>hight return hight ,orther return in
	template<typename T>
	static const T Clamp(T in,T low,T high)
	{
		return in < low ? low : (in > high ? high : in);
	}
	// Returns random float in [0, 1).
	static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}
	// Returns random float in [a, b).
	static float RandF(float a, float b)
	{
		return a + RandF()*(b - a);
	}
	//Return Inverse of Matrix m 
	static XMMATRIX InverseTranspose(CXMMATRIX m)
	{
		XMMATRIX A = m;
		A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR det = XMMatrixDeterminant(A);
		return XMMatrixTranspose(XMMatrixInverse(&det, A));
	}
public:
	const static float PI;
	const static  float INV_PI;
	const static  float D_PI ;
	const static  float INV_D_PI ;
	const static  float EULER ;
	const static  float INFIN;
};





/*======================================*/
/*                                           Utils                                                     */
/*======================================*/
class CoreUtils
{
public:
	//Ture any type to wstring type
	template<typename T>
	static D3DX11INLINE std::wstring ToString(const T& s)
	{
		std::wostringstream oss;
		oss << s;
		return oss.str();
	}
	//Turn wstring to Type T
	//i.e. int i=FromString(L"25");
	template<typename T>
	static D3DX11INLINE T FromString(const std::wstring& s)
	{
		T x;
		std::wistringstream iss(s);
		iss >> x;
		return x;
	}
};


/*======================================*/
/*                                          Colors                                                   */
/*======================================*/


namespace Colors
{
	const float Yellow[]{ 0.996078f,0.996078f,0.254902f,1.0f };
	const float Crimson[]{ 1.0f,0.431373f,0.254902f,1.0f };
	const float Silver[]{ 0.733333f,0.733333f,0.733333f,1.0f };
	const float IronSilver[]{ 0.2333f,0.2333f,0.2333f,1.0f };
	const float PureBlack[]{ 0.0f,0.0f,0.0f,1.0f };
	const float SnowWhite[]{ 1.0f,1.0f,1.0f,1.0f };
	const float VioletBlue[]{ 0.541176f,0.168627f,0.886275f,1.0f };
	const float Cyan[]{ 0.0f,1.0f,1.0f,1.0f };
	const float DeepSkyBlue[]{ 0.0f,0.749020f,1.0f,1.0f };
	const float DarkOrange[]{ 1.0f,0.549020f,0.0f,1.0f };
	const float GreenYellow[]{ 0.678431f,1.0f,0.184314f,1.0f };
}


/*======================================*/
/*                                              Vertex                                              */
/*======================================*/

namespace Vertex
{
	struct NormalVertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
	};

	struct BaseVertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 tex;
	};

	struct ColorVertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT4 color;
	};

	const static D3D11_INPUT_ELEMENT_DESC NormalInputLayout[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	static const  D3D11_INPUT_ELEMENT_DESC BaseInputLayout[]=
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	const static D3D11_INPUT_ELEMENT_DESC ColorInputLayout[]=
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

}




/*======================================*/
/*                                           Materials                                             */
/*======================================*/

class CoreMaterial
{
public:
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT3 specular;
	FLOAT gloss;
};

// Globel Fog Effect Var
class Fog
{
public:
	XMFLOAT4 fogColor;
	//** x=start y=far range 
	XMFLOAT2 fogVar;
};



/*======================================*/
/*                                           Lighting                                              */
/*======================================*/



class DirectLight
{
public:
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 dir;
};


class PointLight
{
public:
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT4 antten;
	XMFLOAT3 pos;
	FLOAT rang;

};

class SpotLight
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
};
