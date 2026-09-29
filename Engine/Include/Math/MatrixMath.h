#pragma once
#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"

Matrix4x4 MakeIdentityMatrix();
/// <summary>
///　translation matrix作成. 
/// </summary>
Matrix4x4 MakeTranslateMatrix(const Vector3& position);