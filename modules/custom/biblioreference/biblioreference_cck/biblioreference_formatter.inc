<?php
// $Id$

/**
 * @file
 * Defines a CCK field formatter for biblio nodereferences.
 */

// Implementation of hook_theme() is in biblioreference_cck.module.

/**
 * FAPI theme for an individual text elements.
 */
function theme_xxxx_link($element) {
//  drupal_add_css(drupal_get_path('module', 'link') .'/link.css');

  // Prefix single value link fields with the name of the field.
  if (empty($element['#field']['multiple'])) {
    if (isset($element['url']) && isset($element['title'])) {
      $element['url']['#title'] = $element['#title'] .' '. $element['url']['#title'];
      $element['title']['#title'] = $element['#title'] .' '. $element['title']['#title'];
    }
    elseif($element['url']) {
      $element['url']['#title'] = $element['#title'];
    }
  }

  $output = '';
  $output .= '<div class="link-field-subrow clear-block">';
  if (isset($element['title'])) {
    $output .= '<div class="link-field-title link-field-column">' . theme('textfield', $element['title']) . '</div>';
  }
  $output .= '<div class="link-field-url' . (isset($element['title']) ? ' link-field-column' : '') . '">' . theme('textfield', $element['url']) . '</div>';
  $output .= '</div>';
  if (!empty($element['attributes'])) {
    $output .= '<div class="link-attributes">' . theme('form_element', $element['attributes'], $element['attributes']['#value']) . '</div>';
  }
  return $output;
}


/**
 * Implementation of hook_field_formatter_info().
 */
function biblioreference_cck_field_formatter_info() {
  return array(
    'pubmed' => array(
      'label' => t('Pubmed style display for referenced biblio content ONLY'),
      'field types' => array('nodereference'),
      'multiple values' => CONTENT_HANDLE_CORE,
    ),
  );
}

/**
 * Theme function for 'pubmed' biblio nodereference field formatter.
 */
function theme_biblioreference_cck_formatter_pubmed($element) {
  $output = '';
  if (!empty($element['#item']['nid']) && is_numeric($element['#item']['nid'])) {
    $nid = $element['#item']['nid'];
    $title = _nodereference_titles($nid); // uses static caching, good
    $biblio = biblioreference_biblio_load($nid);
// $output .= '<pre>' . var_export($biblio, TRUE) . '</pre>';
//    $authors = _biblioreference_authors($nid);
    $contributors = $biblio->biblio_contributors;
    module_load_include('inc', 'biblio', 'biblio.pages');
    usort($contributors, 'biblioreference_au_compare_author_level');
    // contributor/author presentation adopted from theme_biblio_long().
    foreach ($contributors as $auth) {
      $authors .= (!empty ($authors)) ? '; '. _biblio_author_link($auth['name'], $auth['cid']) : _biblio_author_link($auth['name'], $auth['cid']);
    }
    // set the date string
    if ($biblio->biblio_date) {  // prefer the full publication date
      $date = $biblio->biblio_date;
    }
    elseif ($biblio->biblio_year) {  // take the year if available
      $date = $biblio->biblio_year;
    }
    else {
      $date = '';
    }
    // set the page string
    if ($biblio->biblio_pages) {  // prefer the page range
      $pages = $biblio->biblio_pages;
    }
    elseif ($biblio->biblio_number) {  // take the start page
      $pages = $biblio->biblio_number;
    }
    else {
      $pages = '';
    }
    // make the pubmed link
    if ($biblio->biblio_citekey && is_numeric($biblio->biblio_citekey)) {
      // @TODO can't assume citekey is pubmed ID, use custom3 to store 'pubmed' if it is
      $pubmed = l(t('PubMed'), 'http://www.ncbi.nlm.nih.gov/pubmed/' . $biblio->biblio_citekey);
    }
    else {
      $pubmed = '';
    }

    // now let's make the output
    if ($authors) {
      $output .= $authors;
      $output .= '. ';
    }
    $output .= '<strong>';
    $output .= l($title, 'node/' . $nid);
    $output .= '</strong>';
    // @TODO perhaps add punctuation if none
    $output .= ' ';
    $output .= $date;
    if ($date && $pages)  $output .= '; ';
    $output .= $pages;
    if ($date || $pages)  $output .= '. ';
    if ($pubmed) {
      $output .= $pubmed;
      $output .= '.';
    }
  }
  return $output;
}

/**
 * Potential Agaric Utility (AU) function (would need generalizing).
 */
function biblioreference_au_compare_author_level($auth_a, $auth_b) {
  // category is misspelled?!
  // this would be easier with gmp_cmp installed:
  // return gmp_cmp($auth_a['auth_catagory'], $auth_b['auth_catagory']);
  $a = $auth_a['auth_catagory'];
  $b = $auth_a['auth_catagory'];
  if ($a < $b)  return -1;
  elseif ($a > $b)  return 1;
  else  return 0;  // $a == $b
}

/**
 * Modeled very loosely on biblio_load().
 *
 * @param $nid;
 */
function biblioreference_biblio_load($nid) {
  // @TODO replace with module_load
  require_once(drupal_get_path('module', 'biblio') . '/biblio.contributors.inc');
  $biblio = db_fetch_object(db_query('SELECT b.*, bt.name as biblio_type_name
                                         FROM {biblio} b
                                         LEFT JOIN {biblio_types} bt on b.biblio_type = bt.tid
  LEFT JOIN {node} n on b.vid = n.vid
                                         WHERE n.nid = %d', $nid));
  $biblio->biblio_contributors = biblio_load_contributors($biblio->vid, $biblio->biblio_type);
  return $biblio;
}


function _biblioreference_authors($nid, $reset = FALSE) {
  static $authors = array();
  
}
