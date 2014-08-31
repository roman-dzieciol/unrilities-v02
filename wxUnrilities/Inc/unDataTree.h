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
//  unDataTree.h
// ============================================================================
#ifndef UNDATATREE_H
#define UNDATATREE_H

#include "unTree.h"

#define ID_IGNORE	dword(-1)


// ============================================================================
// unDataTree - Data storage for TreeCtrl widgets
// ============================================================================
class unDataTree : public unTree<unDataTree>
{
public:

	// ========================================================================
	// unDataTreeData
	// ========================================================================
	class unDataTreeData : public wxTreeItemData
	{
	public:		unDataTreeData( unDataTree* tree ) : Tree(tree) {}
				unDataTree*	GetTree() const { return Tree; }
	private:	unDataTree*	Tree;
	};


	// ========================================================================
	// unDataTreeSorter
	// ========================================================================
	struct unDataTreeSorter
	{
		bool operator() ( const unDataTree* a, const unDataTree* b )
		{
			return a->GetCaption().compare(b->GetCaption()) < 0;
		}
	};


public:
	unDataTree( const wxString& caption, dword serialid = ID_IGNORE, bool bsort = false ) 
	:	Caption(caption)
	,	TreeData(NULL)
	,	SerialID(serialid)
	,	bSort(bsort)
	,	bExpanded(false)
	{
	}

	~unDataTree()
	{
		for( const_iterator it=begin(); it!=end(); ++it )
			delete static_cast<unDataTree*>(*it);
	}

public:

	void Sort()
	{
		sort( begin(), end(), unDataTreeSorter() );
	}

	unDataTree* FindSerialID( dword serial ) const
	{
		for( const_iterator it=begin(); it!=end(); ++it )
		{
			unDataTree* tree = (*it);
			if( serial == tree->GetSerialID() )
				return tree;
		}

		for( const_iterator it=begin(); it!=end(); ++it )
		{
			unDataTree* tree = (*it);
			if( !tree->empty() )
			{
				unDataTree* sub = tree->FindSerialID( serial );
				if( sub != NULL )
					return sub;
			}
		}

		return NULL;
	}

	bool Expand()								
	{ 
		if( ShouldExpand() )
		{
			bExpanded = true;
			return true;
		}
		return false;
	}

	unDataTreeData*	GetTreeData()								
	{ 
		wxASSERT_MSG( !TreeData, wxT("TreeData already created") );
		TreeData = new unDataTreeData(this);
		return TreeData;
	}

public:
	const wxString&	GetCaption		() const						{ return Caption; }
	void			SetCaption		( const wxString& caption )		{ Caption = caption; }
	void			SetTreeID		( const wxTreeItemId& treeid )	{ TreeID = treeid; }
	wxTreeItemId	GetTreeID		() const						{ return TreeID; }
	void			SetSerialID		( dword serial )				{ SerialID = serial; }
	const dword		GetSerialID		() const						{ return SerialID; }
	bool			ShouldSort		() const						{ return bSort; }
	bool			ShouldExpand	() const						{ return !empty() && !bExpanded; }

protected:
	wxString			Caption;
	wxTreeItemId		TreeID;
	unDataTreeData*		TreeData;
	dword				SerialID;
	bool				bSort;
	bool				bExpanded;
};


// ============================================================================
// unDataTreeGroup
// ============================================================================
struct unDataTreeGroup : public vector<unDataTree*>
{
	unDataTree* Add( unDataTree* tree )
	{
		push_back(tree);
		return back();
	}

	wxString GetCaption( dword index ) const
	{
		return at(index)->GetCaption();
	}

	const wxArrayString GetCaptions() const
	{
		wxArrayString strings;
		for( const_iterator it=begin(); it!=end(); ++it)
			strings.Add( (*it)->GetCaption() );
		return strings;
	}
};


#endif UNDATATREE_H
// ============================================================================