
                                 Unrilities 0.3

               Copyright (C) 2005 Roman Dzieciol <neai at o2.pl>
                  http://sourceforge.net/projects/unrilities/

-------------------------------------------------------------------------------
Welcome to Unrilities, a cross-platform toolkit for previewing and editing
Unreal Engine package files.


Other documentation files
-------------------------------------------------------------------------------
* CHANGES.txt - Record of application changes
* LICENSE.txt - Complete license text


Current feature set
-------------------------------------------------------------------------------
* Package Preview

  * Name, Export and Import package tables displayed in lists.
    * Multi-column list sorting by default.
    * Each list column has a numerical or regular expression filter. Use them to
      view just the specified items.
  
  * Info about package tables displayed in a list.
  
  * Import and Export tables displayed in tree lists.
    * Export table has 7 tree display modes:
      * "All - Package Tree" - show all items, use package tree.
      * "All - Class List" - show all items, group by item class.
      * "Scripts - Package Tree" - show scripts, use package tree.
      * "Scripts - Class List" - show scripts, group by item class.
      * "Scripts - Inheritance Tree" - show script classes, use script tree.
      * "Data - Package Tree" - show data items, use package tree.
      * "Data - Class List" - show data items, group by item class.
    * Import table has 2 tree display modes:
      * "All - Package Tree" - show all items, use package tree.
      * "All - Class List" - show all items, group by item class.


Syntax for filters
-------------------------------------------------------------------------------
* Regular expressions
  * Docs: http://wiki.wxwidgets.org/docs.cgi?q=regex&target=class&target=wiki
  * Examples:
    * [Export] Objects in MyClass class:
      * Column: "Package"
      * Expression: "^MyClass(\.|$)"

* Numerical expressions
  * Base-10 numbers have ordinary syntax, ie: 1234
  * Base-16 numbers need "0x" prefix, ie: 0xABCD
  * Numbers can be prefixed with '-' or '+' character.
  * Prefix the expression with '!' to match all numbers except this one.
  * White-space chars before and after the expression will be trimmed.
  * Examples:
    * [Export] Hide 0-sized items:
      * Column: "SerialSize"
      * Expression: "!0"


Unreal Engine products supported
-------------------------------------------------------------------------------
* Supported, Tested
  * Unreal Tournament 2004

* Supported, NOT Tested
  * Most of them

* NOT Supported, Tested
  * Unreal Championship 2

* NOT Supported, NOT Tested
  * Unpatched Unreal 1
 

Where to get the source code
-------------------------------------------------------------------------------
* CVSROOT: :pserver:anonymous@cvs.sourceforge.net:/cvsroot/unrilities
  * Repository: wxUnrilities
  * Tag: RC_0-2


Thanks
-------------------------------------------------------------------------------
Epic Games
Antonio Cordero Balcazar
Esko Ilola
#unrealscript
UnrealWiki


Licensing
-------------------------------------------------------------------------------
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 2.1 of the License, or (at your option) any
later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

-------------------------------------------------------------------------------