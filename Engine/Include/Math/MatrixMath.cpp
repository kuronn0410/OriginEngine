
#include "Vector3.h"
#include "Matrix4x4.h"
#include "MatrixMath.h"
#include <cmath>

Matrix4x4 MakeIdentityMatrix()
{
    Matrix4x4 matrix{};
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}
Matrix4x4 MakeTranslateMatrix(const Vector3& position)
{
    Matrix4x4 matrix = MakeIdentityMatrix();
    matrix.m[3][0] = position.x;
    matrix.m[3][1] = position.y;
    matrix.m[3][2] = position.z;
    return matrix;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
    Matrix4x4 matrix = MakeIdentityMatrix();
    matrix.m[0][0] = scale.x;
    matrix.m[1][1] = scale.y;
    matrix.m[2][2] = scale.z;
    return matrix;
}

Matrix4x4 MakeRotateMatrix(const Vector3& rotation)
{
    Matrix4x4 rotateX = MakeRotateXMatrix(rotation.x);
    Matrix4x4 rotateY = MakeRotateYMatrix(rotation.y);
    Matrix4x4 rotateZ = MakeRotateZMatrix(rotation.z);
    Matrix4x4 matrix = MakeIdentityMatrix();

    matrix = Multiply(matrix, rotateX);
    matrix = Multiply(matrix, rotateY);
    matrix = Multiply(matrix, rotateZ);

    return matrix;
}

Matrix4x4 MakeRotateXMatrix(float angle)
{
    float radian = angle * 3.14159265358979323846f / 180.0f;
    Matrix4x4 matrix = MakeIdentityMatrix();
    float c = std::cos(radian);
    float s = std::sin(radian);

    matrix.m[1][1] = c;
    matrix.m[1][2] = s;
    matrix.m[2][1] = -s;
    matrix.m[2][2] = c;

    return matrix;
}
Matrix4x4 MakeRotateYMatrix(float angle)
{
	float radian = angle * 3.14159265358979323846f / 180.0f;
	Matrix4x4 matrix = MakeIdentityMatrix();
	float c = std::cos(radian);
	float s = std::sin(radian);
	matrix.m[0][0] = c;
	matrix.m[0][2] = -s;
	matrix.m[2][0] = s;
	matrix.m[2][2] = c;
	return matrix;
}

Matrix4x4 MakeRotateZMatrix(float angle)
{
	float radian = angle * 3.14159265358979323846f / 180.0f;
	Matrix4x4 matrix = MakeIdentityMatrix();
	float c = std::cos(radian);
	float s = std::sin(radian);
	matrix.m[0][0] = c;
	matrix.m[0][1] = s;
	matrix.m[1][0] = -s;
	matrix.m[1][1] = c;
	return matrix;
}   

/*
Translate
[           ][          ][          ][       ]
[           ][          ][          ][       ]
[           ][          ][          ][       ]
[position.x ][position.y][position.z][       ]
Scale
[scale.x][          ][          ][      ]
[       ][ scale.y  ][          ][      ]
[       ][          ][ scale.z  ][      ]
[       ][          ][          ][      ]
Rotate
[       ][       ][       ][       ]
[       ][       ][       ][       ]
[       ][       ][       ][       ]
[       ][       ][       ][       ]
*/

Matrix4x4 Multiply(
    const Matrix4x4& matrix1,
    const Matrix4x4& matrix2)
{
    Matrix4x4 result{};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                result.m[i][j] += matrix1.m[i][k] * matrix2.m[k][j];
            }
        }
    }

    return result;
}
