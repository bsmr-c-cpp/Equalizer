
/* Copyright (c) 2007-2009, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#include "staticSlaveCM.h"

#include "command.h"
#include "commands.h"
#include "log.h"
#include "object.h"
#include "objectInstanceDataIStream.h"
#include "session.h"

#include <eq/base/scopedMutex.h>

using namespace eq::base;
using namespace std;

namespace eq
{
namespace net
{
StaticSlaveCM::StaticSlaveCM( Object* object )
        : _object( object )
        , _currentIStream( new ObjectInstanceDataIStream )
{
    registerCommand( CMD_OBJECT_INSTANCE_DATA,
           CommandFunc<StaticSlaveCM>( this, &StaticSlaveCM::_cmdInstanceData ),
                     0 );
    registerCommand( CMD_OBJECT_INSTANCE,
               CommandFunc<StaticSlaveCM>( this, &StaticSlaveCM::_cmdInstance ),
                     0 );
}

StaticSlaveCM::~StaticSlaveCM()
{
    delete _currentIStream;
    _currentIStream = 0;
}

//---------------------------------------------------------------------------
// command handlers
//---------------------------------------------------------------------------
CommandResult StaticSlaveCM::_cmdInstanceData( Command& command )
{
    EQASSERT( _currentIStream );
    _currentIStream->addDataPacket( command );
    return eq::net::COMMAND_HANDLED;
}

CommandResult StaticSlaveCM::_cmdInstance( Command& command )
{
    EQASSERT( _currentIStream );
    _currentIStream->addDataPacket( command );
 
    const ObjectInstancePacket* packet = 
        command.getPacket<ObjectInstancePacket>();
    _currentIStream->setVersion( packet->version );

    EQLOG( LOG_OBJECTS ) << "id " << _object->getID() << "." 
                         << _object->getInstanceID() << " ready" << endl;
    return eq::net::COMMAND_HANDLED;
}

void StaticSlaveCM::applyMapData()
{
    EQASSERT( _currentIStream );
    _currentIStream->waitReady();

    _object->applyInstanceData( *_currentIStream );

    delete _currentIStream;
    _currentIStream = 0;

    EQLOG( LOG_OBJECTS ) << "Mapped initial data for " << _object->getID()
                         << "." << _object->getInstanceID() << " ready" << endl;
}

}
}
