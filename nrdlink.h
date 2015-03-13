#ifndef __NRDLINK_H_
#define __NRDLINK_H_

// #include <gibbon/external/MetrologicalDRM.h>
#include <gibbon/external/MetrologicalEGL.h>
#include <gibbon/external/MetrologicalInput.h>
#include <gibbon/external/MetrologicalPlayer.h>

#include <QOpenGLContext>
#include <QOffscreenSurface>

void SetEGLParameters (QOpenGLContext* context, QOffscreenSurface* surface);

#endif
