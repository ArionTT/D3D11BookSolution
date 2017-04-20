#pragma once
#include"CoreUtils.h"


class BasicCamera
{
public:
	BasicCamera();
	~BasicCamera();

	XMFLOAT4X4 GetView();
	XMFLOAT3 GetViewPos();

	void SetCamera(XMFLOAT3 pos, XMFLOAT3 at, XMFLOAT3 up);
	void UpdateCameraMatrix();

protected:

};
