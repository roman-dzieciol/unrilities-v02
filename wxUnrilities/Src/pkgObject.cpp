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
//	pkgObject.cpp
// ============================================================================
#include "prec.h"
#include "pkgObject.h"
#include "pkgSerializer.h"


// ============================================================================
//	pkgProperty
// ============================================================================
const wxString pkgProperty::PropertyTerminator = wxT("None");


// ============================================================================
//	pkgPropertyList
// ============================================================================



// ============================================================================
//	pkgObject
// ============================================================================
pkgObject::pkgObject( pkgSerializer* serializer, dword index ) 
:   Serializer(serializer)
,   ExportIndex(index)
,   ExportItem(Serializer->GetExportItem(index))
,   StateFrame(NULL)
,   Class(NULL)
{
    Flags = ExportItem.ObjectFlags;
}

pkgObject::~pkgObject()
{
    delete StateFrame;
    delete Class;
}

void pkgObject::LoadObject()
{
    try
    {
        dword range = ExportItem.SerialOffset + ExportItem.SerialSize;
        pkgArchive& A = *Serializer;
        A.Seek( ExportItem.SerialOffset );

        if( A.Tell() >= range )
            return;

        if( Flags.HasFlag(ObjectFlags::HasStack) )
        {
            StateFrame = new pkgStateFrame;
            A >> *StateFrame;
        }

        if( A.Tell() >= range )
            return;

        if( ExportItem.Class == 0 )
        {
            Class = new pkgClass;
            A >> *Class;

            if( !Class->HasScript() )
                LoadProperties();
        }
        else
        {
            LoadProperties();
        }

    }
    catch( exception& e )
    {
        wxString oname = Serializer->GetObjectName( ExportIndex+1 );
        if( Properties.empty() )
        {
            wxString s;
            s.Printf(wxT("!\t[%s] First Property, %s"), oname.wx_str(), e.what());
            wxLogMessage(wxT("%s"), s.wx_str()); 
            //throw exception(s.wx_str()); 
        }
        else
        {
            wxString s;
            s.Printf(wxT("!\t[%s] After %s, %s"), oname.wx_str(), Properties.back()->GetName().wx_str(), e.what()); 
            wxLogMessage(wxT("%s"), s.wx_str()); 
            //throw exception(s.wx_str()); 
        }
    }

}

void pkgObject::LoadProperties()
{
    pkgArchive& A = *Serializer;
    dword range = ExportItem.SerialOffset + ExportItem.SerialSize;
    
    while( A.Tell() < range )
    {
        if( !Properties.push_existing( pkgProperty::CreateProperty(*Serializer,A) ) )
            break;  
    }
}


void pkgObject::DumpProperties()
{
    wxString oname = Serializer->GetObjectName( ExportIndex+1 );
    wxString props;
    for( pkgProperties::iterator it = Properties.begin(); it!=Properties.end(); ++it )
    {
        pkgProperty* p = (*it);
        props.Append( p->DumpInfo() );
        if( it+1 != Properties.end() )
            props.Append( wxT(", ") );
    }

    if( Properties.begin() == Properties.end() )
    {
        //wxLogMessage( wxT("[%s] No Properties"), oname );
    }
    else
    {
        wxLogMessage( wxT("[%s] %s"), oname, props );
    }
}

pkgProperty* pkgProperty::CreateProperty( pkgSerializer& S, pkgArchive& A )
{
    // Read common header and create property
    // Property reads everything else

    // 1. Read name index
    const pkgNameItem pname = ReadNameItem(S);

    // Name "None" marks end of properties block
    if( PropertyTerminator.IsSameAs( pname.Name.GetData() ) )
        return NULL;

    // 2. Read property info
    byte pinfo;
    A >> pinfo;

    // Get property type and create property object
    byte ptype = pinfo & PROPERTY_TYPE_MASK;
    switch( ptype )	
    {
    case	PT_Byte:		return new pkgpByte			(pname,pinfo,ptype,A,S);    break;
    case	PT_Int:		    return new pkgpInt			(pname,pinfo,ptype,A,S);    break;
    case	PT_Bool:		return new pkgpBool			(pname,pinfo,ptype,A,S);    break;
    case	PT_Float:		return new pkgpFloat		(pname,pinfo,ptype,A,S);    break;
    case	PT_Object:		return new pkgpObject		(pname,pinfo,ptype,A,S);    break;
    case	PT_Name:		return new pkgpName			(pname,pinfo,ptype,A,S);    break;
    case	PT_String:		return new pkgpString		(pname,pinfo,ptype,A,S);    break;
    case	PT_Class:		return new pkgpClass		(pname,pinfo,ptype,A,S);    break;
    case	PT_Array:		return new pkgpArray		(pname,pinfo,ptype,A,S);    break;
    case	PT_Struct:		return new pkgpStruct		(pname,pinfo,ptype,A,S);    break;
    case	PT_Vector:		return new pkgpVector		(pname,pinfo,ptype,A,S);    break;
    case	PT_Rotator:	    return new pkgpRotator		(pname,pinfo,ptype,A,S);    break;
    case	PT_Str:		    return new pkgpStr			(pname,pinfo,ptype,A,S);    break;
    case	PT_Map:		    return new pkgpMap			(pname,pinfo,ptype,A,S);    break;
    case	PT_FixedArray:	return new pkgpFixedArray	(pname,pinfo,ptype,A,S);    break;
    default:	
        throw exFail( wxT("Unknown Property") );
    }

    return NULL;
}


wxString pkgProperty::DumpInfo()
{
    return GetName().wx_str();
}

// ============================================================================