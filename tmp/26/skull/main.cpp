#include <GL/glew.h>
#include "model.hpp"

#include <eve/eve.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include <functional>
#include <sstream>

int modelAnimateSubmeshId;
unsigned modelMaxSpineChanges;
std::string modelPath;
std::string modelFile;
std::string modelHeadBone;
float modelScale;
glm::vec3 modelQuatX, modelQuatY, modelQuatZ;
std::stringstream console;
bool options[12 + 1] = {0 /*<-unused*/,1,1,1,0,0,0,0,0,0,0,0,0}, options_pvt[12 + 1] = {};
enum options {
    QUANT_POS = 1,
    QUANT_ROT = 2,
    QUANT_SCA = 3,    
    HEAD_IK = 12,
};

aiQuaternion to_assimp( float x, float y, float z = 0 ) {
    glm::vec3 vec = ( modelQuatX * x + modelQuatY * y + modelQuatZ * z );
    aiMatrix3x3 newRotation;
    aiMatrix3x3::Rotation(1, aiVector3D(vec.x,vec.y,vec.z), newRotation);
    return aiQuaternion( newRotation );
}

void reselect( int selector, const char *model = 0 ) {
    switch(selector) {
        default:
        case 0:
        modelAnimateSubmeshId = 1;
        modelPath = "models/";
        modelFile = model ? model : "astroBoy_walk_Maya.dae";
        modelHeadBone = "head";
        modelScale = 1.f;
        modelQuatX = glm::vec3( 0,  0, +1);
        modelQuatY = glm::vec3( 0, -1,  0);
        modelQuatZ = glm::vec3(-1,  0,  0);
        modelMaxSpineChanges = 4;
        break;
        case 1:
        modelAnimateSubmeshId = 0;
        modelPath = "models/";
        modelFile = model ? model : "demon_boss_animations_allinone.fbx";
        modelHeadBone = "Character1_Head";
        modelScale = 0.1f;
        modelQuatX = glm::vec3(-1,  0,  0);
        modelQuatY = glm::vec3( 0, -1, +1);
        modelQuatZ = glm::vec3( 0, -1,  0);
        modelMaxSpineChanges = ~0;
        break;
        case 2:
        modelFile = model ? model : "ole_ole_y_ole.fbx"; //animacion.fbx";
        modelAnimateSubmeshId = 0;
        modelPath = "models/";
        modelHeadBone = "Bip01 Head";
        modelScale = 10.f;
        modelQuatX = glm::vec3( 0,  0, +1);
        modelQuatY = glm::vec3( 0, -1,  0);
        modelQuatZ = glm::vec3(-1,  0,  0);
        modelMaxSpineChanges = 0;
        break;
    }
}


/* quantization */
#include "quant.hpp"
using namespace quant;
namespace {

    std::string diff( const aiVector3D &A, const aiVector3D &B ) {
        std::stringstream ss;
        ss << "(diff: (" << A.x << ',' << A.y << ',' << A.z << ") -> B(" << B.x << ',' << B.y << ',' << B.z << "))";
        return ss.str();
    }

    // raw
    struct transformR {
        aiVector3D pos;
        aiQuaternion rot;
        aiVector3D sca;
    };
    // quantized
    struct transformQ {
        uint32_t pos;
        uint32_t rot;
        uint32_t sca;
    };

    transformQ quant( const transformR &t ) {
        transformQ qt;
        encode8814_vec( qt.pos, t.pos );
        encode8814_vec( qt.sca, t.sca );
        encode101010_quat( qt.rot, t.rot );
        return qt;
    }
    transformR dequant( const transformQ &qt ) {
        transformR t;
        decode8814_vec( t.pos, qt.pos );
        decode8814_vec( t.sca, qt.sca );
        decode101010_quat( t.rot, qt.rot );
        return t;
    }
}


//Class that defines how we are to handle materials, and especially textures, in the 3D model.
//Only diffuse textures stored in external files are handled in this example
class AppMaterial : Material {
public:
    std::vector<eve::texture> diffuseTextures;

    AppMaterial(const aiMaterial* material) {
        for(unsigned int cdt=0;cdt<material->GetTextureCount(aiTextureType_DIFFUSE);cdt++) {
            aiString path;
            material->GetTexture(aiTextureType_DIFFUSE, cdt, &path); //Assumes external texture files

            eve::string txname = path.C_Str();
            txname = wire::strings( txname.tokenize("\\/") )[-1];
            txname = modelPath + '/' + txname;

            eve::texture tx;
            if( !tx.load( txname ) ) {
                std::cout << txname << " failed" << std::endl;
                if( !tx.load( txname+=".webp" ) ) {
                    std::cout << txname << " failed" << std::endl;
                    if( !tx.load( txname+=".jpg" ) ) {
                        std::cout << txname << " failed" << std::endl;
                    }
                }
            }
            if( tx.loaded() ) {
                diffuseTextures.push_back( tx.flip_h() );
                diffuseTextures.back().upload();                
            }
        }
    }
    
    void bindTexture(aiTextureType textureType, unsigned int textureId) const {
        diffuseTextures[textureId].bind();
    }
    
    bool texture() const {
        return diffuseTextures.size()>0;
    }
};

//Example 1 - draws an unanimated model
class modelStatic {
public:
    Model<AppMaterial> model;
    
    modelStatic() 
    {}

    void init() {
        model.read(modelPath+modelFile);
    }
    
    void draw() {
        model.draw();
    }
};

//Example 2 - draws a skeletal animated model using SkeletalAnimationModel::drawFrame, which consists of:
//SkeletalAnimationModel::createFrame makes the animation frame given animationId and time, 
//SkeletalAnimationModel::getMeshFrame receives the frame vertices and normals for the given mesh, and
//SkeletalAnimationModel::drawMeshFrame draws the given mesh frame. Example 3 and 4 show why we have these 3 functions.
class modelAnimated {
public:
    SkeletalAnimationModel<AppMaterial> model; //template argument specifies how we are to handle textures
    
    modelAnimated() {
    }

    void init() {
        model.read(modelPath+modelFile);
    }
    
    //Draw the animation frame given time in seconds
    void drawFrame(double time) {
        model.drawFrame(0, time); //this function equals the following lines:
        //model.createFrame(0, time); //first parameter selects which animation to use
        //for(auto& mesh: model.meshes) {
        //    auto meshFrame=model.getMeshFrame(mesh);
        //    model.drawMeshFrame(meshFrame);
        //}
    }
};

//Example 3 - moves a mesh after the mesh of an animation frame is created.
//Modification happens here between SkeletalAnimationModel::getMeshFrame and SkeletalAnimationModel::drawMeshFrame
class modelAnimatedSubmesh {
public:
    SkeletalAnimationModel<AppMaterial> model;
    
    modelAnimatedSubmesh() 
    {}

    void init( const SkeletalAnimationModel<AppMaterial>& model) {
        this->model = model;
    }
    
    //Draw the animation frame given time in seconds
    void drawFrame(double time) {
        model.createFrame(0, time);
        for(auto end = model.meshes.size(), submesh = end - end; submesh < end; submesh++ ) {
            auto meshFrame = model.getMeshFrame(model.meshes[submesh]);
            if( submesh == modelAnimateSubmeshId ) { 
                for( auto &vertex: meshFrame.vertices ) {
                    vertex.z+=4.0*(cos(time*10.0)+1.0);                    
                }
            }
            model.drawMeshFrame(meshFrame);
        }
    }
};

//Example 4 - changing animation through direct manipulation of a bone transformation matrix
//Modification happens here between SkeletalAnimationModel::createFrame and SkeletalAnimationModel::getMeshFrame
class modelAnimatedHeadBanging {
public:
    SkeletalAnimationModel<AppMaterial> model;
    
    modelAnimatedHeadBanging() {}

    void init( const SkeletalAnimationModel<AppMaterial>& model) {
        this->model = model;
    }
    
    //Draw the animation frame given time in seconds
    void drawFrame(double time, float mx, float my) {
        bool loopable = true;
        unsigned int animationId = 0;
        model.createFrame(animationId, time, loopable);

        unsigned int boneId = model.boneName2boneId.at(modelHeadBone), max_changes = modelMaxSpineChanges;
        while( mx != 0 && my != 0 ) {
            aiVector3D oldScale;
            aiQuaternion oldRotation;
            aiVector3D oldPosition;

            model.bones[boneId].transformation.Decompose(oldScale, oldRotation, oldPosition);

            // this is to simulate quantization magic
            // de/quantize scale (vec3), rotation (quat) and position (vec3)
            uint32_t Q;
            if( options[QUANT_SCA] ) {
                encode8814_vec( Q, oldScale );
                decode8814_vec( oldScale, Q );
            }
            if( options[QUANT_ROT] ) {
                encode101010_quat( Q, oldRotation );
                decode101010_quat( oldRotation, Q );
            }
            if( options[QUANT_POS] ) {
                encode8814_vec( Q, oldPosition );
                decode8814_vec( oldPosition, Q );
            }

            if( options[HEAD_IK] ) {
                ;
            } else {
                mx = my = 0;
            }
            aiQuaternion newRotation = to_assimp( my, mx, 0.f );
            model.bones[boneId].transformation=aiMatrix4x4Compose(oldScale, (newRotation*oldRotation).Normalize(), oldPosition);            

            if( !model.bones[boneId].hasParentBoneId ) {
                break;
            }

            if( !--max_changes ) break;

            // propagate modification to parents as well (where possible)
            boneId = model.bones[boneId].parentBoneId;
            my *= 0.5f;
            mx *= 0.5f;
        }
        
        for(auto& mesh: model.meshes) {
            auto meshFrame=model.getMeshFrame(mesh);
            model.drawMeshFrame(meshFrame);
        }
    }
};
    
//Example 5 - print bone hierarchy. Format: boneId boneName
template <class TextureHandler>
void printBoneHierarchy(const SkeletalAnimationModel<TextureHandler>& model) {
    //Find bone children from Bone::parentBoneId
    std::vector<std::vector<unsigned int> > boneChildrenIds(model.bones.size());
    for( auto end = model.bones.size(), bone = end - end; bone < end; bone++ ) {
        if( model.bones[bone].hasParentBoneId ) {
            boneChildrenIds[model.bones[bone].parentBoneId].push_back( bone );
        }
    }
    
    //map boneId to boneName
    std::vector<std::string> boneId2BoneName(model.bones.size());
    for( auto& p: model.boneName2boneId ) {
        boneId2BoneName[p.second]=p.first;
    }
    
    const std::function<void(unsigned int, unsigned int)> recursiveFunction=[&](unsigned int boneId, unsigned int level) {
        std::cout << std::string( level * 2, ' ' ) << boneId << " " << boneId2BoneName[boneId] << std::endl;
        for( unsigned int childBoneId : boneChildrenIds[boneId] ) {
            recursiveFunction(childBoneId, level+1);
        }
    };

    for( auto end = model.bones.size(), bone = end - end; bone < end; bone++ ) {
        if( !model.bones[bone].hasParentBoneId ) {
            recursiveFunction(bone, 0);
        }
    }
}

//Create window, handle events, and OpenGL draw-function
class Application {
public:
    Application()
    {}

    modelStatic modelStatic;
    modelAnimated modelAnimated;
    modelAnimatedSubmesh modelAnimatedSubmesh;
    modelAnimatedHeadBanging modelAnimatedHeadBanging;

    void setup(int w, int h) {
        glewExperimental = GL_TRUE;
        glewInit();

        //Various settings
        glViewport( 0, 0, w, h );
        glClearColor(40/255.f, 40/255.f, 40/255.f, 0.0f);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glClearDepth(1.0f);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glEnable(GL_TEXTURE_2D);
        
        //Lighting
        GLfloat light_color[] = {0.9, 0.9, 0.9, 1.f};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, light_color);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        //Loading models
        modelStatic.init();
        modelAnimated.init();
        printBoneHierarchy(modelAnimated.model);
        modelAnimatedSubmesh.init(modelAnimated.model);
        modelAnimatedHeadBanging.init(modelAnimated.model);
    }
    
    void draw( double time, unsigned pass_no, const glm::vec4 &viewport, double mx, double my ) {
        console.str("");
        console << "viewport #" << pass_no << std::endl;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLfloat ratio = float(viewport.z) / viewport.w;
        glFrustum(-ratio, ratio, -1.0, 1.0, 1.0, 500.0);

        //The rendering loop
        glViewport( viewport.x, viewport.y, viewport.z, viewport.w );

        //Setup projection matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(0.0f, -20.0f, -45.0f);

        glPushMatrix();
            glRotatef(-time*50.0+180.0, 0.0, 1.0, 0.0);
            glScalef( modelScale, modelScale, modelScale );
            auto duration = 400;
            auto fps = 30;
            auto bones = 70;
            if( pass_no == 0 ) {
                // auto bytes_per_matrix = sizeof(transformR); // <-- assimp's
                auto bytes_per_matrix = ( 4 * 4 ) * sizeof(float); // <-- our standard engine
                console << "original animation:" << std::endl;
                console << duration << " secs * " << fps << " fps * " << bones << " bones * " << bytes_per_matrix << " (bytes per matrix) = " 
                    << wire::format("%5.2f MiB", ((duration * fps * bones * bytes_per_matrix) / 1024.f / 1024.f )) << std::endl;
                modelAnimatedSubmesh.drawFrame(time);                
            }
            else {
                auto bytes_per_matrix = (
                    (options[1] ? sizeof(transformQ().pos) : sizeof(transformR().pos)) + // position
                    (options[2] ? sizeof(transformQ().rot) : sizeof(transformR().rot)) + // rotation
                    (options[3] ? sizeof(transformQ().sca) : sizeof(transformR().sca)) + // scale
                    0
                );
                console << "quantized animation:" << std::endl;
                console << duration << " secs * " << fps << " fps * " << bones << " bones * " << bytes_per_matrix << " (bytes per matrix) = " 
                    << wire::format("%5.2f MiB", ((duration * fps * bones * bytes_per_matrix) / 1024.f / 1024.f )) << std::endl;
                console << std::endl;
                // update keymap
                for( unsigned i = 1; i <= 12; ++i ) {
                    bool pressed = (GetAsyncKeyState(VK_F1 + i - 1) & 0x8000) > 0;
                    if( pressed && (options_pvt[i] ^ pressed) ) {
                        options[i] ^= true;
                    } 
                    options_pvt[i] = pressed;                    
                }
                //
                console << (options[ 1] ? "[x]" : "[ ]") << " - quantize position (f1)" << std::endl;
                console << (options[ 2] ? "[x]" : "[ ]") << " - quantize rotation (f2)" << std::endl;
                console << (options[ 3] ? "[x]" : "[ ]") << " - quantize scale (f3)" << std::endl;
                console << (options[ 4] ? "[x]" : "[ ]") << " - 15 Hz toggle (f4)" << std::endl;
                console << (options[ 5] ? "[x]" : "[ ]") << " - curve simplification toggle (f5)" << std::endl;
                console << (options[12] ? "[x]" : "[ ]") << " - head IK (f12)" << std::endl;
                modelAnimatedHeadBanging.drawFrame(time, mx, my);                
            }
        glPopMatrix();

        // HUD
        float x = 10, y = 10;
        glMatrixMode(GL_PROJECTION);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glColor4ub(255,192,0,255);
#if 1
//        eve::style::yellow text_color;
        eve::geometry::text2d( x, y, console.str() );
        {
        eve::matrix::ortho gl;            
        glColor4ub( 80, 80, 80, 255 );
        glBegin(GL_LINES);
            glVertex2i(0, 0);
            glVertex2i(0, viewport.w);
        glEnd();
        }
#else
        glLoadIdentity();
        glRasterPos2f(x+0.375, y+0.375);
        glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *)console.str().c_str());
#endif
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glColor4ub(255,255,255,255);
    }
};

//If Linux: must run XInitThreads() from X11/Xlib.h before starting threads in X11
#ifdef __linux
#include<X11/Xlib.h>
int main( int argc, const char **argv ) {
    XInitThreads();
#else
int main( int argc, const char **argv ) {
#endif

    {
//        auto x = quantp( aiVector3D)
//        return 0;
    }

    reselect(0);
            
    if( argc > 1 && argv[1][0] >= '0' && argv[1][0] <= '9' )
        reselect(argv[1][0] - '0');

    eve::window window( "skull animation lib - quantization test", 0.70f );
    eve::keyboard keyboard(0);
    eve::mouse mouse(0);

    {
        Application app;
        app.setup( window.w, window.h );
        
        //Store start time
        std::chrono::time_point<std::chrono::system_clock> startTime=std::chrono::system_clock::now();

        //Event thread (main thread)
        while (window.is_open()) {
            mouse.update();
            keyboard.update();
            if( keyboard.escape.trigger() ) {
                break;
            }

            if( !eve::lock(60) ) {
                continue;
            }

            std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - startTime;
            double time=elapsed_seconds.count();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            viewport::update( window.w, window.h, window.w / 2, 0 * window.h / 2 );

            for( unsigned pass = 0; pass < 4; ++pass ) {
                if( viewport::pass(pass) ) {
                    app.draw( time, pass, viewport::get(pass), mouse.local.x, mouse.local.y );
                }
            }

            //Swap buffer (show result)
            window.flush(1/60.f);
        }
    }
    
    return 0;
}
