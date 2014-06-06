#ifndef Object3D_h
#define Object3D_h

#include "AbstractFramework.h"

class Object3D
{
    public:
        static void setFramework( AbstractFramework* );
        void draw();

    protected:
        static AbstractFramework* m_Framework;
        virtual void drawShape() = 0;
};


#endif
