#pragma once

#include <iostream>


class CustomWorld : public chai3d::cWorld
{
public:
    virtual void renderSceneGraph(chai3d::cRenderOptions& a_options);

};