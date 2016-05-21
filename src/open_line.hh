/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HEADER_GUARD_OPEN_LINE_H
#define HEADER_GUARD_OPEN_LINE_H

#include "contents.hh"

namespace vick {
namespace open_line {

/*!
 * \file open_line.hh
 * \brief Describes the open_line_below() and open_line_above()
 * functions to insert newlines.
 */

/*!
 * \brief Makes a given number of newlines after the current line and
 * enters insert mode
 */
boost::optional<std::shared_ptr<change> >
open_line_below(contents&, boost::optional<int> = 1);

/*!
 * \brief Makes a given number of newlines before the current line and
 * enters insert mode
 */
boost::optional<std::shared_ptr<change> >
open_line_above(contents&, boost::optional<int> = 1);
}
}

#endif
