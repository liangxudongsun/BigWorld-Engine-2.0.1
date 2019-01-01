/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#include "pch.hpp"
#include "compression_stream.hpp"

#include "zip/zlib.h"


// -----------------------------------------------------------------------------
// Section: CompressionIStream
// -----------------------------------------------------------------------------

/**
 *	Constructor.
 */
CompressionIStream::CompressionIStream( BinaryIStream & stream ) :
	pCurrStream_( NULL )
{
	uint8 compressionType;
	stream >> compressionType;

	if (compressionType == BW_COMPRESSION_NONE)
	{
		pCurrStream_ = &stream;
	}
	else if ((BW_COMPRESSION_ZIP_BEST_SPEED <= compressionType) &&
			(compressionType <= BW_COMPRESSION_ZIP_BEST_COMPRESSION))
	{
		pCurrStream_ = &zipStream_;
		zipStream_.init( stream );
	}
	else
	{
		CRITICAL_MSG( "CompressionIStream::CompressionIStream: "
				"Invalid compression type: %d\n", compressionType );
		pCurrStream_ = &stream;
	}
}


// -----------------------------------------------------------------------------
// Section: CompressionOStream
// -----------------------------------------------------------------------------

// This value are used if BW_COMPRESSION_DEFAULT_INTERNAL is passed to the
// constructor.
BWCompressionType CompressionOStream::s_defaultInternalCompression =
			BW_COMPRESSION_NONE;

// This value are used if BW_COMPRESSION_DEFAULT_EXTERNAL is passed to the
// constructor.
BWCompressionType CompressionOStream::s_defaultExternalCompression =
			BW_COMPRESSION_NONE;

/**
 *	Constructor.
 */
CompressionOStream::CompressionOStream( BinaryOStream & stream,
		BWCompressionType compressionType ) :
	pCurrStream_( &stream )
{
	if (compressionType == BW_COMPRESSION_DEFAULT_INTERNAL)
	{
		compressionType = s_defaultInternalCompression;
	}
	else if (compressionType == BW_COMPRESSION_DEFAULT_EXTERNAL)
	{
		compressionType = s_defaultExternalCompression;
	}

	if (compressionType == BW_COMPRESSION_NONE)
	{
		pCurrStream_ = &stream;
	}
	else if ((BW_COMPRESSION_ZIP_BEST_SPEED <= compressionType) &&
			(compressionType <= BW_COMPRESSION_ZIP_BEST_COMPRESSION))
	{
		int compressionLevel = 
			(compressionType - BW_COMPRESSION_ZIP_BEST_SPEED) + Z_BEST_SPEED;
		zipStream_.init( stream, compressionLevel );
		pCurrStream_ = &zipStream_;
	}
	else
	{
		CRITICAL_MSG( "CompressionIStream::CompressionIStream: "
				"Invalid compression type: %d\n", compressionType );
		compressionType = BW_COMPRESSION_NONE;
		pCurrStream_ = &stream;
	}

	stream << uint8( compressionType );
}


/**
 *	This static method initialises the default compression to use.
 */
bool CompressionOStream::initDefaults( DataSectionPtr pSection )
{
	return initCompressionTypes( pSection,
			s_defaultInternalCompression, s_defaultExternalCompression );
}


/**
 *	This function initialises internal and external compression types.
 */
bool initCompressionTypes( DataSectionPtr pSection,
		BWCompressionType & internalCompressionType,
		BWCompressionType & externalCompressionType )
{
	if (!pSection)
	{
		return true;
	}


	DataSectionPtr pInternal = pSection->findChild( "internal" );
	DataSectionPtr pExternal = pSection->findChild( "external" );

	return initCompressionType( pInternal, internalCompressionType ) &&
			initCompressionType( pExternal, externalCompressionType );
}


/**
 *	This function initialises a BWCompressionType from a DataSection.
 */
bool initCompressionType( DataSectionPtr pSection,
		BWCompressionType & compressionType )
{
	if (!pSection)
	{
		return true;
	}

	std::string asString = pSection->asString();

	if (asString == "NONE")
	{
		compressionType = BW_COMPRESSION_NONE;
	}
	else if (asString == "ZIP")
	{
		int level = pSection->readInt( "level", Z_BEST_SPEED );

		if ((level < Z_BEST_SPEED) || (Z_BEST_COMPRESSION < level))
		{
			ERROR_MSG( "initCompressionType: "
						"Invalid zip compression level %d\n",
					level );
			return false;
		}

		compressionType = BWCompressionType( BW_COMPRESSION_ZIP_BEST_SPEED +
							(level - Z_BEST_SPEED) );
	}
	else
	{
		ERROR_MSG( "initCompressionType: Invalid compression type '%s'\n",
				asString.c_str() );
		return false;
	}

	return true;
}

// compression_stream.cpp
