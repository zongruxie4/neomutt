/**
 * @file
 * Test code for mutt_str_atos()
 *
 * @authors
 * Copyright (C) 2019-2023 Richard Russon <rich@flatcap.org>
 * Copyright (C) 2021 Pietro Cerutti <gahr@gahr.ch>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define TEST_NO_MAIN
#include "config.h"
#include "acutest.h"
#include <stddef.h>
#include "mutt/lib.h"
#include "test_common.h"

struct TestValue
{
  const char *str; ///< String to test
  int retval;      ///< Expected return value
  int result;      ///< Expected result (outparam)
};

// clang-format off
static const struct TestValue tests[] = {
  // Valid tests
  { "0",      0,  0 },
  { "1",      0,  1 },
  { "2",      0,  2 },
  { "3",      0,  3 },
  { " 3",     0,  3 },
  { "  3",    0,  3 },

  { "32765",  0,  32765 },
  { "32766",  0,  32766 },
  { "32767",  0,  32767 },

  { "-1",     0,  -1 },
  { "-2",     0,  -2 },
  { "-3",     0,  -3 },
  { " -3",    0,  -3 },
  { "  -3",   0,  -3 },

  { "-32766", 0,  -32766 },
  { "-32767", 0,  -32767 },
  { "-32768", 0,  -32768 },

  // Out of range tests
  { "32768",  -2, 0 },
  { "32769",  -2, 0 },
  { "32770",  -2, 0 },

  { "-32769", -2, 0 },
  { "-32770", -2, 0 },
  { "-32771", -2, 0 },

  // Invalid tests
  { "abc",    -1, 0 },
  { "a123",   -1, 0 },
  { "a-123",  -1, 0 },
  { "0a",     -1, 0 },
  { "123a",   -1, 0 },
  { "-123a",  -1, 0 },
  { "1,234",  -1, 0 },
  { "-1,234", -1, 0 },
  { "1.234",  -1, 0 },
  { "-1.234", -1, 0 },
  { ".123",   -1, 0 },
  { "-.123",  -1, 0 },
  { "3 ",     -1, 0 },
  { "-3 ",    -1, 0 },
};
// clang-format on

static const int UNEXPECTED = -9999;

void test_mutt_str_atos(void)
{
  // int mutt_str_atos(const char *str, short *dst);

  short result = UNEXPECTED;

  // Degenerate tests
  TEST_CHECK(mutt_str_atos(NULL, &result) == NULL);
  TEST_CHECK_NUM_EQ(result, 0);
  TEST_CHECK(mutt_str_atos("42", NULL) != 0);

  // Normal tests
  for (size_t i = 0; i < countof(tests); i++)
  {
    TEST_CASE(tests[i].str);

    result = UNEXPECTED;
    const char *end = mutt_str_atos(tests[i].str, &result);

    if ((tests[i].retval == 0) && (!end || *end))
    {
      TEST_MSG("retval: Expected: \\0, Got: %s", end);
    }
    else if ((tests[i].retval == -1) && end)
    {
      TEST_MSG("retval: Expected: NULL, Got: %s", end);
    }
    else if ((tests[i].retval == -2) && end)
    {
      TEST_MSG("retval: Expected: NULL, Got: %s", end);
    }
  }
}
