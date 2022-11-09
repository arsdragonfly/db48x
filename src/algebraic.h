#ifndef ALGEBRAIC_H
#define ALGEBRAIC_H
// ****************************************************************************
//  algebraic.h                                                  DB48X project
// ****************************************************************************
//
//   File Description:
//
//     RPL algebraic objects
//
//     RPL algebraics are objects that can be placed in algebraic expression
//     (between quotes). They are defined by a precedence and an arity.
//     Items with higher precedence are grouped, a.g. * has higher than +
//     Arity is the number of arguments the command takes
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
//     Unlike traditional RPL, algebraics are case-insensitive, i.e. you can
//     use either "DUP" or "dup". There is a setting to display them as upper
//     or lowercase. The reason is that on the DM42, lowercases look good.
//
//     Additionally, many algebraics also have a long form. There is also an
//     option to prefer displaying as long form. This does not impact encoding,
//     and when typing programs, you can always use the short form

#include "command.h"

struct algebraic : command
// ----------------------------------------------------------------------------
//   Shared logic for all algebraics
// ----------------------------------------------------------------------------
{
    algebraic(id i): command(i) {}

    // Arity is the number of arguments this takes on the stack
    static uint arity()                 { return 1; }

    // Precedence is the precedence when rendering as equations
    static uint precedence()            { return 1; }

    // Promotion of integer / fractions to real
    static bool real_promotion(gcobj &x, object::id type);
    static id   real_promotion(gcobj &x);

    // Function pointers used by generic evaluation code
    typedef void (*bid128_fn)(BID_UINT128 *res, BID_UINT128 *x);
    typedef void (*bid64_fn) (BID_UINT64  *res, BID_UINT64  *x);
    typedef void (*bid32_fn) (BID_UINT32  *res, BID_UINT32  *x);

    // Precedence for the various operators
    enum
    {
        LOGICAL         = 1,    // and, or, xor
        RELATIONAL      = 3,    // <, >, =, etc
        ADDITIVE        = 5,    // +, -
        MULTIPICATIVE   = 7,    // *, /
        POWER           = 9,    // ^

        FUNCTION_POWER  = 44,   // X²

        UNKNOWN         = 55,   // Unknown operator
        PARENTHESES     = 77,   // Parentheses
        SYMBOL          = 88,   // Names
        FUNCTION        = 99,   // Functions, e.g. f(x)
    };

    // Standard object interface
    OBJECT_HANDLER_NO_ID(algebraic);
};

#endif // ALGEBRAIC_H
