#include "BasicCamera.h"

BasicCamera::BasicCamera()
{
}

BasicCamera::~BasicCamera()
{
}

XMFLOAT4X4 BasicCamera::GetView()
{
	return XMFLOAT4X4();
}

XMFLOAT3 BasicCamera::GetViewPos()
{
	return XMFLOAT3();
}

void BasicCamera::SetCamera(XMFLOAT3 pos, XMFLOAT3 at, XMFLOAT3 up)
{
}

void BasicCamera::UpdateCameraMatrix()
{
}
