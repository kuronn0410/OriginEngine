#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"

Matrix4x4 MakeIdentityMatrix();
/// <summary>
///　translation matrix作成. 
/// </summary>
Matrix4x4 MakeTranslateMatrix(const Vector3& position);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeRotateMatrix(const Vector3& rotation);
Matrix4x4 MakeRotateZMatrix(float angle);
Matrix4x4 MakeRotateXMatrix(float angle);
Matrix4x4 MakeRotateYMatrix(float angle);

Matrix4x4 MakeInverseMatrix(const Matrix4x4& matrix);

Matrix4x4 Multiply(
    const Matrix4x4& matrix1,
    const Matrix4x4& matrix2
);