// ============================================================================
//  Unrilities :: Cross-platform toolkit for Unreal Engine packages
// ============================================================================
//  Copyright (C) 2005  Roman Dzieciol <neai at o2.pl>
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// ============================================================================
//  unDataTreeFactories.h
// ============================================================================
#ifndef UNDATATREEFACTORIES_H
#define UNDATATREEFACTORIES_H

#include "pkgSerializer.h"
#include "unDataTree.h"


// ============================================================================
// unPkgDataTreeFactory
// ============================================================================
struct unPkgDataTreeFactory
{
	pkgSerializer* Serializer;	

	unPkgDataTreeFactory( pkgSerializer* serializer, unDataTree* parenttree, pkgObjectItem* parentitem )
	:	Serializer(serializer)
	{
		AddItem(parenttree,parentitem);
	}

	void AddItem( unDataTree* parenttree, pkgObjectItem* parentitem )
	{
		for( pkgObjectItem::iterator it=parentitem->begin(); it!=parentitem->end(); ++it )
		{
			pkgObjectItem* item = static_cast<pkgObjectItem*>(*it);
			unDataTree* tree = parenttree->AddChild( new unDataTree( Serializer->GetName(item->ObjectName), item->ItemIndex, true ) );
			if( !item->empty() )
				AddItem( tree, item );
		}
	}
};


// ============================================================================
// unInheritanceMapper
// ============================================================================
struct unInheritanceMapper
{
	unInheritanceMapper( pkgSerializer* serializer, unDataTree* parenttree, pkgExportItem* parentitem ) 
	{
		typedef multimap<int,unDataTree*> MapType;	
		MapType SuperMap;							
		MapType IndexMap;							

		for( pkgExportItem::iterator it=parentitem->begin(); it!=parentitem->end(); ++it )
		{
			pkgExportItem* item = static_cast<pkgExportItem*>(*it);
			unDataTree* tree = new unDataTree( serializer->GetName(item->ObjectName), item->ItemIndex, true );
			SuperMap.insert( make_pair( int(item->Super), tree ) );
			IndexMap.insert( make_pair( int(item->ItemIndex)+1, tree ) );
		}

		int id, lastid = 0x7FFFFFFF;
		unDataTree* group = parenttree;

		for( MapType::iterator it=SuperMap.begin(); it!=SuperMap.end(); ++it )
		{
			id = (*it).first;
			if( id != lastid )
			{
				MapType::iterator found = IndexMap.find(id);
				if( found != IndexMap.end() )
				{
					group = (*found).second;
				}
				else if( id < 0 )
				{
					group = parenttree->AddChild( new unDataTree( serializer->GetObjectName(id), ID_IGNORE, true ) );
				}
				else
				{
					group = parenttree;
				}
				lastid = id;
			}

			group->AddChild( (*it).second );
		}
	}
};


// ============================================================================
// unClassMapper
// ============================================================================
typedef wxString (pkgSerializer::*NameFuncT)( int32 i ) const;

template <class IT, class MT>
class unClassMapper
{
public:
	unClassMapper( pkgSerializer* serializer, unDataTree* parenttree,  const pkgObjectItem* parentitem, MT IT::*category, NameFuncT NameFunc ) 
	:	Serializer(serializer)
	,	Category(category)
	{
		// find valid entries
		MapItems( parentitem );		

		// add class tree
		dword id, lastid = 0;
		unDataTree *tree, *classtree = NULL;
		for( MapType::iterator it=ClassMap.begin(); it!=ClassMap.end(); ++it )
		{
			// add class node
			id = (*it).first;
			if( id != lastid )
			{
				classtree = parenttree->AddChild( new unDataTree(  MEMBER_PTR(Serializer,NameFunc)(id), ID_IGNORE, true ) );
				lastid = id;
			}

			// add item node
			tree = classtree->AddChild( (*it).second );
		}
	}

	void MapItems( const pkgObjectItem* parentitem )
	{
		for( pkgObjectItem::const_iterator it=parentitem->begin(); it!=parentitem->end(); ++it )
		{
			pkgObjectItem* item = static_cast<pkgObjectItem*>(*it);
			if( item->Class != 0 )
			{
				unDataTree* tree = new unDataTree( Serializer->GetName(MEMBER_PTR(item,Category)), item->ItemIndex, true );
				ClassMap.insert( make_pair( dword( item->Class ), tree ) );
				if( !item->empty() )
					MapItems( item );
			}
		}
    }
protected:
    pkgSerializer* Serializer;						
    typedef multimap<dword,unDataTree*> MapType;	
    MapType ClassMap;		
    MT IT::*Category;
};

// ============================================================================
// ClassMapper Creator
// ============================================================================
template <class IT, class MT>
unClassMapper<IT,MT> CreateClassMapper( pkgSerializer* serializer, unDataTree* tree,  const pkgObjectItem* item, MT IT::*category, NameFuncT NameFunc ) 
{ 
	return unClassMapper<IT,MT>(serializer,tree,item,category,NameFunc); 
}



#endif UNDATATREEFACTORIES_H
// ============================================================================