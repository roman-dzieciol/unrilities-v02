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
//	pkgArchive.h
// ============================================================================
#ifndef PKGARCHIVE_H
#define PKGARCHIVE_H

#define CHECK_EOF if(A.Eof() && A.Tell() > A.Length()) throw exFail(wxT("Unexpected EOF"))


// ============================================================================
// pkgArchive
// ============================================================================
class pkgArchive : public wxFFile
{
};


// ============================================================================
// pkgArchive stream interface
// ============================================================================
inline pkgArchive& operator >> ( pkgArchive& A, qword& D )	{ A.Read( &D, sizeof(qword) ); CHECK_EOF; /*wxUINT64_SWAP_ON_?E(D);*/ return A; }
inline pkgArchive& operator >> ( pkgArchive& A, dword& D )	{ A.Read( &D, sizeof(dword) ); CHECK_EOF; /*wxUINT32_SWAP_ON_?E(D);*/ return A; }
inline pkgArchive& operator >> ( pkgArchive& A, word& D )	{ A.Read( &D, sizeof(word) ); CHECK_EOF; /*wxUINT16_SWAP_ON_?E(D);*/ return A; }
inline pkgArchive& operator >> ( pkgArchive& A, byte& D )	{ A.Read( &D, sizeof(byte) ); CHECK_EOF; return A; }
inline pkgArchive& operator >> ( pkgArchive& A, int64& D )	{ A.Read( &D, sizeof(int64) ); CHECK_EOF; /*wxINT64_SWAP_ON_?E(D);*/ return A; }
inline pkgArchive& operator >> ( pkgArchive& A, int32& D )	{ A.Read( &D, sizeof(int32) ); CHECK_EOF; /*wxINT32_SWAP_ON_?E(D);*/ return A; }
inline pkgArchive& operator >> ( pkgArchive& A, int16& D )	{ A.Read( &D, sizeof(int16) ); CHECK_EOF; /*wxINT16_SWAP_ON_?E(D);*/ return A; }
inline pkgArchive& operator >> ( pkgArchive& A, int8& D )	{ A.Read( &D, sizeof(int8) ); CHECK_EOF; return A; }
inline pkgArchive& operator >> ( pkgArchive& A, char& D )	{ A.Read( &D, sizeof(int8) ); CHECK_EOF; return A; }
inline pkgArchive& operator >> ( pkgArchive& A, float& D )	{ A.Read( &D, sizeof(dword) ); CHECK_EOF; /*wxINT32_SWAP_ON_?E(D);*/ return A; }


// ============================================================================
// unIndex -- compressed index
// ============================================================================
class unIndex
{
public:
	// Definitions
	typedef int32	dtype;

	// Constructors
	unIndex() : Data(0), BytesRead(0) {}
	unIndex( pkgArchive& A ) : Data(0), BytesRead(0) { A >> *this; }

	// Operators
	unIndex& operator =		( const dtype& D )	{ Data = D; return *this; }
	operator dtype			() const			{ return Data; }
	int32	GetBytesRead	() const			{ return BytesRead; }
	int32	GetData			() const			{ return Data; }

	// Serialization
	friend pkgArchive& operator >> ( pkgArchive& A, unIndex& D )
	{
		byte B0,B1,B2,B3,B4;
		dtype value = 0;
		D.BytesRead = 0;

		A >> B0;
		++D.BytesRead;
		if(B0 & 0x40){
			A >> B1;
			++D.BytesRead;
			if(B1 & 0x80){
				A >> B2;
				++D.BytesRead;
				if(B2 & 0x80){
					A >> B3;
					++D.BytesRead;
					if(B3 & 0x80){
						A >> B4;
						++D.BytesRead;
						value = B4;
					}value = (value << 7) | (B3 & 0x7f);
				}value = (value << 7) | (B2 & 0x7f);
			}value = (value << 7) | (B1 & 0x7f);
		}value = (value << 6) | (B0 & 0x3f);

		if(B0 & 0x80) 
			value = -value;

		D.Data = value;
		return A;
	}

protected:
	dtype	Data;
	dword	BytesRead;
}; 



// ============================================================================
// unName -- pascal string
// ============================================================================
class unName
{
public:
	// Constructors
	unName(){}
	unName( pkgArchive& A ) { A >> *this; }

	// Operators
    operator wxString() const	{ return Data; }
    wxString GetData() const    { return Data; }

	// Serialization
	friend pkgArchive& operator >> ( pkgArchive& A, unName& D )
	{
        unIndex len(A);
		std::string buffer;
		buffer.resize(len);
        if( len > 0 && len < MaxLength )
		{
			A.Read( &buffer[0], len );
            D.Data = buffer;
			CHECK_EOF;
		}
        else if( len == 0 )
        {
            D.Data = buffer;
        }
		else
		{
			//D.Data = wxT("INVALID NAME LENGTH");
			wxString err;
			err.Printf( wxT("Invalid name length (%d) at offset %d"), len.GetData(), static_cast<dword>(A.Tell() - len.GetBytesRead()) );
			throw exFail( err.wx_str() );
		}
		return A;
	}

protected:
	wxString			Data;
	static const dword	MaxLength = 16777216;
};


// ============================================================================
// unGuid
// ============================================================================
struct unGuid
{
	friend pkgArchive& operator >> ( pkgArchive& A, unGuid& D )
	{
		return A >> D.A >> D.B >> D.C >> D.D;
	}

	operator wxString() const
	{
		return wxString::Format(wxT("%X-%X-%X-%X"),A,B,C,D);
	}

	dword	A;
	dword	B;
	dword	C;
	dword	D;
};


// ============================================================================
// unFlag
// ============================================================================
class unFlag
{
public:
    // Definitions
    typedef int32	dtype;

    // Constructors
    unFlag() : Data(0){}
    unFlag( pkgArchive& A ) : Data(0) { A >> *this; }

    // Operators
    unFlag& operator =		( const dtype& D )	{ Data = D; return *this; }
    operator dtype			() const			{ return Data; }

    // Serialization
    friend pkgArchive& operator >> ( pkgArchive& A, unFlag& D )
    {
        A >> D.Data;
        return A;
    }

    // helpers
    bool HasFlag( dword flag )
    {
        return (Data & flag) == flag;
    }

protected:
    dtype	Data;
}; 


// ============================================================================
// unStringZ -- zero-terminated string
// ============================================================================
class unStringZ
{
public:
    // Constructors
    unStringZ(){}
    unStringZ( pkgArchive& A ) { A >> *this; }

    // Operators
    operator wxString() const	{ return Data; }

    // Serialization
    friend pkgArchive& operator >> ( pkgArchive& A, unStringZ& D )
    {
        D.Data.Clear();
        byte idx = 0;
        char c;
        do		{ A >> c; D.Data.Append(c); CHECK_EOF;	}
        while	( c != 0 && idx++ < MaxLength );
        return A;
    }

protected:
    wxString			Data;
    static const dword	MaxLength = 16777216;
};


#endif PKGARCHIVE_H
// ============================================================================