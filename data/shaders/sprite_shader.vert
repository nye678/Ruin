#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in float index;
layout(location = 2) uniform ivec2 textureSize;
layout(location = 3) uniform ivec2 tileDim;
layout(location = 4) uniform ivec2 tileSize;
layout(location = 6) uniform mat4 camera;
layout(location = 8) uniform mat3 uvMatrixTest;

out vec2 uv;

void main()
{
    const vec4 vertices[4] = vec4[4](
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(1.0, 1.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0));

    mat4 T = mat4(
        tileSize.x, 0, 0, 0,
        0, tileSize.y, 0, 0,
        0, 0, 1.0, 0,
        position.x, position.y, position.z, 1.0);

    gl_Position = camera * T * vertices[gl_VertexID];

    const vec3 uv_verts[4] = vec3[4](vec3(0.0, 0.0, 1.0),
                                     vec3(1.0, 0.0, 1.0),
                                     vec3(1.0, 1.0, 1.0),
                                     vec3(0.0, 1.0, 1.0));

    int x = int(index) % tileDim.x;
    int y = int(floor(float(index) / float(tileDim.x)));

    mat3 uvMatrix = mat3(1.0);
    uvMatrix[2].x = float(x) * float(tileSize.x) / float(textureSize.x);
    uvMatrix[2].y = float(y) * float(tileSize.y) / float(textureSize.y);
    uvMatrix[0].x = float(tileSize.x) / float(textureSize.x);
    uvMatrix[1].y = float(tileSize.y) / float(textureSize.y);

    uv = (uvMatrix * uv_verts[gl_VertexID]).xy;
    //uv = (uvMatrixTest * uv_verts[gl_VertexID]).xy;
}