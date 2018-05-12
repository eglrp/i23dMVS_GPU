#ifndef _WILDCARD_H_
#define _WILDCARD_H_

// From stlplus3: http://sourceforge.net/projects/stlplus/files/

////////////////////////////////////////////////////////////////////////////////

//   Copyright: (c) Southampton University 1999-2004
//              (c) Andy Rushton           2004 onwards
//   License:   BSD License, see ../docs/license.html

//   This is a portable interface to wildcard matching.

//   The problem:
//     *  matches any number of characters - this is achieved by matching 1 and seeing if the remainder matches
//        if not, try 2 characters and see if the remainder matches etc.
//        this must be recursive, not iterative, so that multiple *s can appear in the same wildcard expression
//     ?  matches exactly one character so doesn't need the what-if approach
//     \  escapes special characters such as *, ? and [
//     [] matches exactly one character in the set - the difficulty is the set can contain ranges, e.g [a-zA-Z0-9]
//        a set cannot be empty and the ] character can be included by making it the first character

////////////////////////////////////////////////////////////////////////////////

namespace stlplus
{

  // wild = the wildcard expression
  // match = the string to test against that expression
  // e.g. wildcard("[a-f]*", "fred") returns true
  bool wildcard(const std::string& wild, const std::string& match);

}

#endif //_WILDCARD_H_