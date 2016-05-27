/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "open_line.hh"
#include "concat_c.hh"
#include "contents.hh"
#include "vick-insert-mode/src/insert_mode.hh"

namespace vick {
namespace open_line {

struct open_line_c : public change {
private:
    const move_ts num_lines;
    const move_t y;

public:
    open_line_c(move_ts num_lines, move_t y)
        : num_lines(num_lines)
        , y(y) {
        assert(num_lines != 0);
    }
    virtual bool is_overriding() override { return true; }
    virtual void undo(contents& contents) override {
        if (num_lines < 0) {
            contents.cont.erase(contents.cont.begin() + y,
                                contents.cont.begin() + y -
                                    num_lines);
            // contents.y = y /*+ num_lines*/;
        } else {
            contents.cont.erase(contents.cont.begin() + y + 1,
                                contents.cont.begin() + y + 1 +
                                    num_lines);
            // contents.y = y + num_lines;
        }
        contents.y = y;
        contents.x = 0;
    }
    virtual void redo(contents& contents) override {
        if (num_lines < 0) {
            for (int x = 0; x > num_lines; x--)
                contents.cont.insert(contents.cont.begin() + y + x,
                                     "");
            contents.y = y /*+ num_lines*/;
        } else {
            for (int x = 1; x <= num_lines; x++)
                contents.cont.insert(contents.cont.begin() + y + x,
                                     "");
            contents.y = y + num_lines;
        }
        contents.x = 0;
    }
    virtual std::shared_ptr<change>
    regenerate(const contents& contents) const override {
        return std::make_shared<open_line_c>(num_lines, contents.y);
    }
};

boost::optional<std::shared_ptr<change> >
open_line_below(contents& contents, boost::optional<int> num) {
    if (num and num.get() == 0)
        return boost::none;
    std::shared_ptr<change> open_line =
        std::make_shared<open_line_c>(num ? num.get() : 1,
                                      contents.y);
    open_line->redo(contents);
    auto insert = insert_mode::enter_insert_mode(contents);
    if (insert) {
        auto list = {open_line, insert.get()};
        return boost::optional<std::shared_ptr<change> >(
            std::make_shared<concat_c>(list));
    } else {
        return open_line;
    }
}

boost::optional<std::shared_ptr<change> >
open_line_above(contents& contents, boost::optional<int> num) {
    return open_line_below(contents, num ? -num.get() : -1);
}
}
}
