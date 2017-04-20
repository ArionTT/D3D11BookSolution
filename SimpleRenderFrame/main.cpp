//#include"demos\texDemo.h"
#include"demos\texDemo.h"
#include<iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	////Create App Instance
	//InitDemo app(hInstance);
	//app.Init();
	//return app.Run();

	texDemo app(hInstance);
	app.Init();
	return app.Run();

}


////***********cmd mode************////
//int main()
//{
//	HINSTANCE hInstance;
//	hInstance = GetModuleHandle(NULL);
//
//	PolygonBuilder builder;
//	PolygonBuilder::Mesh cube;
//	builder.BuildCube(cube);
//
//	for (size_t i = 0; i < cube.vertices.size(); i++)
//	{
//		
//		std::cout <<"Pos "<<i<<": ("<< cube.vertices[i].position.x << "," << cube.vertices[i].position.y << "," << cube.vertices[i].position.z <<" )"<< std::endl;
//	}
//	std::cout<<std::endl<<std::endl;
//	for (size_t i = 0; i < cube.indices.size(); i+=3)
//	{
//		std::cout << "Ind " << i << ":[1]  " << cube.indices[i] << "  [2] " << cube.indices[i + 1] << "  [3] " << cube.indices[i + 2] << std::endl;
//	}
//
//
//	InitDemo app(hInstance);
//	app.Init();
//	return app.Run();
//}
