#include "math_types.h"
#include "string.h"
#include "util.h"
#include <math.h>

static always_inline pure Mat4 Mat4_transpose_inline(const Mat4 m) {
    const float (*const c)[4] = &m.columns[0].entries;
    return (Mat4) {{
        {{c[0][0], c[1][0], c[2][0], c[3][0]}},
        {{c[0][1], c[1][1], c[2][1], c[3][1]}},
        {{c[0][2], c[1][2], c[2][2], c[3][2]}},
        {{c[0][3], c[1][3], c[2][3], c[3][3]}},
    }};
}

pure Mat4 Mat4_transpose(const Mat4 m) {
    return Mat4_transpose_inline(m);
}

pure Mat4 Mat4_identity() {
    return (Mat4) {{
        {{1, 0, 0, 0}},
        {{0, 1, 0, 0}},
        {{0, 0, 1, 0}},
        {{0, 0, 0, 1}},
    }};
}

pure Vec4 Mat4_apply(const Mat4 matrix, const Vec4 vector) {
    const float (*const m)[4] = &matrix.columns[0].entries;
    const float *const v = &vector.entries[0];

    return (Vec4) {{
        m[0][0]*v[0] + m[1][0]*v[1] + m[2][0]*v[2] + m[3][0]*v[3],
        m[0][1]*v[0] + m[1][1]*v[1] + m[2][1]*v[2] + m[3][1]*v[3],
        m[0][2]*v[0] + m[1][2]*v[1] + m[2][2]*v[2] + m[3][2]*v[3],
        m[0][3]*v[0] + m[1][3]*v[1] + m[2][3]*v[2] + m[3][3]*v[3],
    }};
}

// Aspect ratio: w/h
pure Mat4 Mat4_perspective(const float fov, const float aspect_ratio, const float znear, const float zfar) {
    const float fov_multiplier = tan(fov);

    // Note: Mat4 is defined as an array of columns, so we need to transpose the initializer
    return Mat4_transpose_inline((Mat4) {{
        {{fov_multiplier, 0,                            0,      0          }},
        {{0,              -fov_multiplier*aspect_ratio, 0,      0          }},
        {{0,              0,                            1/zfar, -znear/zfar}},
        {{0,              0,                            1,      0          }},
    }});
}

pure Mat4 Mat4_yaw_pitch_roll(const float yaw, const float pitch, const float roll) {
    const float cos_theta_y = cos(yaw);
    const float cos_theta_p = cos(pitch);
    const float cos_theta_r = cos(roll);
    const float sin_theta_y = sin(yaw);
    const float sin_theta_p = sin(pitch);
    const float sin_theta_r = sin(roll);

    return Mat4_transpose_inline((Mat4) {{
        {{cos_theta_r*cos_theta_y-sin_theta_p*sin_theta_r*sin_theta_y, sin_theta_p*cos_theta_r*sin_theta_y+sin_theta_r*cos_theta_y, -cos_theta_p*sin_theta_y, 0}},
        {{-cos_theta_p*sin_theta_r,                                    cos_theta_p*cos_theta_r,                                     sin_theta_p,              0}},
        {{cos_theta_r*sin_theta_y+sin_theta_p*sin_theta_r*cos_theta_y, sin_theta_r*sin_theta_y-sin_theta_p*cos_theta_r*cos_theta_y, cos_theta_p*cos_theta_y,  0}},
        {{0,                                                           0,                                                           0,                        1}},
    }});
}

pure Mat4 Mat4_inverse_yaw_pitch_roll(const float yaw, const float pitch, const float roll) {
    return Mat4_transpose(Mat4_yaw_pitch_roll(yaw, pitch, roll));
}

pure Mat4 Mat4_yaw_pitch_roll_translate(const float yaw, const float pitch, const float roll, const Vec3 translation) {
    const float *const t = &translation.entries[0];
    Mat4 mat = Mat4_yaw_pitch_roll(yaw, pitch, roll);

    const Vec4 offset_component = {{t[0], t[1], t[2], 1}};
    memcpy(&mat.columns[3], &offset_component, sizeof(offset_component));

    return mat;
}

pure Mat4 Mat4_inverse_yaw_pitch_roll_translate(const float yaw, const float pitch, const float roll, const Vec3 translation) {
    const float *const t = &translation.entries[0];
    Mat4 mat = Mat4_inverse_yaw_pitch_roll(yaw, pitch, roll);
    
    const Vec4 offset_component = Mat4_apply(mat, (Vec4){{-t[0], -t[1], -t[2], 1}});
    memcpy(&mat.columns[3], &offset_component, sizeof(offset_component));

    return mat;
}
