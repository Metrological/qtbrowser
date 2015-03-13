// #include <gibbon/external/MetrologicalDRM.h>
#include <gibbon/external/MetrologicalEGL.h>
#include <gibbon/external/MetrologicalInput.h>
#include <gibbon/external/MetrologicalPlayer.h>

EGLDisplay GetDisplay     ( EGLNativeDisplayType )
{
	return (0);
}

EGLSurface GetSurface     ( EGLDisplay, EGLConfig )
{
	return (NULL);
}

EGLContext GetContext     ( EGLDisplay, EGLConfig, EGLContext, const EGLint* )
{
	return (NULL);
}

EGLContext CurrentContext ( )
{
	return (NULL);
}

EGLint     MakeCurrent    ( EGLDisplay, EGLSurface, EGLSurface, EGLContext )
{
	return (0);
}

bool AudioFrame (const unsigned char* frame, const unsigned int size)
{
    return (false);
}

bool VideoFrame (const unsigned long long timeStamp, const unsigned char* frame, const unsigned int size)
{
    return (false);
}

