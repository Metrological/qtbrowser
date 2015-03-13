#include "nrdlink.h"
#include <stdio.h>

static QOpenGLContext* _context;
static QOffscreenSurface* _surface;

void SetEGLParameters (QOpenGLContext* context, QOffscreenSurface* surface)
{
    _context = context;
    _surface = surface;
}

EGLDisplay GetDisplay     ( EGLNativeDisplayType )
{
	return (eglGetCurrentDisplay());
}

EGLSurface GetSurface     ( EGLDisplay, EGLConfig )
{
	if (_surface != NULL)
	{
		return (reinterpret_cast<EGLSurface>(_surface));
	}

	return (NULL);
}

EGLContext GetContext     ( EGLDisplay, EGLConfig, EGLContext, const EGLint* )
{
	if (_context != NULL)
        { 
		QVariant value =  _context->nativeHandle();

		if (value.isValid ())
		{
			bool converted = true;
			unsigned int native = value.toUInt(&converted);

			if (converted == true)
			{
				return (reinterpret_cast<EGLContext>(native));
			}
			else
			{
				printf ("This was type: %s\n", value.typeName());
			}	
		}
		else
		{
			printf ("No native context handle.");
		}	
	}
	return (NULL);
}

EGLContext CurrentContext ( )
{
	return (eglGetCurrentContext());
}

EGLint MakeCurrent    ( EGLDisplay, EGLSurface, EGLSurface, EGLContext )
{
   if (_context != NULL)
   {
     _context->makeCurrent(_surface);
   }

   return (0);
}

bool AudioFrame (const unsigned char* /* frame */, const unsigned int /* size */)
{
    return (false);
}

bool VideoFrame (const unsigned long long /* timeStamp */, const unsigned char* /* frame */, const unsigned int /* size */)
{
    return (false);
}

