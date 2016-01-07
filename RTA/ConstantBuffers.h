#include "SharedDefines.h"

CONSTANT_BUFFER_BEGIN(cbPerObject, b0)
float4x4	gMVP;
float4x4	gWorld;
#ifdef __cplusplus
const static int REGISTER_SLOT = 0;
#endif
CONSTANT_BUFFER_END

CONSTANT_BUFFER_BEGIN(cbPerCamera, b1)
float4x4	gViewProj;
float4x4	gInvViewProj;
float3 gCameraPos;
float gFarDist;
float2 gScreenSize;
float gPlaneX;
float gPlaneY;
float3 gCameraDir;
#ifdef __cplusplus
const static int REGISTER_SLOT = 1;
#endif
CONSTANT_BUFFER_END