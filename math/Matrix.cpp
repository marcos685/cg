#include "Matrix.hpp"

Matrix::Matrix()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = 0;
}

Matrix Matrix::operator+(Matrix &m)
{
    Matrix ret;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret(i, j) = this->m[i][j] + m(i, j);
    return ret;
}

Matrix Matrix::operator-(Matrix &m)
{
    Matrix ret;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret(i, j) = this->m[i][j] - m(i, j);
    return ret;
}

Matrix Matrix::operator*(Matrix &m)
{
    Matrix ret;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                ret(i, j) += this->m[i][k] * m(k, j);

    return ret;
}

double &Matrix::operator()(int i, int j)
{
    return m[i][j];
}

Point Matrix::operator*(Point &p)
{
    double ox, oy, oz;
    double newx, newy, newz;
    p.get_coordinates(&ox, &oy, &oz);
    newx = m[0][0] * ox + m[0][1] * oy + m[0][2] * oz + m[0][3];
    newy = m[1][0] * ox + m[1][1] * oy + m[1][2] * oz + m[1][3];
    newz = m[2][0] * ox + m[2][1] * oy + m[2][2] * oz + m[2][3];

    return Point(newx, newy, newz);
}

Vector Matrix::operator*(Vector &v)
{
    double ox, oy, oz;
    double newx, newy, newz;
    v.get_coordinates(&ox, &oy, &oz);
    newx = m[0][0] * ox + m[0][1] * oy + m[0][2] * oz;
    newy = m[1][0] * ox + m[1][1] * oy + m[1][2] * oz;
    newz = m[2][0] * ox + m[2][1] * oy + m[2][2] * oz;

    return Vector(newx, newy, newz);
}

Quaternion Matrix::operator*(Quaternion &q)
{
    double ox, oy, oz;
    double newx, newy, newz;
    Vector *vec = q.get_vec();
    (*vec).get_coordinates(&ox, &oy, &oz);
    newx = m[0][0] * ox + m[0][1] * oy + m[0][2] * oz + m[0][3] * q.get_scalar();
    newy = m[1][0] * ox + m[1][1] * oy + m[1][2] * oz + m[1][3] * q.get_scalar();
    newz = m[2][0] * ox + m[2][1] * oy + m[2][2] * oz + m[2][3] * q.get_scalar();

    return Quaternion(Vector(newx, newy, newz), q.get_scalar());
}

void Matrix::identity()
{
    for (int i = 0; i < 4; i++)
        m[i][i] = 1;
}

Matrix Matrix::inverse()
{
    // Ugly but fast inverse calculation
    Matrix inv_mat;
    double det;

    double adj2323 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
    double adj1323 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
    double adj1223 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
    double adj0323 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
    double adj0223 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
    double adj0123 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
    double adj2313 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
    double adj1313 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
    double adj1213 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
    double adj2312 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
    double adj1312 = m[1][1] * m[2][3] - m[1][3] * m[2][1];
    double adj1212 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    double adj0313 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
    double adj0213 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
    double adj0312 = m[1][0] * m[2][3] - m[1][3] * m[2][0];
    double adj0212 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
    double adj0113 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
    double adj0112 = m[1][0] * m[2][1] - m[1][1] * m[2][0];

    det = m[0][0] * (m[1][1] * adj2323 - m[1][2] * adj1323 + m[1][3] * adj1223) - m[0][1] * (m[1][0] * adj2323 - m[1][2] * adj0323 + m[1][3] * adj0223) + m[0][2] * (m[1][0] * adj1323 - m[1][1] * adj0323 + m[1][3] * adj0123) - m[0][3] * (m[1][0] * adj1223 - m[1][1] * adj0223 + m[1][2] * adj0123);
    det = 1 / det;

    inv_mat(0, 0) = det * (m[1][1] * adj2323 - m[1][2] * adj1323 + m[1][3] * adj1223);
    inv_mat(0, 1) = det * -(m[0][1] * adj2323 - m[0][2] * adj1323 + m[0][3] * adj1223);
    inv_mat(0, 2) = det * (m[0][1] * adj2313 - m[0][2] * adj1313 + m[0][3] * adj1213);
    inv_mat(0, 3) = det * -(m[0][1] * adj2312 - m[0][2] * adj1312 + m[0][3] * adj1212);
    inv_mat(1, 0) = det * -(m[1][0] * adj2323 - m[1][2] * adj0323 + m[1][3] * adj0223);
    inv_mat(1, 1) = det * (m[0][0] * adj2323 - m[0][2] * adj0323 + m[0][3] * adj0223);
    inv_mat(1, 2) = det * -(m[0][0] * adj2313 - m[0][2] * adj0313 + m[0][3] * adj0213);
    inv_mat(1, 3) = det * (m[0][0] * adj2312 - m[0][2] * adj0312 + m[0][3] * adj0212);
    inv_mat(2, 0) = det * (m[1][0] * adj1323 - m[1][1] * adj0323 + m[1][3] * adj0123);
    inv_mat(2, 1) = det * -(m[0][0] * adj1323 - m[0][1] * adj0323 + m[0][3] * adj0123);
    inv_mat(2, 2) = det * (m[0][0] * adj1313 - m[0][1] * adj0313 + m[0][3] * adj0113);
    inv_mat(2, 3) = det * -(m[0][0] * adj1312 - m[0][1] * adj0312 + m[0][3] * adj0112);
    inv_mat(3, 0) = det * -(m[1][0] * adj1223 - m[1][1] * adj0223 + m[1][2] * adj0123);
    inv_mat(3, 1) = det * (m[0][0] * adj1223 - m[0][1] * adj0223 + m[0][2] * adj0123);
    inv_mat(3, 2) = det * -(m[0][0] * adj1213 - m[0][1] * adj0213 + m[0][2] * adj0113);
    inv_mat(3, 3) = det * (m[0][0] * adj1212 - m[0][1] * adj0212 + m[0][2] * adj0112);

    return inv_mat;
}
