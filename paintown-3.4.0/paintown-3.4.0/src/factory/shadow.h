#ifndef _paintown_shadow_h
#define _paintown_shadow_h

#include <vector>

class Bitmap;

/* nothing special, just the shadow that is shown under a character 
 * Shadow is a singleton factory class that returns the proper
 * shadow for a given Y coordinate( height )
 */
class Shadow{
public:
	static Bitmap const * getShadow( unsigned int y );
	static void destroy();

protected:
private:
	Shadow();
	~Shadow();

        std::vector< Bitmap * > shadows;
	static Shadow * my_shadow;
};

#endif
