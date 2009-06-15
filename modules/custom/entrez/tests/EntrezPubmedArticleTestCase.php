<?php
/**
 * PHPUnit test case for entrez module
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
 * @package     entrez
 * @author      Stefan Freudenberg <stefan@agaricdesign.com>
 * @copyright   2009 Stefan Freudenberg
 * @license     http://www.gnu.org/licenses/gpl.html
 */

require_once 'PHPUnit/Framework/Test.php';
require_once dirname(__FILE__) . '/../EntrezPubmedArticle.php';

class EntrezPubmedArticleTestCase extends PHPUnit_Framework_TestCase
{
  protected $object;
  
  protected function setUp()
  {
    $testFile = 'pubmed_article.xml';
    $this->object = new EntrezPubmedArticle(simplexml_load_file($testFile));
  }
  
  public function testId()
  {
    $this->assertEquals(19479730, $this->object->getId());
  }
  
  public function testMd5()
  {
    $this->assertEquals('7b4ca58852535f6c73d04e4cbd314a12', $this->object->getMd5());
  }
  
  public function testBiblio()
  {
    $biblio = $this->object->getBiblio();
    
    $this->assertEquals('2009 May 28', $biblio['biblio_date']);
    $this->assertEquals('Human brain mapping', $biblio['biblio_secondary_title']);
    $this->assertEquals(11, count($biblio['biblio_contributors'][1]));
  }
}
