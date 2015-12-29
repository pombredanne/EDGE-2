#include <glm/glm.hpp>

namespace viewport
{
   // split screen interface

	glm::vec4 window[4]; // four regions glm::vec4(x-left,y-bottom,pix-width,pix-height) each

	void update( size_t w, size_t h, size_t x, size_t y )
	{
		window[0] = glm::vec4( 0, y,     x, h-1-y );
		window[1] = glm::vec4( x, y, w-1-x, h-1-y );
		window[2] = glm::vec4( 0, 0,     x,     y );
		window[3] = glm::vec4( x, 0, w-1-x,     y );
	}

	bool pass( int i )
	{
		return( i >= 0 && i < 4 && window[i].z > 0 && window[i].w > 0 );
	}

	glm::vec4 get( int i )
	{
		return pass(i) ? window[i] : glm::vec4(0,0,0,1);
	}

   // classic interface

   void update( size_t w, size_t h )
   {
      window[0] = glm::vec4( 0, 0, w, h );
      window[1] = glm::vec4( 0, 0, 0, 1 );
      window[2] = glm::vec4( 0, 0, 0, 1 );
      window[3] = glm::vec4( 0, 0, 0, 1 );
   }

   bool pass()
   {
      return pass( 0 );
   }

   glm::vec4 get()
   {
      return get( 0 );
   }
}

