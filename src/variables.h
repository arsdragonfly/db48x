#ifndef VARIABLES_H
#define VARIABLES_H
// ****************************************************************************
//  variables.h                                                   DB48X project
// ****************************************************************************
//
//   File Description:
//
//    Operations on variables
//
//    Global variables are stored in catalog objects
//    Local variables are stored just above the stack
//
//
//
//
//
// ****************************************************************************
//   (C) 2022 Christophe de Dinechin <christophe@dinechin.org>
//   This software is licensed under the terms outlined in LICENSE.txt
// ****************************************************************************
//   This file is part of DB48X.
//
//   DB48X is free software: you can redistribute it and/or modify
//   it under the terms outlined in the LICENSE.txt file
//
//   DB48X is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//
// Payload format:
//
//   A catalog is represented in memory as follows:
//   - The type ID (one byte, ID_directory)
//   - The total length of the directory
//   - For each entry:
//     * An object for the name, normally an ID_symbol
//     * An object for the content
//
//   This organization makes it possible to put names or values from directories
//   directly on the stack.
//
//   Unlike the HP48, the names can be something else than symbols.
//   This is used notably
//
//   Searching through a catalog is done using a linear search, but given the
//   small number of objects typically expected in a calculator, this should be
//   fine. Note that local variables, which are more important for the
//   performance of programs.
//
//   Catalogs are the only mutable RPL objects.
//   They can change when objects are stored or purged.

#include "list.h"
#include "runtime.h"
#include "command.h"
#include "menu.h"


struct catalog : list
// ----------------------------------------------------------------------------
//   Representation of a catalog
// ----------------------------------------------------------------------------
{
    catalog(id type = ID_catalog): list(nullptr, 0, type)
    {}

    static size_t required_memory(id i)
    {
        return leb128size(i) + leb128size(0);
    }

    bool store(gcobj name, gcobj value);
    // ------------------------------------------------------------------------
    //    Store an object in the catalog
    // ------------------------------------------------------------------------

    object_p recall(object_p name) const;
    // ------------------------------------------------------------------------
    //    Check if a name exists in the catalog, return value pointer if it does
    // ------------------------------------------------------------------------

    object_p lookup(object_p name) const;
    // ------------------------------------------------------------------------
    //    Check if a name exists in the catalog, return name pointer if it does
    // ------------------------------------------------------------------------

    size_t purge(object_p name);
    // ------------------------------------------------------------------------
    //   Purge an entry from the catalog, return purged size
    // ------------------------------------------------------------------------

    size_t count()
    // ------------------------------------------------------------------------
    //   Return the number of variables in the catalog
    // ------------------------------------------------------------------------
    {
        return enumerate(nullptr, nullptr);
    }


    typedef bool (*enumeration_fn)(symbol_p name, object_p obj, void *arg);
    size_t enumerate(enumeration_fn callback, void *arg);
    // ------------------------------------------------------------------------
    //   Enumerate all the variables in the catalog, return count of true
    // ------------------------------------------------------------------------


    OBJECT_HANDLER(catalog);
    OBJECT_PARSER(catalog);
    OBJECT_RENDERER(catalog);
};

typedef const catalog *catalog_p;


COMMAND_DECLARE(Sto);
COMMAND_DECLARE(Rcl);
COMMAND_DECLARE(Purge);
COMMAND_DECLARE(PurgeAll);
COMMAND_DECLARE(Mem);
COMMAND_DECLARE(FreeMemory);
COMMAND_DECLARE(GarbageCollect);

struct VariablesMenu : menu
// ----------------------------------------------------------------------------
//   The variables menu is a bit special
// ----------------------------------------------------------------------------
//   The VariablesMenu is shows variables in the current menu
//   For each variable, the function key evaluates it, shift recalls it,
//   and xshift stores it. In program mode, the function key shows the name
//   for evaluation purpose, and shifted, shows it between quotes
{
    VariablesMenu(id type = ID_VariablesMenu) : menu(type) {}

    static uint count_variables();
    static void list_variables(info &mi);

public:
    OBJECT_HANDLER(VariablesMenu);
};

COMMAND_DECLARE(VariablesMenuRecall);
COMMAND_DECLARE(VariablesMenuStore);

#endif // VARIABLES_H