/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef PY_RENDER_TARGET_HPP
#define PY_RENDER_TARGET_HPP

#include "moo/render_target.hpp"
#include "pyscript/script.hpp"
#include "pyscript/pyobject_plus.hpp"
#include "py_texture_provider.hpp"

class PyRenderTarget : public PyObjectPlusWithWeakReference
{
	Py_Header( PyRenderTarget, PyObjectPlusWithWeakReference )
public:
	PyRenderTarget( Moo::RenderTargetPtr pRT, PyTypePlus *pType = &s_type_ );
	~PyRenderTarget();

	Moo::RenderTargetPtr pRenderTarget() const	{ return pRenderTarget_; }
	PyTextureProviderPtr pTexture()				
	{ 
		PyTextureProviderPtr tex( 
			new PyTextureProvider(this, pRenderTarget_), 
			PyObjectPtr::STEAL_REFERENCE ); 
		return tex;
	}

	PyObject *pyGetAttribute( const char *attr );
	int pySetAttribute( const char *attr, PyObject *value );

	PY_RO_ATTRIBUTE_DECLARE( pRenderTarget_->width(), width );
	PY_RO_ATTRIBUTE_DECLARE( pRenderTarget_->height(), height );
	PY_RO_ATTRIBUTE_DECLARE( pRenderTarget_->format(), format );
	PY_RO_ATTRIBUTE_DECLARE( pRenderTarget_->textureMemoryUsed(), textureMemoryUsed );
	PY_RO_ATTRIBUTE_DECLARE( pRenderTarget_->resourceID(), name );
	PY_RO_ATTRIBUTE_DECLARE( pTexture(), texture );

	void release()	{ pRenderTarget_->release(); }
	PY_AUTO_METHOD_DECLARE( RETVOID, release, END )	

	PY_FACTORY_DECLARE()
private:	
	Moo::RenderTargetPtr pRenderTarget_;
};

typedef SmartPointer<PyRenderTarget> PyRenderTargetPtr;

PY_SCRIPT_CONVERTERS_DECLARE( PyRenderTarget )

#ifdef CODE_INLINE
#include "py_render_target.ipp"
#endif

#endif //#ifndef PY_MATERIAL_HPP