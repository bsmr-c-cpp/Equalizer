
/* Copyright (c) 2007-2009, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#ifndef EQNET_STATICMASTERCM_H
#define EQNET_STATICMASTERCM_H

#include <eq/net/objectCM.h> // base class
#include <eq/net/object.h>   // nested enum (Object::Version)
#include <eq/base/idPool.h>  // for EQ_ID_INVALID

#include <deque>

namespace eq
{
namespace net
{
    class Node;

    /** 
     * An object change manager handling a static master instance.
     */
    class StaticMasterCM : public ObjectCM
    {
    public:
        StaticMasterCM( Object* object );
        virtual ~StaticMasterCM();

        virtual void makeThreadSafe(){}

        /**
         * @name Versioning
         */
        //*{
        virtual uint32_t commitNB() { EQDONTCALL; return EQ_ID_INVALID; }
        virtual uint32_t commitSync( const uint32_t commitID )
            { EQDONTCALL; return Object::VERSION_NONE; }

        virtual void obsolete( const uint32_t version ) { EQDONTCALL; }

        virtual void setAutoObsolete( const uint32_t count, 
                                      const uint32_t flags ) { EQDONTCALL; }
        
        virtual uint32_t getAutoObsoleteCount() const
            { EQDONTCALL; return 0; }

        virtual uint32_t sync( const uint32_t version )
            { EQDONTCALL; return Object::VERSION_NONE; }

        virtual uint32_t getHeadVersion() const { return Object::VERSION_NONE; }
        virtual uint32_t getVersion() const     { return Object::VERSION_NONE; }
        virtual uint32_t getOldestVersion() const {return Object::VERSION_NONE;}
        //*}

        virtual bool isMaster() const { return true; }
        virtual uint32_t getMasterInstanceID() const
            { EQDONTCALL; return EQ_ID_INVALID; }
        virtual void addSlave( NodePtr node, const uint32_t instanceID,
                               const uint32_t version );
        virtual void removeSlave( NodePtr node ) {}
        virtual void addOldMaster( NodePtr node, const uint32_t instanceID )
            { EQDONTCALL }

        virtual void applyMapData() { EQDONTCALL; }

    private:
        /** The managed object. */
        Object* _object;
    };
}
}

#endif // EQNET_STATICMASTERCM_H
