uint32_t traverse(
    uint32_t frustum_x, uint32_t frustum_y, uint32_t frustum_size,
    uint32_t voxel_x, uint32_t voxel_y, uint32_t voxel_z, uint32_t voxel_size, uint32_t voxel_node,
    uint32_t i
) {
    struct vector cone_pos = inputs.camera_pos;
    float x = 2 * 3.1415926535 / 6 * ((float)frustum_x + frustum_size / 2) / 1024 - 0.5;
    float y = 2 * 3.1415926535 / 6 * ((float)frustum_y + frustum_size / 2) / 1024 - 0.5;
    struct vector cone_dir = mulmv(inputs.camera_dir, (struct vector){{
        sin(x),
        sin(y),
        cos(x) * cos(y),
    }});
    struct vector sphere_pos = (struct vector){{
        voxel_x + voxel_size / 2,
        voxel_y + voxel_size / 2,
        voxel_z + voxel_size / 2
    }};
    float d1 = length(mulvs(cone_dir, dot(cone_dir, subvv(cone_pos, sphere_pos))));
    float d0 = length(subvv(subvv(cone_pos, sphere_pos), mulvs(cone_dir, dot(cone_dir, subvv(cone_pos, sphere_pos)))));
    float alpha = 2 * 3.14159265 / 6 * (float)frustum_size / 1024;
    float distance = d0 / cos(alpha);
    float cone_radius = (d1 + d0 * tan(alpha)) * sin(alpha);
    float sphere_radius = voxel_size;
    if (distance > sphere_radius + cone_radius) {
        //voxel outside frustum
        //ignore
    } else if (distance < cone_radius - sphere_radius && frustum_size != 1) {
        //voxel inside frustum
        goto split_frustum;
    } else if (distance < sphere_radius - cone_radius) {
        //frustum inside voxel
        if (nodes[voxel_node].data != 0) {
            goto render;
        } else if (nodes[voxel_node].offset != -1) {
            goto split_voxel;
        }
    } else if (sphere_radius > cone_radius && nodes[voxel_node].offset != -1) {
        //other intersection, voxel larger than frustum
        goto split_voxel;
    } else if (cone_radius > sphere_radius && frustum_size != 1) {
        //other intersection, frustum larger than voxel
        goto split_frustum;
    }
    return 0;
render:
    for (uint32_t y = frustum_y; y < frustum_y + frustum_size; y++) {
        for (uint32_t x = frustum_x; x < frustum_x + frustum_size; x++) {
            image[x + y * width][0] = 1;
            image[x + y * width][1] = 1;
            image[x + y * width][2] = 1;
        }
    }
    return 1;
split_frustum:
    traverse(frustum_x                   , frustum_y                   , frustum_size / 2, voxel_x, voxel_y, voxel_z, voxel_size, voxel_node, i + 1);
    traverse(frustum_x + frustum_size / 2, frustum_y                   , frustum_size / 2, voxel_x, voxel_y, voxel_z, voxel_size, voxel_node, i + 1);
    traverse(frustum_x                   , frustum_y + frustum_size / 2, frustum_size / 2, voxel_x, voxel_y, voxel_z, voxel_size, voxel_node, i + 1);
    traverse(frustum_x + frustum_size / 2, frustum_y + frustum_size / 2, frustum_size / 2, voxel_x, voxel_y, voxel_z, voxel_size, voxel_node, i + 1);
    return 0;
split_voxel:
    {}
    uint32_t index_x = cone_dir.x[0] > 0;
    uint32_t index_y = cone_dir.x[1] > 0;
    uint32_t index_z = cone_dir.x[2] > 0;
    uint32_t smallest = 0;
    uint32_t medium = 0;
    uint32_t largest = 0;
    cone_dir.x[0] = fabs(cone_dir.x[0]);
    cone_dir.x[1] = fabs(cone_dir.x[1]);
    cone_dir.x[2] = fabs(cone_dir.x[2]);
    if (cone_dir.x[0] < cone_dir.x[1]) {
        if (cone_dir.x[0] < cone_dir.x[2]) {
            if (cone_dir.x[1] < cone_dir.x[2]) {
                smallest = 0;
                medium = 1;
                largest = 2;
            } else {
                smallest = 0;
                medium = 2;
                largest = 1;
            }
        } else {
            smallest = 2;
            medium = 0;
            largest = 1;
        }
    } else {
        if (cone_dir.x[1] < cone_dir.x[2]) {
            if (cone_dir.x[0] < cone_dir.x[2]) {
                smallest = 1;
                medium = 0;
                largest = 2;
            } else {
                smallest = 1;
                medium = 2;
                largest = 0;
            }
        } else {
            smallest = 2;
            medium = 1;
            largest = 0;
        }
    }
    uint32_t mask[8][3] = {
        {1, 0, 0},
        {1, 1, 0},
        {1, 0, 0},
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 0},
        {1, 0, 0},
        {0, 0, 0},
    };
    for (uint32_t i = 0; i < 8; i++) {
        if (traverse(
            frustum_x,
            frustum_y,
            frustum_size,
            voxel_x + index_x * voxel_size / 2,
            voxel_y + index_y * voxel_size / 2,
            voxel_z + index_z * voxel_size / 2,
            voxel_size / 2,
            nodes[voxel_node].offset + ((index_x << 0) | (index_y << 1) | (index_z << 2)),
            i + 1
        )) return 1;
        index_x ^= mask[i][0];
        index_y ^= mask[i][1];
        index_z ^= mask[i][2];
    }
    return 0;
}
