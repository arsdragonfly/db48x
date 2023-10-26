#ifndef UNIT_H
#  define UNIT_H
// ****************************************************************************
//  unit.h                                                        DB48X project
// ****************************************************************************
//
//   File Description:
//
//    Unit objects represent objects such as 1_km/s.
//
//    The representation is a complex number where the x() part is the value
//    and the y() part is the unit. This makes it faster to extract them.
//
//
//
//
// ****************************************************************************
//   (C) 2023 Christophe de Dinechin <christophe@dinechin.org>
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

#include "command.h"
#include "complex.h"

#include "menu.h"

GCP(unit);

struct unit : complex
// ----------------------------------------------------------------------------
//   A unit object is mostly like an equation, except for parsing
// ----------------------------------------------------------------------------
{
    unit(id type, algebraic_r value, algebraic_r uexpr):
        complex(type, value, uexpr) {}

    static unit_p make(algebraic_r v, algebraic_r u, id ty = ID_unit)
    {
        if (!v.Safe() || !u.Safe())
            return nullptr;
        return rt.make<unit>(ty, v, u);
    }

    algebraic_p value() const   { return x(); }
    algebraic_p uexpr() const   { return y(); }

    bool convert(algebraic_g &x) const;
    bool convert(unit_g &x) const;

public:
    OBJECT_DECL(unit);
    PARSE_DECL(unit);
    RENDER_DECL(unit);
    HELP_DECL(unit);
};


struct unit_menu : menu
// ----------------------------------------------------------------------------
//   A unit menu is like a standard menu, but with conversion / functions
// ----------------------------------------------------------------------------
{
    unit_menu(id type) : menu(type) { }
    static void units(info &mi, cstring utable[], size_t count);
};


#define ID(i)
#define UNIT_MENU(UnitMenu)                                             \
struct UnitMenu : unit_menu                                             \
/* ------------------------------------------------------------ */      \
/*   Create a units menu                                        */      \
/* ------------------------------------------------------------ */      \
{                                                                       \
    UnitMenu(id type = ID_##UnitMenu) : unit_menu(type) { }             \
    OBJECT_DECL(UnitMenu);                                              \
    MENU_DECL(UnitMenu);                                                \
};
#include "ids.tbl"

COMMAND_DECLARE(ApplyUnit);
COMMAND_DECLARE(ConvertToUnit);
COMMAND_DECLARE(ApplyInverseUnit);

#endif // UNIT_H