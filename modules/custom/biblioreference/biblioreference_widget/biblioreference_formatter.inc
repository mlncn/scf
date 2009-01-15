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
// @debug
  $output = '';
/*
  foreach ($element as $section) {
    $output .= 
  }
*/  
//  $output = $element; // '<pre>'.var_export($element,TRUE).'</pre>';
  $output .= array_keys($element);
  return $output;
}

