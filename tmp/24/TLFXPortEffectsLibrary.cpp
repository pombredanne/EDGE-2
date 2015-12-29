#include "TLFXPortEffectsLibrary.h"
#include "TLFXPugiXMLLoader.h"

#include <cmath>

/* port { */

#include <eve/eve.hpp>
#include <cassert>
#include <cstdlib>
#include <memory>

class CIwTexture {
public:
    eve::texture tx;
};

struct R {

    // position, texture and color
    struct Vertex { vec2 pos; vec2 uv; vec4 rgba; };

    // indexes
    IBO<GLuint> ibo;
    // vertices
    VBO<Vertex> vbo;
    // array of arrays
    VAO vao;

    // shader
    Shader shader;

    R() {
        shader.vertexShader(glsl3(
            /* combined projection and view matrix of our SpriteBatch */
            uniform mat4 u_projView;

            /* "in" attributes from our SpriteBatch */
            in vec2 Position;
            in vec2 TexCoord;
            in vec4 Color;

            /* "out" varyings to our fragment shader */
            out vec4 vColor;
            out vec2 vTexCoord;

            void main() {
                vColor = Color;
                vTexCoord = TexCoord;
                gl_Position = u_projView * vec4(Position, 0.0, 1.0);

                const float right = 800.0;
                const float bottom = 600.0;
                const float left = 0.0;
                const float top = 0.0;
                const float far = 1.0;
                const float near = -1.0;
                /* This uses the standard OpenGL glOrtho transform. */
                /* The origin will be at the bottom-left, with +X going right and +Y going up. That's standard for OpenGL. */
                mat4 testmat = mat4(
                    vec4(2.0 / (right - left), 0, 0, 0),
                    vec4(0, 2.0 / (top - bottom), 0, 0),
                    vec4(0, 0, -2.0 / (far - near), 0),
                    vec4(-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1)
                );
                gl_Position = testmat * vec4(Position, 0.0, 1.0);
                /*
                If you want a top-left origin, with +Y going down, you'll have to adjust the matrix accordingly.
                Translate(-1.0f, 1.0f, (depthRange.x + depthRange.y) / 2.0f);
                Scale(2.0f / size.x, -2.0f / size.y, 1.0f);
                Where depthRange.x/y are the zNear/zFar values. size.x/y is the width/height of the window,
                Translate creates a translation matrix, and Scale creates a scale matrix (they are right-multiplied with each other).
                */
            }        
        )).fragmentShader(glsl3(
            /* SpriteBatch will use texture unit 0 */
            uniform sampler2D u_texture;

            /* "in" varyings from our vertex shader */
            in vec4 vColor;
            in vec2 vTexCoord;

            /* out */
            out vec4 FinalColor;

            void main() {
                /* sample the texture */
                vec4 texColor = texture2D(u_texture, vTexCoord);

                /* invert the red, green and blue channels */
                //texColor.rgb = 1.0 - texColor.rgb;

                /* final color */
                FinalColor = vColor * texColor;
            }
        )).link();

        ibo.clear();
        ibo 
            << 0 << 2 << 3 
            << 3 << 1 << 0;  // first quad (2 triangles)
        ibo.upload(GL_ELEMENT_ARRAY_BUFFER);

        vbo.clear();
                    //  XY   UV   RGBA
        vbo << Vertex { vec2(0,0), vec2(0,0), vec4(0,0,0,0) }  // Vertex 0
            << Vertex { vec2(0,0), vec2(0,0), vec4(0,0,0,0) }  // Vertex 0
            << Vertex { vec2(0,0), vec2(0,0), vec4(0,0,0,0) }  // Vertex 0
            << Vertex { vec2(0,0), vec2(0,0), vec4(0,0,0,0) }; // Vertex 0
        vbo.upload(GL_ARRAY_BUFFER); 

        vao.create(shader, vbo, ibo)
            .attribute<float>("Position", 2)
            .attribute<float>("TexCoord", 2)
            .attribute<float>("Color", 4)
            .check();
    }
};

/* } */


TLFX::XMLLoader* TLFXPortEffectsLibrary::CreateLoader() const {
    return new TLFX::PugiXMLLoader();
}

TLFX::AnimImage* TLFXPortEffectsLibrary::CreateImage() const {
    return new TLFXPortImage();
}

bool TLFXPortImage::Load( const char *filename )
{
    std::cout << "[    ] Loading " << filename << "... ";
    _texture = new CIwTexture();
    if( _texture->tx.load(filename) ) {
        _texture->tx.upload();
        std::cout << "\r[ OK\n";
        return true;
    }
    std::cout << "\r[FAIL\n";
    return false;
}

TLFXPortImage::TLFXPortImage()
    : _texture(NULL)
{

}

TLFXPortImage::~TLFXPortImage()
{
    if (_texture)
        delete _texture;
}

CIwTexture* TLFXPortImage::GetTexture() const
{
    return _texture;
}

TLFXPortParticleManager::TLFXPortParticleManager( int particles /*= particleLimit*/, int layers /*= 1*/ )
    : TLFX::ParticleManager(particles, layers)
    , _lastSprite(NULL)
    , _lastAdditive(true)
{

}

void TLFXPortParticleManager::DrawSprite( TLFX::AnimImage* sprite, float px, float py, float frame, float x, float y, float rotation, float scaleX, float scaleY, unsigned char r, unsigned char g, unsigned char b, float a , bool additive )
{
    //assert(frame == 0);

    unsigned char alpha = (unsigned char)(a * 255);
    if (alpha == 0 || scaleX == 0 || scaleY == 0) return;

    if (sprite != _lastSprite || additive != _lastAdditive)
        Flush();

    Batch batch;
    batch.px = px;
    batch.py = py;
    batch.frame = frame;
    batch.x = x;
    batch.y = y;
    batch.rotation = rotation;
    batch.scaleX = scaleX;
    batch.scaleY = scaleY;
    batch.color.r = r;
    batch.color.g = g;
    batch.color.b = b;
    batch.color.a = alpha;
    _batch.push_back(batch);

    _lastSprite = sprite;
    _lastAdditive = additive;
}

void TLFXPortParticleManager::Flush()
{
    if (!_batch.empty() && _lastSprite)
    {
        static R renderer;

        auto &vbo = renderer.vbo;
        auto &ibo = renderer.ibo;
        auto &vao = renderer.vao;
        auto &shader = renderer.shader;

        vbo.clear();
        ibo.clear();

        int index = 0;
        for (auto it = _batch.begin(); it != _batch.end(); ++it )
        {
            float x0 = -it->x * it->scaleX;
            float y0 = -it->y * it->scaleY;
            float x1 = x0;
            float y1 = (-it->y + _lastSprite->GetHeight()) * it->scaleY;
            float x2 = (-it->x + _lastSprite->GetWidth()) * it->scaleX;
            float y2 = y1;
            float x3 = x2;
            float y3 = y0;

            float cos = cosf(it->rotation / 180.f * (float)M_PI);
            float sin = sinf(it->rotation / 180.f * (float)M_PI);

            vec2 v0( it->px + x0 * cos - y0 * sin, it->py + x0 * sin + y0 * cos );
            vec2 v1( it->px + x1 * cos - y1 * sin, it->py + x1 * sin + y1 * cos );
            vec2 v2( it->px + x2 * cos - y2 * sin, it->py + x2 * sin + y2 * cos );
            vec2 v3( it->px + x3 * cos - y3 * sin, it->py + x3 * sin + y3 * cos );

            vec2 uv0( 0.0f, 0.0f ); // Vertex 0 (A)
            vec2 uv1( 1.0f, 0.0f ); // Vertex 1 (B)
            vec2 uv2( 1.0f, 1.0f ); // Vertex 2 (C)
            vec2 uv3( 0.0f, 1.0f ); // Vertex 3 (D)

            vec4 rgba(it->color.r / 255.0, it->color.g / 255.0, it->color.b / 255.0, it->color.a / 255.0);
                   
            auto A = (index+0), B = (index+1), C = (index+2), D = (index+3); index += 4;

            //      A--B                  A              A-B
            //      |  |                  |\              \|
            // quad D--C becomes triangle D-C and triangle C

            vbo << R::Vertex { v0, uv0, rgba }  // Vertex 0 (A)
                << R::Vertex { v1, uv1, rgba }  // Vertex 1 (B)
                << R::Vertex { v2, uv2, rgba }  // Vertex 2 (C)
                << R::Vertex { v3, uv3, rgba }; // Vertex 3 (D)

            ibo << C << D << A                  // Triangle 1
                << C << A << B;                 // Triangle 2
        }

        ibo.upload(GL_ELEMENT_ARRAY_BUFFER);
        vbo.upload(GL_ARRAY_BUFFER); 

        // get the time in seconds
        float t = glfwGetTime();

        // update camera and set the uniform
        ::camera camera;
        float sin_t = glm::radians(90.0f)*sin(t/10);
        camera.position = glm::vec3(0.0f, 0.0f, -1.0f);
        camera.rotation = glm::vec3( sin_t, 0, sin_t );
        camera.update();        

        ::transform tf;
        tf.position = glm::vec3(0,0,0);
        tf.update();

        glm::mat4 mvp = camera.ModelViewProjection*tf.ModelView;

        // setup gfx
        glDisable( GL_DEPTH_TEST );
        glEnable( GL_BLEND );
        if( _lastAdditive ) {
            // additive but fading out according to source alpha...
            glBlendFunc( GL_SRC_ALPHA, GL_ONE ); 
        } else {
            // alpha (translucency)
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;
        }

        // bind the texture @ unit 0
        eve::texture &tx = static_cast<TLFXPortImage*>(_lastSprite)->GetTexture()->tx;
        tx.bind(0);

        // use the shader
        renderer.shader.use();

        // set (0) in the uniform texture sampler
        int utex = glGetUniformLocation(renderer.shader.id, "u_texture");
        glUniform1i(utex, 0);

        // set mvp in the uniform
        int umvp = glGetUniformLocation(renderer.shader.id, "u_projView");
        glUniformMatrix4fv(umvp, 1, GL_FALSE, glm::value_ptr(mvp));

        // draw the vao
        vao.bind();
        vao.draw(GL_TRIANGLES);    

        _batch.clear();
    }
}
