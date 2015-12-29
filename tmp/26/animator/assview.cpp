#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "opengl.hpp"
#include "assview.hpp"
#include "asset.hpp"
#include "debugdraw.hpp"
#include <spot/spot.hpp>

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define CLAMP(x,a,b) MIN(MAX(x,a),b)

/*
 * Use Sean Barrett's excellent stb_image to load textures.
 */

unsigned char checker_data[256*256];
unsigned int checker_texture = 0, textureless_texture = 0;

void initchecker(void)
{
	int x, y, i = 0;
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			int k = ((x>>5) & 1) ^ ((y>>5) & 1);
			checker_data[i++] = k ? 255 : 192;
		}
	}
	glGenTextures(1, &checker_texture);
	glBindTexture(GL_TEXTURE_2D, checker_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 1, 256, 256, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, checker_data);
}
void inittextureless(void)
{
	glGenTextures(1, &textureless_texture);
	glBindTexture(GL_TEXTURE_2D, textureless_texture);
}

void lowerstring(char *s)
{
	while (*s) { *s = tolower(*s); s++; }
}

unsigned int loadtexture(char *filename)
{
	unsigned int texture;
	std::vector<unsigned char> image;
	size_t w, h, n = 4;
	int intfmt = 0, fmt = 0;

	char *pathfile = (char *)malloc( 4096 );
#if 0
	strcpy(pathfile, basedir());
	strcat(pathfile, filename);
#else
	strcpy(pathfile, filename);
#endif
	std::cout << "decoding " << pathfile << " ..." << std::endl;
	image = spot::decode8(std::string(pathfile), &w, &h, &n);
	if (image.empty()) {
		lowerstring(pathfile);
		std::cout << "decoding " << pathfile << " ..." << std::endl;
		image = spot::decode8(std::string(pathfile), &w, &h, &n);
		if (image.empty()) {
			strcat(pathfile, ".webp");
			std::cout << "decoding " << pathfile << " ..." << std::endl;
			image = spot::decode8(std::string(pathfile), &w, &h, &n);
		}
		if (image.empty()) {
			std::string file, dir = std::string(pathfile);
			file = dir.substr( dir.find_last_of('/') + 1 );
			dir  = dir.substr( 0, dir.find_last_of('/') );
			file = std::string() + dir + "/../" + file;
			std::cout << "decoding " << file << " ..." << std::endl;
			image = spot::decode8(file.c_str(), &w, &h, &n);
			if(image.empty()) fprintf(stderr, "cannot load texture '%s'\n", file.c_str());
		}
		if (image.empty()) {
			pathfile[strlen(pathfile) - 5] = '.';
			pathfile[strlen(pathfile) - 4] = 'j';
			pathfile[strlen(pathfile) - 3] = 'p';
			pathfile[strlen(pathfile) - 2] = 'g';
			pathfile[strlen(pathfile) - 1] = '\0';
			std::cout << "decoding " << pathfile << " ..." << std::endl;
			image = spot::decode8(std::string(pathfile), &w, &h, &n);
		}
		if (image.empty()) {
			std::string file, dir = std::string(pathfile);
			file = dir.substr( dir.find_last_of('/') + 1 );
			dir  = dir.substr( 0, dir.find_last_of('/') );
			file = std::string() + dir + "/../" + file;
			std::cout << "decoding " << file << " ..." << std::endl;
			image = spot::decode8(file, &w, &h, &n);
			if(image.empty()) fprintf(stderr, "cannot load texture '%s'\n", file.c_str());
		}
		if (image.empty()) {
			fprintf(stderr, "cannot load texture '%s'\n", pathfile);
			return 0;
		}
	}
	if( image.empty() ) {
		fprintf(stderr, "%s\n", "texture loaded");
	}
	free( pathfile );

	if (n == 1) { intfmt = fmt = GL_LUMINANCE; }
	if (n == 2) { intfmt = fmt = GL_LUMINANCE_ALPHA; }
	if (n == 3) { intfmt = fmt = GL_RGB; }
	if (n == 4) { intfmt = fmt = GL_RGBA; }

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, intfmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, image.data());
	//glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}

unsigned int loadmaterial(aiMaterial *material)
{
	char filename[2000];
	aiString str;
	// also, AI_MATKEY_TEXTURE_NORMALS AI_MATKEY_TEXTURE_HEIGHT
	if (!aiGetMaterialString(material, AI_MATKEY_TEXTURE_DIFFUSE(0), &str)) {
		char *s = strrchr(str.data, '/');
		if (!s) s = strrchr(str.data, '\\');
		if (!s) s = str.data; else s++;
		strcpy(filename, basedir());
		strcat(filename, s);
		return loadtexture(filename);
	}
	return 0;
}

/*
 * Some extra matrix, vector and quaternion functions.
 *
 * Many of these already exist in some variant in assimp,
 * but not exported to the C interface.
 */

// convert 4x4 to column major format for opengl
void transposematrix(float m[16], const aiMatrix4x4 *p)
{
	m[0] = p->a1; m[4] = p->a2; m[8] = p->a3; m[12] = p->a4;
	m[1] = p->b1; m[5] = p->b2; m[9] = p->b3; m[13] = p->b4;
	m[2] = p->c1; m[6] = p->c2; m[10] = p->c3; m[14] = p->c4;
	m[3] = p->d1; m[7] = p->d2; m[11] = p->d3; m[15] = p->d4;
}

void extract3x3(aiMatrix3x3 *m3, const aiMatrix4x4 *m4)
{
	m3->a1 = m4->a1; m3->a2 = m4->a2; m3->a3 = m4->a3;
	m3->b1 = m4->b1; m3->b2 = m4->b2; m3->b3 = m4->b3;
	m3->c1 = m4->c1; m3->c2 = m4->c2; m3->c3 = m4->c3;
}

void mixvector(aiVector3D *p, const aiVector3D *a, const aiVector3D *b, float t)
{
	p->x = a->x + t * (b->x - a->x);
	p->y = a->y + t * (b->y - a->y);
	p->z = a->z + t * (b->z - a->z);
}

float dotquaternions(const aiQuaternion *a, const aiQuaternion *b)
{
	return a->x*b->x + a->y*b->y + a->z*b->z + a->w*b->w;
}

void normalizequaternion(aiQuaternion *q)
{
	float d = sqrt(dotquaternions(q, q));
	if (d >= 0.00001) {
		d = 1 / d;
		q->x *= d;
		q->y *= d;
		q->z *= d;
		q->w *= d;
	} else {
		q->x = q->y = q->z = 0;
		q->w = 1;
	}
}

void mixquaternion(aiQuaternion *q, const aiQuaternion *a, const aiQuaternion *b, float t)
{
	if (dotquaternions(a, b) < 0) {
		aiQuaternion tmp;
		tmp.x = -a->x; tmp.y = -a->y; tmp.z = -a->z; tmp.w = -a->w;
		a = &tmp;
	}
	q->x = a->x + t * (b->x - a->x);
	q->y = a->y + t * (b->y - a->y);
	q->z = a->z + t * (b->z - a->z);
	q->w = a->w + t * (b->w - a->w);
	normalizequaternion(q);
}

void composematrix(aiMatrix4x4 *m,
	const aiVector3D *t, const aiQuaternion *q, const aiVector3D *s)
{
	// quat to rotation matrix
	m->a1 = 1 - 2 * (q->y * q->y + q->z * q->z);
	m->a2 = 2 * (q->x * q->y - q->z * q->w);
	m->a3 = 2 * (q->x * q->z + q->y * q->w);
	m->b1 = 2 * (q->x * q->y + q->z * q->w);
	m->b2 = 1 - 2 * (q->x * q->x + q->z * q->z);
	m->b3 = 2 * (q->y * q->z - q->x * q->w);
	m->c1 = 2 * (q->x * q->z - q->y * q->w);
	m->c2 = 2 * (q->y * q->z + q->x * q->w);
	m->c3 = 1 - 2 * (q->x * q->x + q->y * q->y);

	// scale matrix
	m->a1 *= s->x; m->a2 *= s->x; m->a3 *= s->x;
	m->b1 *= s->y; m->b2 *= s->y; m->b3 *= s->y;
	m->c1 *= s->z; m->c2 *= s->z; m->c3 *= s->z;

	// set translation
	m->a4 = t->x; m->b4 = t->y; m->c4 = t->z;

	m->d1 = 0; m->d2 = 0; m->d3 = 0; m->d4 = 1;
}

/*
 * Init, animate and draw aiScene.
 *
 * We build our own list of the meshes to keep the vertex data
 * in a format suitable for use with OpenGL. We also store the
 * resulting meshes during skeletal animation here.
 */

int vertexcount = 0, facecount = 0; // for statistics only

// opengl (and skinned vertex) buffers for the meshes
int meshcount = 0;
glMesh *meshlist = NULL;

// find a node by name in the hierarchy (for anims and bones)
aiNode *findnode(aiNode *node, char *name)
{
	int i;
	if (!strcmp(name, node->mName.data))
		return node;
	for (i = 0; i < node->mNumChildren; i++) {
		aiNode *found = findnode(node->mChildren[i], name);
		if (found)
			return found;
	}
	return NULL;
}

// calculate absolute transform for node to do mesh skinning
void transformnode(aiMatrix4x4 *result, aiNode *node)
{
	if (node->mParent) {
		transformnode(result, node->mParent);
		aiMultiplyMatrix4(result, &node->mTransformation);
	} else {
		*result = node->mTransformation;
	}
}

void transformmesh(aiScene *scene, glMesh *mesh)
{
	aiMesh *amesh = mesh->mesh;
	aiMatrix4x4 skin4;
	aiMatrix3x3 skin3;
	int i, k;

	if (amesh->mNumBones == 0)
		return;

	memset(mesh->position, 0, mesh->vertexcount * 3 * sizeof(float));
	memset(mesh->normal, 0, mesh->vertexcount * 3 * sizeof(float));

	for (k = 0; k < amesh->mNumBones; k++) {
		aiBone *bone = amesh->mBones[k];
		aiNode *node = findnode(scene->mRootNode, bone->mName.data);

		transformnode(&skin4, node);
		aiMultiplyMatrix4(&skin4, &bone->mOffsetMatrix);
		extract3x3(&skin3, &skin4);

		for (i = 0; i < bone->mNumWeights; i++) {
			int v = bone->mWeights[i].mVertexId;
			float w = bone->mWeights[i].mWeight;

			aiVector3D position = amesh->mVertices[v];
			aiTransformVecByMatrix4(&position, &skin4);
			mesh->position[v*3+0] += position.x * w;
			mesh->position[v*3+1] += position.y * w;
			mesh->position[v*3+2] += position.z * w;

			aiVector3D normal = amesh->mNormals[v];
			aiTransformVecByMatrix3(&normal, &skin3);
			mesh->normal[v*3+0] += normal.x * w;
			mesh->normal[v*3+1] += normal.y * w;
			mesh->normal[v*3+2] += normal.z * w;
		}
	}
}

void initmesh(aiScene *scene, glMesh *mesh, aiMesh *amesh)
{
	int i;

	vertexcount += amesh->mNumVertices;
	facecount += amesh->mNumFaces;

	mesh->mesh = amesh; // stow away pointer for bones

	mesh->texture = loadmaterial(scene->mMaterials[amesh->mMaterialIndex]);

	mesh->vertexcount = amesh->mNumVertices;
	mesh->position = (float *)calloc(mesh->vertexcount * 3, sizeof(float));
	mesh->normal = (float *)calloc(mesh->vertexcount * 3, sizeof(float));
	mesh->texcoord = (float *)calloc(mesh->vertexcount * 2, sizeof(float));
	mesh->color = (unsigned char *)calloc(mesh->vertexcount * 4, sizeof(unsigned char));

	for (i = 0; i < mesh->vertexcount; i++) {
		mesh->position[i*3+0] = amesh->mVertices[i].x;
		mesh->position[i*3+1] = amesh->mVertices[i].y;
		mesh->position[i*3+2] = amesh->mVertices[i].z;

		if (amesh->mNormals) {
			mesh->normal[i*3+0] = amesh->mNormals[i].x;
			mesh->normal[i*3+1] = amesh->mNormals[i].y;
			mesh->normal[i*3+2] = amesh->mNormals[i].z;
		}

		if (amesh->mTextureCoords[0]) {
			mesh->texcoord[i*2+0] = amesh->mTextureCoords[0][i].x;
			mesh->texcoord[i*2+1] = 1 - amesh->mTextureCoords[0][i].y;
		}

		if (amesh->mColors[0]) {
			mesh->color[i*4+0] = (unsigned char)(amesh->mColors[0][i].r * 255);
			mesh->color[i*4+1] = (unsigned char)(amesh->mColors[0][i].g * 255);
			mesh->color[i*4+2] = (unsigned char)(amesh->mColors[0][i].b * 255);
			mesh->color[i*4+3] = (unsigned char)(amesh->mColors[0][i].a * 255);
		} else {
			mesh->color[i*4+0] = 255;
			mesh->color[i*4+1] = 255;
			mesh->color[i*4+2] = 255;
			mesh->color[i*4+3] = 255;
		}
	}

	mesh->elementcount = amesh->mNumFaces * 3;
	mesh->element = (int *)calloc(mesh->elementcount, sizeof(int));

	for (i = 0; i < amesh->mNumFaces; i++) {
		aiFace *face = amesh->mFaces + i;
		mesh->element[i*3+0] = face->mIndices[0];
		mesh->element[i*3+1] = face->mIndices[1];
		mesh->element[i*3+2] = face->mIndices[2];
	}
}

void initscene(aiScene *scene)
{
	int i;
	meshcount = scene->mNumMeshes;
	meshlist = (glMesh *)calloc(meshcount, sizeof *meshlist);
	for (i = 0; i < meshcount; i++) {
		initmesh(scene, meshlist + i, scene->mMeshes[i]);
		transformmesh(scene, meshlist + i);
	}
}

void drawmesh(glMesh *mesh)
{
	if (mesh->texture > 0) {
		glColor4f(1, 1, 1, 1);
		glBindTexture(GL_TEXTURE_2D, mesh->texture);
	} else {
		glColor4f(0.9, 0.7, 0.7, 1);
		glBindTexture(GL_TEXTURE_2D, mesh->color ? textureless_texture : checker_texture);
	}
	glVertexPointer(3, GL_FLOAT, 0, mesh->position);
	glNormalPointer(GL_FLOAT, 0, mesh->normal);
	glTexCoordPointer(2, GL_FLOAT, 0, mesh->texcoord);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, mesh->color);
	glDrawElements(GL_TRIANGLES, mesh->elementcount, GL_UNSIGNED_INT, mesh->element);
}

void drawnode(aiNode *node, aiMatrix4x4 world)
{
	float mat[16];
	int i;

	//debugstring(node->mName.data);

	aiMultiplyMatrix4(&world, &node->mTransformation);
	transposematrix(mat, &world);

	for (i = 0; i < node->mNumMeshes; i++) {
		glMesh *mesh = meshlist + node->mMeshes[i];
		if (mesh->mesh->mNumBones == 0) {
			// non-skinned meshes are in node-local space
			glPushMatrix();
			glMultMatrixf(mat);
			drawmesh(mesh);
			glPopMatrix();
		} else {
			// skinned meshes are already in world space
			drawmesh(mesh);
		}
	}

	for (i = 0; i < node->mNumChildren; i++)
		drawnode(node->mChildren[i], world);
}

void drawscene(aiScene *scene)
{
	aiMatrix4x4 world;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	aiIdentityMatrix4(&world);
	drawnode(scene->mRootNode, world);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void measuremesh(glMesh *mesh, aiMatrix4x4 transform, float bboxmin[3], float bboxmax[3])
{
	aiVector3D p;
	int i;
	for (i = 0; i < mesh->vertexcount; i++) {
		p.x = mesh->position[i*3+0];
		p.y = mesh->position[i*3+1];
		p.z = mesh->position[i*3+2];
		aiTransformVecByMatrix4(&p, &transform);
		bboxmin[0] = MIN(bboxmin[0], p.x);
		bboxmin[1] = MIN(bboxmin[1], p.y);
		bboxmin[2] = MIN(bboxmin[2], p.z);
		bboxmax[0] = MAX(bboxmax[0], p.x);
		bboxmax[1] = MAX(bboxmax[1], p.y);
		bboxmax[2] = MAX(bboxmax[2], p.z);
	}
}

void measurenode(aiNode *node, aiMatrix4x4 world, float bboxmin[3], float bboxmax[3])
{
	aiMatrix4x4 identity;
	int i;

	aiMultiplyMatrix4(&world, &node->mTransformation);
	aiIdentityMatrix4(&identity);

	for (i = 0; i < node->mNumMeshes; i++) {
		glMesh *mesh = meshlist + node->mMeshes[i];
		if (mesh->mesh->mNumBones == 0) {
			// non-skinned meshes are in node-local space
			measuremesh(mesh, world, bboxmin, bboxmax);
		} else {
			// skinned meshes are already in world space
			measuremesh(mesh, identity, bboxmin, bboxmax);
		}
	}

	for (i = 0; i < node->mNumChildren; i++)
		measurenode(node->mChildren[i], world, bboxmin, bboxmax);
}

float measurescene(aiScene *scene, float center[3])
{
	aiMatrix4x4 world;
	float bboxmin[3];
	float bboxmax[3];
	float dx, dy, dz;

	bboxmin[0] = 1e10; bboxmax[0] = -1e10;
	bboxmin[1] = 1e10; bboxmax[1] = -1e10;
	bboxmin[2] = 1e10; bboxmax[2] = -1e10;

	aiIdentityMatrix4(&world);
	measurenode(scene->mRootNode, world, bboxmin, bboxmax);

	center[0] = (bboxmin[0] + bboxmax[0]) / 2;
	center[1] = (bboxmin[1] + bboxmax[1]) / 2;
	center[2] = (bboxmin[2] + bboxmax[2]) / 2;

	dx = MAX(center[0] - bboxmin[0], bboxmax[0] - center[0]);
	dy = MAX(center[1] - bboxmin[1], bboxmax[1] - center[1]);
	dz = MAX(center[2] - bboxmin[2], bboxmax[2] - center[2]);

	return sqrt(dx*dx + dy*dy + dz*dz);
}

int animationlength(aiAnimation *anim)
{
	int i, len = 0;
	for (i = 0; i < anim->mNumChannels; i++) {
		aiNodeAnim *chan = anim->mChannels[i];
		len = MAX(len, chan->mNumPositionKeys);
		len = MAX(len, chan->mNumRotationKeys);
		len = MAX(len, chan->mNumScalingKeys);
	}
	return len;
}

blender mix( const blender &a, const blender &b, float dt ) {
	blender t;
	t.node = a.node;
	mixvector(&t.p, &a.p, &b.p, dt);
	mixquaternion(&t.r, &a.r, &b.r, dt);
	mixvector(&t.s, &a.s, &b.s, dt);
	return t;
}

blenders mix( const blenders &A, const blenders &B, float dt ) {
	blenders c;
	size_t num = A.size();
	c.resize( num );
	for( auto i = 0; i < num; ++i ) {
		c[ i ] = mix( A[i], B[i], dt );
	}
	return c;
}

blenders blendscene(aiScene *scene, aiAnimation *anim, int frameA, int frameB, float t)
{
	aiVectorKey *p0, *p1, *s0, *s1;
	aiQuatKey *r0, *r1;
	aiVector3D p, s;
	aiQuaternion r;
	int i;

	blenders bl(anim->mNumChannels);

	for (i = 0; i < anim->mNumChannels; i++) {
		aiNodeAnim *chan = anim->mChannels[i];
		aiNode *node = findnode(scene->mRootNode, chan->mNodeName.data);
		p0 = chan->mPositionKeys + (frameA) % chan->mNumPositionKeys;
		p1 = chan->mPositionKeys + (frameB) % chan->mNumPositionKeys;
		r0 = chan->mRotationKeys + (frameA) % chan->mNumRotationKeys;
		r1 = chan->mRotationKeys + (frameB) % chan->mNumRotationKeys;
		s0 = chan->mScalingKeys + (frameA) % chan->mNumScalingKeys;
		s1 = chan->mScalingKeys + (frameB) % chan->mNumScalingKeys;
		mixvector(&p, &p0->mValue, &p1->mValue, t);
		mixquaternion(&r, &r0->mValue, &r1->mValue, t);
		mixvector(&s, &s0->mValue, &s1->mValue, t);
		bl[ i ] = {node, p, r, s};
	}

	return bl;
}

float blendtick = 0;
  int blendpass = 0;
extern unsigned quantizers_pos;
extern unsigned quantizers_sca;
extern unsigned quantizers_rot;

int get_game_time();

#include "quant.hpp"

void animatescene(aiScene *scene, aiAnimation *anim, float tick)
{
	// Assumes even key frame rate and synchronized pos/rot/scale keys.
	// We should look at the key->mTime values instead, but I'm lazy.

	int frameA = floor(tick);
	float t = tick - frameA;

	int frameF = floor(blendtick);
	int i;

	auto blenders = blendscene(scene, anim, frameA, frameA + 1, t);

	switch( blendpass ) {
		default: {
		}
		break; case 2: {
			auto blenders2 = blendscene(scene, anim, frameF, frameF + 1, t);
			blenders = mix( blenders, blenders2, 0.75 );
		}
		break; case 1: {
			auto blenders2 = blendscene(scene, anim, frameF, frameF + 1, t);
			blenders = mix( blenders, blenders2, 0.50 );
		}
		break; case 0: {
			auto blenders2 = blendscene(scene, anim, frameF, frameF + 1, t);
			blenders = mix( blenders, blenders2, 0.25 );
		}
	}

	for( auto &bl : blenders ) {
		if( quantizers_pos ) {
			if( quantizers_pos == 1 ) {
				uint64_t n;
				quant::encode161616_vec( n, bl.p.x, bl.p.y, bl.p.z );
				quant::decode161616_vec( bl.p.x, bl.p.y, bl.p.z, n );
			}	
			if( quantizers_pos == 2 ) {
				uint32_t x;
				quant::encode_vec<11,11,8>( x, bl.p );
				quant::decode_vec<11,11,8>( bl.p, x );				
			}
			if( quantizers_pos == 3 ) {
				uint32_t x;
				quant::encode_vec<10,10,10>( x, bl.p );
				quant::decode_vec<10,10,10>( bl.p, x );				
			}
			if( quantizers_pos == 4 ) {
				uint32_t x;
				quant::encode_vec<9,9,12>( x, bl.p );
				quant::decode_vec<9,9,12>( bl.p, x );				
			}
			if( quantizers_pos == 5 ) {
				uint32_t x;
				quant::encode_vec<8,8,14>( x, bl.p );
				quant::decode_vec<8,8,14>( bl.p, x );				
			}
			if( quantizers_pos == 6 ) {
				uint32_t x;
				quant::encode_vec<7,7,16>( x, bl.p );
				quant::decode_vec<7,7,16>( bl.p, x );				
			}
		}
		if( quantizers_rot ) {
			uint32_t x;
			quant::encode101010_quat( x, bl.r );
			quant::decode101010_quat( bl.r, x );
		}
		if( quantizers_sca == 1 ) {
			uint32_t x;
			bl.s.x = 1 - bl.s.x;
			bl.s.y = 1 - bl.s.y;
			bl.s.z = 1 - bl.s.z;
			quant::encode8814_vec( x, bl.s );
			quant::decode8814_vec( bl.s, x );
			bl.s.x = 1 - bl.s.x;
			bl.s.y = 1 - bl.s.y;
			bl.s.z = 1 - bl.s.z;
		}
		if( quantizers_sca == 2 ) {
			uint16_t x;
			bl.s.x = 1 - bl.s.x;
			bl.s.y = 1 - bl.s.y;
			bl.s.z = 1 - bl.s.z;
			quant::encode555_vec( x, bl.s );
			quant::decode555_vec( bl.s, x );
			bl.s.x = 1 - bl.s.x;
			bl.s.y = 1 - bl.s.y;
			bl.s.z = 1 - bl.s.z;
		}
		composematrix(&bl.node->mTransformation, &bl.p, &bl.r, &bl.s);
	}

	for (i = 0; i < meshcount; i++) {
		transformmesh(scene, meshlist + i);		
	}
}


void printscene(const aiScene* scene ){
 printf("void printAiSceneInfo(const aiScene* scene )...\n");

 //if the scene contains meshes...
 //...for every mesh in the scene...
 //print if the scene has meshes
 printf("scene->HasMeshes(): %i\n", scene->HasMeshes());
 if(scene->HasMeshes()){
  //print the number of meshes
  printf("aiScene->mNumMeshes: %i\n", scene->mNumMeshes);
  //for all the meshes...
  for(int i(0);i<scene->mNumMeshes;i++){
   //print which mesh we are referring to
   printf("aiScene->mMeshes[%i]: \n", i);
   //for the vertex colors:
   //...for 0 to AI_MAX_NUMBER_OF_COLOR_SETS (for every colour set)...
   for(int j(0);j<AI_MAX_NUMBER_OF_COLOR_SETS;j++){
    //print if the scene has vertex colours for that colour set
    printf("aiScene->mMeshes[%i]->HasVertexColors[%i]: %i\n", i, j, scene->mMeshes[i]->HasVertexColors(j));
    //if mesh has vertex colours for specifc colour set...
    if(scene->mMeshes[i]->HasVertexColors(j)){
     //...for all the colours in that set
     for(int k(0);k<scene->mMeshes[i]->mNumVertices;k++){
      printf("aiScene->mMeshes[%i]->mColors[%i][%i]: [%f, %f, %f, %f]\n",
              i,
              j,
              k,
              scene->mMeshes[i]->mColors[j][k].r,
              scene->mMeshes[i]->mColors[j][k].g,
              scene->mMeshes[i]->mColors[j][k].b,
              scene->mMeshes[i]->mColors[j][k].a);
     }
    }else{
     //...else print out that there is not colours in that set
     printf("aiScene->mMeshes[%i]->mColors[%i]: None\n", i, j);
    }
   }
   //for the faces:
   //print if the mesh has faces
   printf("aiScene->mMeshes[%i]->HasFaces(): %i\n", i, scene->mMeshes[i]->HasFaces());
   //if the mesh has faces...
   if(scene->mMeshes[i]->HasFaces()){
    //print number of faces
    printf("aiScene->mMeshes[%i]->mNumFaces: %i\n", i, scene->mMeshes[i]->mNumFaces);
    //for every face...
    if(0)
    for(int j(0);j<scene->mMeshes[i]->mNumFaces;j++){
     //print the face index info
     printf("scene->mMeshes[%i]->mFaces[%i].mNumIndices: %i\n", i, j, scene->mMeshes[i]->mFaces[j].mNumIndices);
     //for every face index...
     for(int k(0);k<scene->mMeshes[i]->mFaces[j].mNumIndices;k++){
      //print the indices of the face
      printf("aiScene->mMeshes[%i]->mFaces[%i].mIndices[%i]: %i\n",
             i,
             j,
             k,
             scene->mMeshes[i]->mFaces[j].mIndices[k]);
     }
    }
   }
  }
 }
}
