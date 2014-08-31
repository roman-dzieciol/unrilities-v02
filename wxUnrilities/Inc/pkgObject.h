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
//  pkgObject.h
// ============================================================================
#ifndef PKGOBJECT_H
#define PKGOBJECT_H

#include "pkgArchive.h"
#include "unFlagManager.h"
#include "pkgSerializer.h"

struct pkgExportItem;
class pkgSerializer;
class pkgProperty;
struct pkgNameItem;


// ============================================================================
// PropertyInfo
// ============================================================================
#define PROPERTY_TYPE_MASK		0x0f
#define PROPERTY_SIZE_MASK		0x70
#define PROPERTY_ARRAY_MASK		0x80


// ============================================================================
// PropertyType
// ============================================================================
enum PropertyType
{
    PT_Byte        = 1
,   PT_Int         = 2
,   PT_Bool        = 3
,   PT_Float       = 4
,   PT_Object      = 5
,   PT_Name        = 6
,   PT_String      = 7
,   PT_Class       = 8
,   PT_Array       = 9
,   PT_Struct      = 10
,   PT_Vector      = 11
,   PT_Rotator     = 12
,   PT_Str         = 13
,   PT_Map         = 14
,   PT_FixedArray  = 15
};


// ============================================================================
// EPropertyToken
// ============================================================================
enum EPropertyToken
{
    PRT_None            = 0x00,
    PRT_Property        = 0x01,
    PRT_Array           = 0x02,
    PRT_ArrayHdr        = 0x03
};

typedef vector<byte> TPropertyTokens;

typedef ownvector<pkgProperty> pkgProperties;


// ============================================================================
// pkgStateFrame
// ============================================================================
struct pkgStateFrame
{
    friend pkgArchive& operator >> ( pkgArchive& A, pkgStateFrame& D )
    {
        A >> D.Node;
        A >> D.StateNode;
        A >> D.ProbeMask;
        A >> D.LatentAction;
        if( D.Node != 0 )
            A >> D.Offset;
        return A;
    }

    unIndex Node;
    unIndex StateNode;
    qword   ProbeMask;
    int32   LatentAction;
    unIndex Offset;
};


// ============================================================================
// pkgDependency
// ============================================================================
class pkgDependency
{
public:
    friend pkgArchive& operator >> ( pkgArchive& A, pkgDependency& D )
    {
        A >> D.Class;
        A >> D.Deep;
        A >> D.ScriptTextCRC;
        return A;
    }

protected:
    unIndex     Class;
    dword       Deep;
    dword       ScriptTextCRC;
};


// ============================================================================
// pkgClass
// ============================================================================
class pkgClass
{
public:
    friend pkgArchive& operator >> ( pkgArchive& A, pkgClass& D )
    {
        A >> D.SuperField;
        A >> D.Next;

        A >> D.ScriptText;
        A >> D.Children;
        A >> D.FriendlyName;
        A >> D.UnknownA;
        A >> D.Line;
        A >> D.TextPos;
        A >> D.ScriptSize;

        if( D.ScriptSize > 0 )
        {
            // ScriptSize invalid, decompiler required
            throw exFail( wxT("Can't inspect class object, decompiler required.") );
        }

        A >> D.ProbeMask;
        A >> D.IgnoreMask;
        A >> D.LabelTableOffset;
        A >> D.StateFlags;

        A >> D.ClassFlags;
        A >> D.ClassGuid;

        A >> D.DependenciesCount;
        D.Dependencies.resize(D.DependenciesCount);
        for( vector<pkgDependency>::iterator it=D.Dependencies.begin(); it!=D.Dependencies.end(); ++it )
        {
            A >> static_cast<pkgDependency&>(*it);
        }

        A >> D.PackageImportsCount;
        D.PackageImports.resize(D.PackageImportsCount);
        for( vector<unIndex>::iterator it=D.PackageImports.begin(); it!=D.PackageImports.end(); ++it )
        {
            A >> static_cast<unIndex&>(*it);
        }

        A >> D.ClassWithin;
        A >> D.ClassConfigName;

        A >> D.HideCategoriesCount;
        D.HideCategories.resize(D.HideCategoriesCount);
        for( vector<unIndex>::iterator it=D.HideCategories.begin(); it!=D.HideCategories.end(); ++it )
        {
            A >> static_cast<unIndex&>(*it);
        }

        return A;
    }

    bool HasScript() const { return ScriptSize > 0; }

protected:

    // Field
    unIndex     SuperField;
    unIndex     Next;

    // Struct
    unIndex     ScriptText;
    unIndex     Children;
    unIndex     FriendlyName; 
    dword       UnknownA;
    dword       Line;
    dword       TextPos;
    dword       ScriptSize;

    // State
    qword       ProbeMask;
    qword       IgnoreMask;
    word        LabelTableOffset;
    dword       StateFlags;

    // Class
    dword       ClassFlags;
    unGuid      ClassGuid;

    unIndex     DependenciesCount;
    vector<pkgDependency> Dependencies;

    unIndex     PackageImportsCount;
    vector<unIndex> PackageImports;

    unIndex     ClassWithin;
    unIndex     ClassConfigName;

    unIndex     HideCategoriesCount;
    vector<unIndex> HideCategories;

};


// ============================================================================
// pkgObject
// ============================================================================
class pkgObject
{
public:
    pkgObject( pkgSerializer* serializer, dword index );
    ~pkgObject();

public:
    friend pkgArchive& operator >> ( pkgArchive& A, pkgObject& D )
    {
        D.LoadObject();
        return A;
    }

public:
    void LoadObject();
    void LoadProperties();
    void DumpProperties();

protected:
    pkgSerializer*          Serializer;
    dword                   ExportIndex;
    const pkgExportItem&    ExportItem;

    pkgStateFrame*          StateFrame;
    pkgClass*               Class;
    unFlag                  Flags;
    ownvector<pkgProperty>  Properties;
};




// ============================================================================
// pkgProperty
// ============================================================================
class pkgProperty
{
public:
    pkgProperty( const pkgNameItem& name, byte info, byte type, pkgArchive& A, pkgSerializer& S )
    :   Name(name)
    ,   Info(info)
    ,   Type(type)
    {
    }

    virtual ~pkgProperty()
    {
    }

    static dword ReadArray( pkgArchive& A, dword info )
    {
        if( (info & PROPERTY_ARRAY_MASK) != 0 )
        {
            byte idxinfo, index;
            A >> idxinfo;

            if( idxinfo < 128 ) 
            {
                index = static_cast<dword>(idxinfo);
            }
            else 
            {
                A.Seek( -1, wxFromCurrent );
                word idxword;
                A >> idxword;
                idxword = idxword & 0x7fff;
                if( idxword < 16384 )
                {
                    index = static_cast<dword>(idxword);
                }
                else 
                {
                    A.Seek( -2, wxFromCurrent );
                    A >> index;
                    index = index & 0x3fffffff;
                }
            }

            if( index < 0 )
                throw exFail( wxT("invalid array index") );

            return index;
        }
        else
            return -1;
    }

    static dword ReadSize( pkgArchive& A, dword info )
    {
        switch( info & PROPERTY_SIZE_MASK ) 
        {
        case	0x00:	return 1;	
        case	0x10:	return 2;											
        case	0x20:	return 4;											
        case	0x30:	return 12;										
        case	0x40:	return 16;										
        case	0x50:	byte  psizeb; A >> psizeb; return psizeb;       
        case	0x60:	word  psizew; A >> psizew; return psizew;	      
        case	0x70:	dword psized; A >> psized; return psized;
        default:		
            throw exFail(wxT("Unknown Size"));
        }
    }

    static const pkgNameItem ReadNameItem( pkgSerializer& S )
    {
        return S.GetNameItem( unIndex(S) );
    }

    static pkgProperty* CreateProperty( pkgSerializer& S, pkgArchive& A );
    //static bool IsSpecialCase( pkgSerializer& S, pkgArchive& A, pkgProperty* P );

    wxString GetName()   { return Name.Name; }
    dword GetNameIdx()   { return Name.ItemIndex; }
    bool IsArray() const { return Array != -1; }

    void Tell( pkgArchive& A ) const { wxLogMessage(wxT("%x"),static_cast<dword>(A.Tell())); }
    virtual wxString DumpInfo();


public:
    pkgNameItem Name;
    byte        Info;
    byte        Type;

    dword       Size;
    dword       Array;


public:
    static const wxString   PropertyTerminator;
};




// ============================================================================
// PKG_PROPERTY_CTOR
// ============================================================================
#define PKG_PROPERTY_CTOR(T) \
T( const pkgNameItem& name, byte info, byte type, pkgArchive& A, pkgSerializer& S ) \
:   pkgProperty( name, info, type, A, S )


// ============================================================================
// pkgpByte
// ============================================================================
class pkgpByte : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpByte)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        A >> Value;     
    }

public:
    byte Value;
};


// ============================================================================
// pkgpInt
// ============================================================================
class pkgpInt : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpInt)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        A >> Value;     
    }

public:
    int32 Value;
};


// ============================================================================
// pkgpBool
// ============================================================================
class pkgpBool : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpBool)
    {
        Size  = ReadSize(A,info);  // Read property size
        
        // Read property value
        Value = (Info & PROPERTY_ARRAY_MASK) != 0;
        Array = -1; 
    }
    /*pkgpBool( const pkgNameItem* name, dword size, bool barray, pkgArchive& A )
    :   Value(barray)
    ,   pkgProperty(name, size, false, A )
    {
    }*/

public:
    bool Value;
};


// ============================================================================
// pkgpFloat
// ============================================================================
class pkgpFloat : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpFloat)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        A >> Value;     
    }

public:
    float Value;
};


// ============================================================================
// pkgpObject
// ============================================================================
class pkgpObject : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpObject)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        A >> Value;     
    }

public:
    unIndex Value;
};


// ============================================================================
// pkgpName
// ============================================================================
class pkgpName : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpName)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        A >> Value;     
    }

public:
    unIndex Value;
};


// ============================================================================
// pkgpString
// ============================================================================
class pkgpString : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpString)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        //A >> Value;     
        A.Seek( Size, wxFromCurrent );
    }

public:
    unStringZ Value;
};


// ============================================================================
// pkgpClass
// ============================================================================
class pkgpClass : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpClass)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        A.Seek( Size, wxFromCurrent );
        //throw exception(wxT("Class Property not supported"));
    }

public:
};


// ============================================================================
// pkgpArray
// ============================================================================
class pkgpArray : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpArray)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        //A >> ItemCount;     
        A.Seek( Size, wxFromCurrent );
    }

public:
    unIndex                 ItemCount;  // Number of items in array
    ownvector<pkgProperty>  Properties;
};


// ============================================================================
// pkgpStruct
// ============================================================================
class pkgpStruct : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpStruct)
    {
        Struct = ReadNameItem(S);   // Read struct type
        Size   = ReadSize(A,info);  // Read property size
        Array  = ReadArray(A,info); // Read array info

        // ASSUME: 
        // Native structs have special-case serialization code.
        // Therefore just read/skip specified number of bytes.
        if( Struct.Flags.HasFlag(ObjectFlags::Native) )
        {
            A.Seek( Size, wxFromCurrent );
        }

        // ASSUME: 
        // Non-Native structs are saved as block of None-terminated properties
        // TODO:
        // Find out why Size is off by few bytes sometimes
        else
        {
            wxFileOffset readrange = A.Tell() + Size;
            while( A.Tell() < readrange )
            {
                if( !Properties.push_existing( pkgProperty::CreateProperty(S,A) ) )
                    break;
            }
        }
    }

public:
    pkgNameItem             Struct;
    ownvector<pkgProperty>  Properties;
};


// ============================================================================
// pkgpVector
// ============================================================================
class pkgpVector : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpVector)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        A.Seek( Size, wxFromCurrent );
        //throw exception(wxT("Vector Property not supported"));
    }

public:
};


// ============================================================================
// pkgProperty
// ============================================================================
class pkgpRotator : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpRotator)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        A.Seek( Size, wxFromCurrent );
        //throw exception(wxT("Rotator Property not supported"));
    }

public:
};


// ============================================================================
// pkgpStr
// ============================================================================
class pkgpStr : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpStr)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        A >> Value;     
    }

public:
    unName Value;
};


// ============================================================================
// pkgpMap
// ============================================================================
class pkgpMap : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpMap)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        A.Seek( Size, wxFromCurrent );
        //throw exception(wxT("Map Property not supported"));
    }

public:
};


// ============================================================================
// pkgpFixedArray
// ============================================================================
class pkgpFixedArray : public pkgProperty
{
public:
    PKG_PROPERTY_CTOR(pkgpFixedArray)
    {
        Size  = ReadSize(A,info);  // Read property size
        Array = ReadArray(A,info); // Read array info

        // Read property value
        A.Seek( Size, wxFromCurrent );
        //throw exception(wxT("FixedArray Property not supported"));
    }

public:
};



#endif PKGOBJECT_H
// ============================================================================