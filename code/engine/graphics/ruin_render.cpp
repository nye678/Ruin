#include "ruin_int.h"
#include "ruin_render.h"
#include "ruin_memory.h"
#include "ruin_math.h"
#include "ruin_assert.h"
#include "ruin_gl.h"
#include "ruin_texture.h"
#include "ruin_vector.h"
#include "ruin_matrix.h"

using namespace Ruin;

#define INITIAL_LIST_SIZE 200005

const char* DEBUG_VERTEX_CODE = R"VERTEX(
#version 440 core
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
)VERTEX";

const char* DEBUG_FRAG_CODE = R"FRAGMENT(
#version 440 core

in vec2 uv;
out vec4 end_color;

layout(location = 7) uniform sampler2D spriteTex;

void main()
{
    vec4 texColor = texture(spriteTex, uv);
    //end_color = vec4(texColor.r, texColor.g, texColor.b texColor.a);

    // BMPs are backwards for some reason.
    end_color = vec4(texColor.a, texColor.b, texColor.g, texColor.r);
}
)FRAGMENT";

//typedef uint64_t RenderKey;

union RenderKey
{
    uint64_t key;
    struct
    {
        uint64_t materialId: 30;
        uint64_t depth: 24;
        uint64_t translucencyType: 2;
        uint64_t viewportLayer: 3;
        uint64_t viewport: 3;
        uint64_t fullscreenLayer: 2;
    };
};

struct RenderCommand
{
    RenderKey key;
    Texture* texture;
    float2 position;
    float depth;
    uint32_t index;
};

struct RenderCommandList
{
    rsVector<RenderCommand> vec;
};

int32_t CompareRenderCommand(const RenderCommand &cmd1, const RenderCommand &cmd2)
{
   return (int32_t)((int64_t)cmd1.key.key - (int64_t)cmd2.key.key);

    /*if (cmd1->depth < cmd2->depth)
        return -1;
    else if (cmd2->depth > cmd2->depth)
        return 1;
    else // Equal Depth
    {
        if (cmd1->texture->unit < cmd2->texture->unit)
            return -1;
        else if (cmd1->texture->unit > cmd2->texture->unit)
            return 1;
        else // Same Texture
        {
            if (cmd1->position.y < cmd2->position.y)
                return -1;
            else if (cmd1->position.y > cmd2->position.y)
                return 1;
            else
                return 0;
        }
    }*/
}

inline bool operator< (const RenderCommand &lhs, const RenderCommand &rhs)
{
    return lhs.key.key < rhs.key.key;
}

void SortCommandList(RenderCommandList& list)
{
    rSort<RenderCommand>(list.vec);
}

struct Renderer
{
    RenderCommandList commands;

    GLuint vertexArray;
    GLuint positionBuffer;
    GLuint indexBuffer;

    GLuint shader;
};

struct DrawCall
{
    Texture* texture;
    uint32_t count;
    uint32_t offset;
};

Texture* DEBUG_TEXTURE = nullptr;

Renderer* CreateRenderer()
{
    Renderer* r = (Renderer*)Alloc(sizeof(Renderer));
    rInitialize<RenderCommand>(r->commands.vec, INITIAL_LIST_SIZE);

    glGenVertexArrays(1, &(r->vertexArray));
    glBindVertexArray(r->vertexArray);

    glGenBuffers(1, &r->positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, r->positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, INITIAL_LIST_SIZE * sizeof(float3), nullptr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
    glVertexAttribDivisor(0, 1);

    glGenBuffers(1, &r->indexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, r->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, INITIAL_LIST_SIZE * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0);
    glVertexAttribDivisor(1, 1);

    r->shader = CompileBasicShader(DEBUG_VERTEX_CODE, DEBUG_FRAG_CODE);

    BitMap* bmp = LoadBMP("D:\\Projects\\Ruin\\data\\textures\\rpg_sprite_walk.bmp");
    DEBUG_TEXTURE = CreateTexture(bmp, Int2(8, 4), Int2(24, 32));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);

    return r;
}

#define POSITION_LOC 0
#define TEXTURE_LOC 7
#define TEXTURE_SIZE_LOC 2
#define TILE_DIM_LOC 3
#define TILE_SIZE_LOC 4
#define TILE_INDEX_LOC 5
#define CAMERA_LOC 6

void LoadTexture(Texture* tex)
{
    glUniform1i(TEXTURE_LOC, tex->unit);
    glActiveTexture(GL_TEXTURE0 + tex->unit);
    glBindTexture(GL_TEXTURE_2D, tex->handle);

    glUniform2i(TEXTURE_SIZE_LOC, tex->width, tex->height);
    glUniform2i(TILE_DIM_LOC, tex->tileDim.x, tex->tileDim.y);
    glUniform2i(TILE_SIZE_LOC, tex->tileSize.x, tex->tileSize.y);
}

void MapBuffers(Renderer* r, float3* &positions, GLfloat* &indices)
{
    glBindBuffer(GL_ARRAY_BUFFER, r->positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, INITIAL_LIST_SIZE * sizeof(float2), nullptr, GL_STATIC_DRAW);
    
    positions = (float3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    
    glBindBuffer(GL_ARRAY_BUFFER, r->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, INITIAL_LIST_SIZE * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    
    indices = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void UnmapBuffers(Renderer* r, float3* &positions, GLfloat* &indices)
{
    glBindBuffer(GL_ARRAY_BUFFER, r->positionBuffer);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    positions = nullptr;
    
    glBindBuffer(GL_ARRAY_BUFFER, r->indexBuffer);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    indices = nullptr;
}

void Render(Renderer* r)
{
    glUseProgram(r->shader);
    glBindVertexArray(r->vertexArray);

    // Sort the Commands List
    SortCommandList(r->commands);

    float3* positions = nullptr;
    GLfloat* indices = nullptr;
    Texture* tex = nullptr;

    //mat4 camera = orthoProjection(0.0f, 1024.0f, -768.0f, 0.0f, 0.1f, 10000.0f);
    //mat4 camera = orthoProjection(0, 1024, 0, 768, 0, 1);
    mat4 camera = orthoProjection(0, 256, 0, 192, 0.0001f, 10000.0f);

    glUniformMatrix4fv(CAMERA_LOC, 1, GL_FALSE, camera.m);
    
    if (r->commands.vec.length > 0)
    {
        RenderCommand* cmd = rGet<RenderCommand>(r->commands.vec, 0);
        tex = cmd->texture;
        LoadTexture(cmd->texture);
        MapBuffers(r, positions, indices);
    }

    // Upload Positions and bucket
    uint32_t i = 0, p = 0;
    for (; i < r->commands.vec.length; ++i)
    {
        RenderCommand* cmd = rGet<RenderCommand>(r->commands.vec, i);

        //glUniform3f(POSITION_LOC, cmd->position.x, cmd->position.y, (float)cmd->depth);
        positions[i - p] = Float3(cmd->position.x, cmd->position.y, cmd->depth);
        indices[i - p] = (GLfloat)cmd->index;

        if (tex != cmd->texture)
        {
            UnmapBuffers(r, positions, indices);

            glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, i - p);

            tex = cmd->texture;
            LoadTexture(tex);
			MapBuffers(r, positions, indices);

            p = i+1;
        }
    }

    UnmapBuffers(r, positions, indices);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, i - p);
}

void CreateKey(RenderCommand &cmd, reFullScreenLayer layer)
{
    cmd.key.key = 0;
    cmd.key.key |= ((uint64_t)layer) << 62;
	cmd.key.key |= ((uint64_t)((fmin(cmd.depth, 10000.0f) / 10000.0f) * 0x00FFFFFF)) << 30;
    cmd.key.key |= ((uint64_t)(cmd.texture->unit));
}

void Draw(Renderer* r, float2 position, float depth, uint32_t index, reFullScreenLayer layer)
{
    RenderCommand cmd;
    cmd.depth = depth;
    cmd.texture = DEBUG_TEXTURE;
    cmd.position = position;
    cmd.index = index;
    CreateKey(cmd, layer);

    rPushBack<RenderCommand>(r->commands.vec, cmd);
}

void Draw(Renderer* r, Texture* texture, float2 position, float depth, uint32_t index, reFullScreenLayer layer)
{
    if (!r || !texture)
        return;

    RenderCommand cmd;
    cmd.depth = depth;
    cmd.texture = texture;
    cmd.position = position;
    cmd.index = index;
    CreateKey(cmd, layer);

    rPushBack<RenderCommand>(r->commands.vec, cmd);
}

void BeginRender(Renderer* r)
{
    rClear<RenderCommand>(&r->commands.vec);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EndRender(Renderer* r)
{
    Render(r);
}
