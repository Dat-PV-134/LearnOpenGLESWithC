//
// Created by Admin on 1/15/2025.
//
#include "matrix.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <android/log.h>

#define LOG_TAG "Matrix"
#define LOG_I(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOG_E(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

void matrix_identity(matrix *m) {
    m->mData[0] = 1.0f;
    m->mData[1] = 0.0f;
    m->mData[2] = 0.0f;
    m->mData[3] = 0.0f;

    m->mData[4] = 0.0f;
    m->mData[5] = 1.0f;
    m->mData[6] = 0.0f;
    m->mData[7] = 0.0f;

    m->mData[8] = 0.0f;
    m->mData[9] = 0.0f;
    m->mData[10] = 1.0f;
    m->mData[11] = 0.0f;

    m->mData[12] =0.0f;
    m->mData[13] = 0.0f;
    m->mData[14] = 0.0f;
    m->mData[15] = 1.0f;
}

int matrix_equals(const matrix *m1, const matrix *m2) {
    int equals = 1;
    const float *d = m2->mData;
    for (int i = 0; i < MATRIX_SIZE && equals; ++i) {
        if (m1->mData[i] != d[i]) {
            equals = 0;
        }
    }
    return equals;
}

void matrix_load_with(matrix *m, const matrix *src) {
    memcpy(m->mData, src->mData, MATRIX_SIZE * sizeof(float));
}

void matrix_translate(matrix *m, float x, float y, float z) {
    matrix *mat = matrix_new_translate(x, y, z);
    matrix temp = *m;
    if (mat != NULL) {
        matrix_multiply(m, &temp, mat);
    }
    free(mat);
}

void matrix_scale(matrix *m, float x, float y, float z) {
    matrix *mat = matrix_new_scale(x, y, z);
    matrix temp = *m;
    if (mat != NULL) {
        matrix_multiply(m, &temp, mat);
    }
    free(mat);
}

void matrix_rotate(matrix *m, float radians, float x, float y, float z) {
    matrix *mat = matrix_new_rotate(radians, x, y, z);
    matrix temp = *m;
    if (mat != NULL) {
        matrix_multiply(m, &temp, mat);
    }
    free(mat);
}

void matrix_multiply(matrix *m, const matrix *l, const matrix *r) {
    const float *lhs = l->mData;
    const float *rhs = r->mData;
    for (int i = 0; i < 4; ++i) {
        const int i4 = i * 4;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;

        for (int j = 0; j < 4; ++j) {
            const int j4 = j * 4;
            const float e = rhs[i4 + j];
            x += lhs[j4 + 0] * e;
            y += lhs[j4 + 1] * e;
            z += lhs[j4 + 2] * e;
            w += lhs[j4 + 3] * e;
        }

        m->mData[i4 + 0] = x;
        m->mData[i4 + 1] = y;
        m->mData[i4 + 2] = z;
        m->mData[i4 + 3] = w;
    }
}

void matrix_print(const matrix *m, const char *label) {
    LOG_I("%c", *label);
    for (int i = 0; i < 4; ++i) {
        const float *d = &(m->mData[i * 4]);
        LOG_I("%f %f %f %f\n", d[0], d[1], d[2], d[3]);
    }
}

matrix* matrix_new_look_at(float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z) {
    matrix *m = (matrix *) malloc(sizeof(matrix));
    if (m != NULL) {
        float fx = center_x - eye_x;
        float fy = center_y - eye_y;
        float fz = center_z - eye_z;

        float rlf = 1.0f / (float) sqrtf(fx * fx + fy * fy + fz * fz);
        fx *= rlf;
        fy *= rlf;
        fz *= rlf;

        float sx = fy * up_z - fz * up_y;
        float sy = fz * up_x - fx * up_z;
        float sz = fx * up_y - fy * up_x;

        float rls = 1.0f / (float) sqrtf(sx * sx + sy * sy + sz * sz);
        sx *= rls;
        sy *= rls;
        sz *= rls;

        float ux = sy * fz - sz * fy;
        float uy = sz * fx - sx * fz;
        float uz = sx * fy - sy * fx;

        float *d = m->mData;
        d[0] = sx;
        d[1] = ux;
        d[2] = -fx;
        d[3] = 0.0f;

        d[4] = sy;
        d[5] = uy;
        d[6] = -fy;
        d[7] = 0.0f;

        d[8] = sz;
        d[9] = uz;
        d[10] = -fz;
        d[11] = 0.0f;

        d[12] = 0.0f;
        d[13] = 0.0f;
        d[14] = 0.0f;
        d[15] = 1.0f;

        matrix_translate(m, -eye_x, -eye_y, -eye_z);
    }
    return m;
}

matrix* matrix_new_frustum(float left, float right, float bottom, float top, float near, float far);

matrix* matrix_new_translate(float x, float y, float z);

matrix* matrix_new_scale(float x, float y, float z);

matrix* matrix_new_rotate(float radians, float x, float y, float z);

void matrix_multiply_vector(float *result, const matrix *m, const float *v);

void matrix_multiply_mv(float *r, const float *lhs, const float *rhs);

float matrix_length(float x, float y, float z);
