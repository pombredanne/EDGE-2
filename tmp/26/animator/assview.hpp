#pragma once

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

extern unsigned int checker_texture, textureless_texture;

void initchecker(void);
void inittextureless(void);
unsigned int loadtexture(char *filename);
unsigned int loadmaterial(aiMaterial *material);

/*
 * Some extra matrix, vector and quaternion functions.
 *
 * Many of these already exist in some variant in assimp,
 * but not exported to the C interface.
 */

// convert 4x4 to column major format for opengl
void transposematrix(float m[16], const aiMatrix4x4 *p);
void extract3x3(aiMatrix3x3 *m3, const aiMatrix4x4 *m4);
void mixvector(aiVector3D *p, const aiVector3D *a, const aiVector3D *b, float t);
float dotquaternions(const aiQuaternion *a, const aiQuaternion *b);
void normalizequaternion(aiQuaternion *q);
void mixquaternion(aiQuaternion *q, const aiQuaternion *a, const aiQuaternion *b, float t);
void composematrix(aiMatrix4x4 *m, const aiVector3D *t, const aiQuaternion *q, const aiVector3D *s);

/*
 * Init, animate and draw aiScene.
 *
 * We build our own list of the meshes to keep the vertex data
 * in a format suitable for use with OpenGL. We also store the
 * resulting meshes during skeletal animation here.
 */

extern int vertexcount, facecount; // for statistics only

// opengl (and skinned vertex) buffers for the meshes
extern int meshcount;
struct glMesh {
	aiMesh *mesh;
	unsigned int texture;
	int vertexcount, elementcount;
	float *position;
	float *normal;
	float *texcoord;
	unsigned char *color;
	int *element;
};
extern glMesh *meshlist;

// find a node by name in the hierarchy (for anims and bones)
aiNode *findnode(aiNode *node, char *name);

// calculate absolute transform for node to do mesh skinning
void transformnode(aiMatrix4x4 *result, aiNode *node);

void transformmesh(aiScene *scene, glMesh *mesh);
void initmesh(aiScene *scene, glMesh *mesh, aiMesh *amesh);
void initscene(aiScene *scene);
void drawmesh(glMesh *mesh);
void drawnode(aiNode *node, aiMatrix4x4 world);
void drawscene(aiScene *scene);

void measuremesh(glMesh *mesh, aiMatrix4x4 transform, float bboxmin[3], float bboxmax[3]);
void measurenode(aiNode *node, aiMatrix4x4 world, float bboxmin[3], float bboxmax[3]);
float measurescene(aiScene *scene, float center[3]);

int animationlength(aiAnimation *anim);

struct blender {
	aiNode *node;	// node->mTransformation holds the transformation
	aiVector3D p;	// position
	aiQuaternion r;	// rotation
	aiVector3D s;	// scale
};
using blenders = std::vector<blender>;

blender mix( const blender &a, const blender &b, float dt );
blenders mix( const blenders &A, const blenders &B, float dt );
blenders blendscene(aiScene *scene, aiAnimation *anim, int frameA, int frameB, float t);

extern float blendtick;
extern   int blendpass;

int get_game_time();
void animatescene(aiScene *scene, aiAnimation *anim, float tick);
void animatescene(aiScene *scene, aiAnimation *anim, int frameA, int frameB, float t, int stage );
void printscene(const aiScene* scene );

/*
 * Boring UI and GLUT hooks.
 */

extern float animtick;
extern volatile int exiting;

struct camera_t {
	float distance;
	float yaw;
	float pitch;
	float center[3];
};
extern camera_t camera;

