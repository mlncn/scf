<?php
/**
 * PHPUnit test case for biblioreference module
 *
 * PHP versions 4 and 5
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @package     SCF
 * @author      Stefan Freudenberg <stefan@agaricdesign.com>
 * @copyright   2009 Stefan Freudenberg
 * @license     http://www.gnu.org/licenses/gpl.html
 */

require_once 'PHPUnit/Framework/Test.php';
require_once dirname(__FILE__) . '/../biblioreference.module';

class BiblioreferenceTest extends PHPUnit_Framework_TestCase
{
  function test_autocomplete_highlight() {
    $tag = 'strong';
    $subject = "Behavioral improvement in a primate Parkinson's model is "
             . "associated with multiple homeostatic effects of human neural "
             . "stem cells";
    $words = array('behavior', 'parkinson');
    $rendered = "<$tag>Behavior</$tag>al improvement in a primate "
              . "<$tag>Parkinson</$tag>'s model is associated with multiple "
              . "homeostatic effects of human neural stem cells";
    $this->assertEquals($rendered,
      biblioreference_autocomplete_highlight($subject, $words));
    
    $words = array('behavior', 'parkinson\'s');
    $rendered = "<$tag>Behavior</$tag>al improvement in a primate "
              . "<$tag>Parkinson's</$tag> model is associated with multiple "
              . "homeostatic effects of human neural stem cells";
    $this->assertEquals($rendered,
      biblioreference_autocomplete_highlight($subject, $words));
  }
}

