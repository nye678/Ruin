#include "ruin_int.h"
#include "ruin_render.h"
#include "ruin_memory.h"
#include "ruin_math.h"
#include "ruin_assert.h"
#include "ruin_gl.h"
#include "ruin_texture.h"
#include "ruin_vector.h"
#include "ruin_matrix.h"
#include "ruin_file.h"
#include "ruin_render_types.h"

using namespace Ruin;

#define INITIAL_LIST_SIZE 200005

struct RenderCommandList
{
    rsVector<RenderCommand> vec;
};

int32_t CompareRenderCommand(const RenderCommand &cmd1, const RenderCommand &cmd2)
{
   return (int32_t)((int64_t)cmd1.key.key - (int64_t)cmd2.key.key);
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

    GLint positionLoc;
    GLint indexLoc;
    GLint textureSizeLoc;
    GLint tileDimLoc;
    GLint tileSizeLoc;
    GLint cameraLoc;
    GLint spriteTexLoc;  
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

    char *spriteShaderVertCode = nullptr, *spriteShaderFragCode = nullptr;
    LoadTextFile("..\\data\\shaders\\sprite_shader.vert", spriteShaderVertCode);
    LoadTextFile("..\\data\\shaders\\sprite_shader.frag", spriteShaderFragCode);

    r->shader = CompileBasicShader(spriteShaderVertCode, spriteShaderFragCode);
    Release(spriteShaderVertCode);
    Release(spriteShaderFragCode);

    r->positionLoc = glGetUniformLocation(r->shader, "position");
    r->indexLoc = glGetUniformLocation(r->shader, "index");
    r->textureSizeLoc = glGetUniformLocation(r->shader, "textureSize");
    r->tileDimLoc = glGetUniformLocation(r->shader, "tileDim");
    r->tileSizeLoc = glGetUniformLocation(r->shader, "tileSize");
    r->cameraLoc = glGetUniformLocation(r->shader, "camera");
    r->spriteTexLoc = glGetUniformLocation(r->shader, "spriteTex");

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

    BitMap* bmp = LoadBMP("..\\data\\textures\\default.bmp");
    DEBUG_TEXTURE = CreateTexture(bmp, Int2(1, 1), Int2(128, 128));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);

    return r;
}

void LoadTexture(Renderer* r, Texture* tex)
{
    glUniform1i(r->spriteTexLoc, tex->unit);
    glActiveTexture(GL_TEXTURE0 + tex->unit);
    glBindTexture(GL_TEXTURE_2D, tex->handle);

    glUniform2i(r->textureSizeLoc, tex->width, tex->height);
    glUniform2i(r->tileDimLoc, tex->tileDim.x, tex->tileDim.y);
    glUniform2i(r->tileSizeLoc, tex->tileSize.x, tex->tileSize.y);
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

    glUniformMatrix4fv(r->cameraLoc, 1, GL_FALSE, camera.m);
    
    if (r->commands.vec.length > 0)
    {
        RenderCommand* cmd = rGet<RenderCommand>(r->commands.vec, 0);
        tex = cmd->texture;
        LoadTexture(r, cmd->texture);
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
            LoadTexture(r, tex);
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
