
#include "Vector3.h"
#include "Matrix4x4.h"
#include "MatrixMath.h"


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