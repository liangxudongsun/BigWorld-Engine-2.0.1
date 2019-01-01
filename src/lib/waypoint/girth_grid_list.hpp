/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef GIRTH_GRID_LIST_HPP
#define GIRTH_GRID_LIST_HPP

#include "girth_grid_element.hpp"

#include <vector>

class Chunk;
class NavigatorFindResult;
class Vector3;
class GeoSpaceVector3;

typedef GeoSpaceVector3 WaypointSpaceVector3;


class GirthGridList
{
public:
	GirthGridList() :
		container_()
	{}

	void add( const GirthGridElement & element );

	GirthGridElement & index( size_t index )
	{
		return container_[index];
	}

	size_t size() const
	{
		return container_.size();
	}

	void eraseIndex( size_t index );

	bool find( const WaypointSpaceVector3 & point, NavigatorFindResult & res,
		bool ignoreHeight = false ) const;

	bool findMatchOrLower( const WaypointSpaceVector3 & point,
		NavigatorFindResult & res ) const;

	void find( const Chunk * chunk, const WaypointSpaceVector3 & point,
		float & bestDistanceSquared, NavigatorFindResult & res ) const;

	void findNonVisited( const Chunk * chunk, const WaypointSpaceVector3 & point,
		float & bestDistanceSquared, NavigatorFindResult & res ) const;
private:
	typedef std::vector< GirthGridElement > Container;
	Container container_;
};

#endif // GIRTH_GRID_LIST_HPP
