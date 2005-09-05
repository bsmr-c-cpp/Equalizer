
/* Copyright (c) 2005, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#ifndef EQNET_GLOBAL_H
#define EQNET_GLOBAL_H

#include <string>

/** 
 * @namespace eqNet
 * @brief The Equalizer networking abstraction layer.
 *
 * The Equalizer network abstraction layer provides the basic functionality to
 * enable execution on distributed and shared memory machines.
 */
namespace eqNet
{
    // global defines

    /** An invalid identifier. */
#   define INVALID_ID 0xfffffffe

    /** The any node identifier. */
#   define NODE_ID_ANY 0xffffffff

    /** 
     * Initialises the Equalizer networking layer.
     * 
     * @param argc the command line argument count.
     * @param argv the command line argument values.
     */
    void init( int argc, char** argv );

    /** 
     * Global parameter handling for the Equalizer network implementation. 
     */
    class Global
    {
    public:
        /** 
         * Sets the name of the program.
         * 
         * @param programName the program name.
         */
        static void setProgramName( const std::string& programName );

        /** 
         * Gets the name of the program.
         * 
         * @return the program name.
         */
        static const std::string& getProgramName();

    private:
        static std::string _programName;
    };
}

#endif // EQNET_GLOBAL_H

