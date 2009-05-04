<?php
// $Id: $

/**
 * @file
 * Import node_import and cck module properties.
 */

// Functionality depends on node_import and CCK.
if (module_exists('node_import') && module_exists('content')) {

  // Load the required API files.
  include_once(drupal_get_path('module', 'node_import') .'/node_import.api.inc');
  node_import_load_supported();

  /**
   * Implementation of hook_user_import_form_field_match().
   */
  function nodeprofile_user_import_form_field_match() {
    $options = array();
    $options['nodeprofile'] = array();

    $nodeprofile_types = (array) nodeprofile_get_types('types');

    foreach (nodeprofile_get_types() as $type => $data) {
      $fields = node_import_fields($type);
      // Give fields a more descriptive title.
      foreach (array_keys($fields) as $key) {
        $fields[$key] = str_replace(t('Node: '), '', $fields[$key]);
        $fields[$key] = t('Node profile: !key', array('!key' => $fields[$key]));
      }
      $options['nodeprofile'] = array_merge($options['nodeprofile'], $fields);
    }

    return $options;
  }

  /**
   * Implementation of hook_user_import_data().
   */
  function nodeprofile_user_import_data($settings, $column_settings, $module, $field_id, $data, $column_id) {

    if ($module != 'nodeprofile') return;

    return trim($data[$column_id]);
  }

  /**
   * Implementation of hook_user_import_after_save().
   */
  function nodeprofile_user_import_after_save($settings, $account, $password, $fields) {
    if (!is_array($fields['nodeprofile'])) return;

    $nodeprofile_types = (array) nodeprofile_get_types();

    foreach (array_keys($nodeprofile_types) as $type) {
      $errors = array();
      $node = new stdClass();
      $node->type = $type;
      $node->status = 1;
      // Assign the mapped fields to the $node.
      foreach ($fields['nodeprofile'] as $column_id => $column_data) {
        if (!empty($column_data)) {
          $node->$column_id = $column_data[0];
        }
      }

      // Call the node import preparation.
      foreach (module_list() as $module_name) {
        $function = $module_name .'_node_import_prepare';
        if (function_exists($function)) {
          $errors = array_merge((array)$errors, (array)$function($node, $preview > 0));
        }
      }

      if (empty($errors)) {
        $node->uid = $account->uid;
        $node->name = $account->name;

        // Assign a default title if one has not already been mapped.
        if (!isset($node->title) || empty($node->title)) {
          $node->title = $node->name;
        }

        $node = node_submit($node);

        // Look for an existing node. This works because profile node types
        // can only have one node per user.
        // change to:
        // if ($old_node = node_load($type, $account->uid)) {
        // using node_load() to temporarely avoid upgrading nodeprofile
        if ($old_node = node_load(array('type' => $node->type, 'uid' => $account->uid))) {
          // By explicitly setting a nid value, we force an update.
          $node->nid = $old_node->nid;
        }

        node_save($node);
        // option to delete user if nodeprofile error
      }
      else {
        // report errors
      }
    }

    return;
  }
}