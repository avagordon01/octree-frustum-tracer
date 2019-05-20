struct vector frustum_pos;
struct vector frustum_dirs[4];
if (frustum_pos[0] < 0) {
    //project and clip onto x = 0
    struct vector frustum_proj[4] = {
        frustum_pos + frustum_dirs[0] * (0 - frustum_pos.x[0]) / frustum_dirs[0].x[0],
        frustum_pos + frustum_dirs[1] * (0 - frustum_pos.x[0]) / frustum_dirs[1].x[0],
        frustum_pos + frustum_dirs[2] * (0 - frustum_pos.x[0]) / frustum_dirs[2].x[0],
        frustum_pos + frustum_dirs[3] * (0 - frustum_pos.x[0]) / frustum_dirs[3].x[0],
    };
    for (uint32_t n = 0; n < 4; n++) {
        //4 planes

        for (uint32_t i = 0; i < 4; i++) {
            struct vector p0 = frustum_proj[i];
            struct vector p1 = frustum_proj[(i + 1) % 4];

        }
    }
} else if (frustum_pos[0] > 4096) {
    //project and clip onto x = 4096
}
if (frustum_pos[1] < 0) {

} else if (frustum_pos[1] > 4096) {

}
if (frustum_pos[2] < 0) {

} else if (frustum_pos[2] > 4096) {

}




struct node n = nodes[node_index];
if (n.data != 0)
    goto render;
if (frustum_size <= 1)
    return 1;
if (n.offset != -1)
    goto split;

split:
    //go through voxel-surfaces in ray order
    //project onto surfaces
    //clip frustum by surface
    struct vector input_poly[n];
    struct vector output_polys[4][n + 2];
    for (uint32_t i = 0; i < n; i++) {
        struct vector p0 = input_poly[i];
        struct vector p1 = input_poly[(i + 1) % 4];
        struct vector m = p0 + p1 / 2;
        uint32_t q0 =
            ((((uint32_t)floor(p0.x[0]) >> n) & 1) << 0) |
            ((((uint32_t)floor(p0.x[1]) >> n) & 1) << 1) |
            ((((uint32_t)floor(p0.x[2]) >> n) & 1) << 2);
        uint32_t q1 =
            ((((uint32_t)floor(p1.x[0]) >> n) & 1) << 0) |
            ((((uint32_t)floor(p1.x[1]) >> n) & 1) << 1) |
            ((((uint32_t)floor(p1.x[2]) >> n) & 1) << 2);
        uint32_t qm =
            ((((uint32_t)floor(m.x[0]) >> n) & 1) << 0) |
            ((((uint32_t)floor(m.x[1]) >> n) & 1) << 1) |
            ((((uint32_t)floor(m.x[2]) >> n) & 1) << 2);
        switch (q0 ^ q1) {
            case 0:
                //same quadrant
                output_polys[q0].push(p0);
                break;
            case 1:
                //adjacent quadrant
                output_polys[q0].push(p0);
                output_polys[q0].push(intersection);
                output_polys[q1].push(center);
                output_polys[q1].push(intersection);
                break;
            case 2:
                //adjacent quadrant
                output_polys[q0].push(p0);
                output_polys[q0].push(intersection);
                output_polys[q1].push(center);
                output_polys[q1].push(intersection);
                break;
            case 3:
                //opposite quadrant
                output_polys[q0].push(p0);
                output_polys[q0].push(intersection0);
                output_polys[qm].push(center);
                output_polys[qm].push(intersection0);
                output_polys[qm].push(intersection1);
                output_polys[q1].push(center);
                output_polys[q1].push(intersection1);
                break;
            case 4:
                //adjacent quadrant
                output_polys[q0].push(p0);
                output_polys[q0].push(intersection);
                output_polys[q1].push(center);
                output_polys[q1].push(intersection);
                break;
            case 5:
                //opposite quadrant
                output_polys[q0].push(p0);
                output_polys[q0].push(intersection);
                output_polys[q1].push(center);
                output_polys[q1].push(intersection);
                break;
            case 6:
                //opposite quadrant
                output_polys[q0].push(p0);
                output_polys[q0].push(intersection0);
                output_polys[qm].push(center);
                output_polys[qm].push(intersection0);
                output_polys[qm].push(intersection1);
                output_polys[q1].push(center);
                output_polys[q1].push(intersection1);
                break;
            case 7:
                //impossible
                break;
        }
    }
