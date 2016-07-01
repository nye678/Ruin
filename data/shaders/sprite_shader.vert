#version 330 core

in vec3 position;
in float index;
uniform ivec2 textureSize;
uniform ivec2 tileDim;
uniform ivec2 tileSize;
uniform mat4 camera;

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
}
