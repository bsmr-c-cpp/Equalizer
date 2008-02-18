
/* 
 * Copyright (c) 2006-2007, Stefan Eilemann <eile@equalizergraphics.com> 
 * All rights reserved.
 *
 * The pipe object is responsible for maintaining GPU-specific and
 * frame-specific data. The identifier passed by the application contains the
 * version of the frame data corresponding to the rendered frame. The pipe's
 * start frame callback synchronizes the thread-local instance of the frame data
 * to this version.
 */

#include "pipe.h"
#include "node.h"

#include <eq/eq.h>

using namespace eqBase;
using namespace std;

namespace eVolve
{
eq::WindowSystem Pipe::selectWindowSystem() const
{
    const Node*            node     = static_cast<Node*>( getNode( ));
    const InitData&        initData = node->getInitData();
    const eq::WindowSystem ws       = initData.getWindowSystem();

    if( ws == eq::WINDOW_SYSTEM_NONE )
        return eq::Pipe::selectWindowSystem();
    if( !supportsWindowSystem( ws ))
    {
        EQWARN << "Window system " << ws 
               << " not supported, using default window system" << endl;
        return eq::Pipe::selectWindowSystem();
    }

    return ws;
}

bool Pipe::configInit( const uint32_t initID )
{
    if( !eq::Pipe::configInit( initID ))
        return false;

    const Node*     node        = static_cast<Node*>( getNode( ));
    const InitData& initData    = node->getInitData();
    const uint32_t  frameDataID = initData.getFrameDataID();
    eq::Config*     config      = getConfig();

    const bool mapped = config->mapObject( &_frameData, frameDataID );
    EQASSERT( mapped );


    const string&  filename    = initData.getFilename();
    const uint32_t precision   = initData.getPrecision();
    const bool     perspective = initData.getPerspective();
    EQINFO << "Loading model " << filename << endl;

    _model = new Model( filename.c_str(), precision, perspective );
    EQASSERT( _model );

    if( !_model->loadHeader( initData.getBrightness(), initData.getAlpha( )))
    {
        setErrorMessage( "Can't load model header file" );
        EQWARN << "Can't load model header file: " << filename << ".vhf"
               << endl;
        delete _model;
        _model = 0;
        return false;
    }

    return mapped;
}


bool Pipe::configExit()
{
    delete _model;
    _model = 0;

    eq::Config* config = getConfig();
    config->unmapObject( &_frameData );

    return eq::Pipe::configExit();
}


void Pipe::frameStart( const uint32_t frameID, const uint32_t frameNumber )
{
    // don't wait for node to start frame, local sync not needed
    // node->waitFrameStarted( frameNumber );
    _frameData.sync( frameID );
    startFrame( frameNumber );
}


}
