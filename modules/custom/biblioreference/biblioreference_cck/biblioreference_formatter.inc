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
 * Theme function for 'pubmed' biblio nodereference field formatter. Returns
 * styled biblio reference.
 *
 * @param array $element
 * @return string formatted output
 */
function theme_biblioreference_cck_formatter_pubmed($element) {
  $output = '';
  
  if (!empty($element['#item']['nid']) && is_numeric($element['#item']['nid'])) {
    $nid = $element['#item']['nid'];
    $biblio = biblioreference_biblio_load($nid);
    $style = variable_get('biblio_style', 'classic');
    $output = theme('biblio_entry', $biblio, 'biblio', $style);
  }
  
  return $output;
}

/**
 * Compares two authors by auth_catagory.
 *
 * @param array $auth_a
 * @param array $auth_b
 * @return int
 */
function biblioreference_au_compare_author_level($auth_a, $auth_b) {
  // category is misspelled?!
  // this would be easier with gmp_cmp installed:
  // return gmp_cmp($auth_a['auth_catagory'], $auth_b['auth_catagory']);
  $a = $auth_a['auth_catagory'];
  $b = $auth_b['auth_catagory'];
  
  if ($a < $b)  return -1;
  elseif ($a > $b)  return 1;
  else  return 0;
}

/**
 * Returns a biblio item with contributors. Modeled very loosely on
 * biblio_load().
 *
 * @param int $nid;
 * @return object
 */
function biblioreference_biblio_load($nid) {
  module_load_include('inc', 'biblio', 'biblio.contributors');
  $sql = 'SELECT b.*, bt.name as biblio_type_name '
       . 'FROM {biblio} b LEFT JOIN {biblio_types} bt '
       . 'ON b.biblio_type = bt.tid LEFT JOIN {node} n ON b.vid = n.vid '
       . 'WHERE n.nid = %d';
  $biblio = db_fetch_object(db_query($sql, $nid));
  $biblio->title = _nodereference_titles($nid);
  $biblio->biblio_contributors = biblio_load_contributors($biblio->vid,
    $biblio->biblio_type);
  
  return $biblio;
}

/**
 *
 * @param int $nid
 * @param bool $reset
 * @return unknown_type
 */
function _biblioreference_authors($nid, $reset = FALSE) {
  static $authors = array();
}
