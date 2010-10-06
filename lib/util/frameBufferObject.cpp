
/* Copyright (c) 2008-2009, Cedric Stalder <cedric.stalder@gmail.com>
                 2009-2010, Stefan Eilemann <eile@equalizergraphics.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *  
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "frameBufferObject.h"

#include <eq/fabric/pixelViewport.h>

#ifdef WIN32
#  define bzero( ptr, size ) memset( ptr, 0, size );
#endif

namespace eq
{
namespace util
{

FrameBufferObject::FrameBufferObject( const GLEWContext* glewContext, 
                                      const GLenum targetTextures )
    : _fboID( 0 )
    , _depth( targetTextures, glewContext )
    , _glewContext( glewContext )
    , _targetTextures( targetTextures )
    , _valid( false )
{
    EQASSERT( GLEW_EXT_framebuffer_object );

    _colors.push_back( new Texture( _targetTextures, glewContext ));
}

FrameBufferObject::~FrameBufferObject()
{
    exit();
    for( size_t i = 0; i < _colors.size(); ++i )
    {
        delete _colors[i];
        _colors[i] = 0;
    }
}

bool FrameBufferObject::addColorTexture()
{
    if( _colors.size() >= 16 )
    {
        EQERROR << "Too many color textures, can't add another one";
        return false;
    }

    _colors.push_back( new Texture( _targetTextures, _glewContext ));
    _valid = false;
    return true;
}

bool FrameBufferObject::init( const int32_t width, const int32_t height,
                              const GLuint colorFormat,
                              const int32_t depthSize,
                              const int32_t stencilSize )
{
    EQ_TS_THREAD( _thread );

    if( _fboID )
    {
        _error = "FBO already initialized";
        EQWARN << _error << std::endl;
        return false;
    }

    // generate and bind the framebuffer
    glGenFramebuffersEXT( 1, &_fboID );
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, _fboID );

    // create and bind textures
    for( size_t i = 0; i < _colors.size(); ++i )
    {
        _colors[i]->init( colorFormat, width, height );
        _colors[i]->bindToFBO( GL_COLOR_ATTACHMENT0 + i, width, height );
    }
    if( stencilSize > 0 && GLEW_EXT_packed_depth_stencil )
    {
        _depth.init( GL_DEPTH24_STENCIL8, width, height );
        _depth.bindToFBO( GL_DEPTH_STENCIL_ATTACHMENT, width, height ); 
    }
    else if( depthSize > 0 )
    {
        _depth.init( GL_DEPTH_COMPONENT, width, height );
        _depth.bindToFBO( GL_DEPTH_ATTACHMENT, width, height );
    }

    if( _checkStatus( ))
        return true;
    // else

    exit();
    return false;
}

void FrameBufferObject::exit()
{
    EQ_TS_THREAD( _thread );
    if( _fboID )
    {
        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
        glDeleteFramebuffersEXT( 1, &_fboID );
        _fboID = 0;
    }

    for( size_t i = 0; i < _colors.size(); ++i )
        _colors[i]->flush();
    _depth.flush();

    _valid = false;
}

bool FrameBufferObject::_checkStatus()
{
    switch( glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT ))
    {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            EQVERB << "FBO supported and complete" << std::endl;
            _valid = true;
            return true;

        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            _error = "Unsupported framebuffer format";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            _error = "Framebuffer incomplete, missing attachment";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            _error = "Framebuffer incomplete, incomplete attachment";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            _error = "Framebuffer incomplete, \
                      attached images must have same dimensions";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            _error = "Framebuffer incomplete, \
                      attached images must have same format";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            _error = "Framebuffer incomplete, missing draw buffer";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            _error = "Framebuffer incomplete, missing read buffer";
            break;
        default:
            break;
    }

    EQWARN << _error << std::endl;
    _valid = false;
    return false;
}

void FrameBufferObject::bind()
{
    EQ_TS_THREAD( _thread );
    EQASSERT( _fboID );
    EQ_GL_CALL( glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, _fboID ));
}

void FrameBufferObject::unbind()
{
    EQ_GL_CALL( glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 ));
}

bool FrameBufferObject::resize( const int32_t width, const int32_t height )
{
    EQ_TS_THREAD( _thread );
    EQASSERT( width > 0 && height > 0 );

    EQASSERT( !_colors.empty( ));
    Texture* color = _colors.front();

    if( color->getWidth() == width && color->getHeight() == height && _valid )
       return true;

    for( size_t i = 0; i < _colors.size(); ++i )
        _colors[i]->resize( width, height );

    if ( _depth.isValid( ))
        _depth.resize( width, height );

    return _checkStatus();
}

}
}
