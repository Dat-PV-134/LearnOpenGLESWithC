//
// Created by Admin on 1/15/2025.
//

#ifndef LEARN_OPENGL_ES_WITH_C_MATRIX_H
#define LEARN_OPENGL_ES_WITH_C_MATRIX_H

#define MATRIX_SIZE 16

typedef struct {
    float mData[MATRIX_SIZE];
} matrix;

void matrix_identity(matrix *m);

int matrix_equals(const matrix *m1, const matrix *m2);

void matrix_load_with(matrix *m, const matrix *src);

void matrix_translate(matrix *m, float x, float y, float z);

void matrix_scale(matrix *m, float x, float y, float z);

void matrix_rotate(matrix *m, float radians, float x, float y, float z);

void matrix_multiply(matrix *m, const matrix *l, const matrix *r);

void matrix_print(const matrix *m, const char *label);

matrix* matrix_new_look_at(float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z);

matrix* matrix_new_frustum(float left, float right, float bottom, float top, float near, float far);

matrix* matrix_new_translate(float x, float y, float z);

matrix* matrix_new_scale(float x, float y, float z);

matrix* matrix_new_rotate(float radians, float x, float y, float z);

void matrix_multiply_vector(float *result, const matrix *m, const float *v);

void matrix_multiply_mv(float *r, const float *lhs, const float *rhs);

float matrix_length(float x, float y, float z);

#endif //LEARN_OPENGL_ES_WITH_C_MATRIX_H
