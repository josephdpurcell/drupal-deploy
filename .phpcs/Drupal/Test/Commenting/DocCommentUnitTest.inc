<?php

/**
 * This doc comment ends in a dot, but has some white space after it. 
 */

/**
 *  Separate different-type sections by a blank line.
 *
 * @param string $param
 *   Something incredibly useful.
 * @return bool
 *   Returns FALSE.
 * @throws Exception
 *   Thrown when $param is TRUE.
 * @ingroup sniffer
 * @deprecated
 * @see some_function()
 * @todo Fix coding standards.
 */
function test7($param) {
  if ($param) {
    throw new Exception();
  }
  return FALSE;
}

/**
 *No space before the short description.
 */
function test8() {

}

/**
 * starting lower case and no dot at the end
 */
function test9() {

}

/**
 * Short description ok.
 *
 * long description starting lower case.
 */
function test10() {

}

class Test {

  /**
   * No newline between tag groups.
   *
   * @see some_function()
   * @todo Fix coding standards.
   */
  public function test() {
  }
}

/**
 * Short description.
 *
 * Long description without full stop
 */
function test11() {

}

/**
 * Long description will start with a number below.
 *
 * 64KB ought to be enough for anybody.
 */
function test12() {

}

/**
 * Creates an organisation with data provided in a table.
 *
 * Table format:
 * @codingStandardsIgnoreStart
 * | name              | chairman         | founder                 | founding year | motto                                  |
 * | City Watch        | Sir Samuel Vimes | King Veltrick I         | 1561 UC       | Fabricati Diem, Pvncti Agvnt Celeriter |
 * | Assassin's Guild  | Lord Downey      | Sir Gyles de Munforte   | 1512 AM       | Nil Mortifi, Sine Lvcre                |
 * | Unseen University | Mustrum Ridcully | Alberto Malich the Wise | 1281 AM       | Nvnc Id Vides, Nvnc Ne Vides           |
 * @codingStandardsIgnoreEnd
 *
 * @param TableNode $organisation_table
 *   The organisation data.
 *
 * @Given (the following )organisations:
 */
public function givenOrganisations(TableNode $organisation_table) {
}
