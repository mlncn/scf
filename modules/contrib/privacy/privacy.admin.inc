<?php
// $Id: privacy.admin.inc,v 1.1 2009/01/28 23:38:34 cptncauliflower Exp $

/**
 * @file
 * Administration callbacks for the privacy module.
 */

/*
 * Menu callback for building the admin page
 */
function privacy_adminpage() {
  $output = t('Select which fields should be able to keep private.  Only content types with extra cck fields are displayed .');

  //return the html of the admin form
  $output .= drupal_get_form('privacy_settings');

  return $output;
}

/*
 * Build form with content types and fields
 */
function privacy_settings() {
  $types = content_types();

  foreach ( $types as $type ) {
    $fields = array();
    if (isset($type['fields'])) {
      foreach ( $type['fields'] as $field_name => $field_properties ) {
        $fields[$field_name] = check_plain($field_properties['widget']['label'] .' ('. $field_properties['field_name'] .')');
      }
    }

    if ( count($fields) ) {
      $fieldset = 'privacy_'. $type['type'];
      $form[$fieldset] = array(
        '#title' => $type['name'],
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $form[$fieldset]['privacy_'. $type['type']] = array(
        '#type' => 'checkboxes',
        '#title' => t('Available fields'),
        '#default_value' => variable_get('privacy_'. $type['type'], array()),
        '#options' => $fields,
      );
    }
  }

  return system_settings_form($form);
}
