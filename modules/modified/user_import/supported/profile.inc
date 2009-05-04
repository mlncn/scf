<?php
// $Id: $

/**
 * @file
 * Import profile module properties.
 */

/**
 * Implementation of hook_user_import_form_field_match().
 */
function profile_user_import_form_field_match() {

  $fields = _user_import_profile('fid', 'title');
  $options['profile'] = $fields;
  return $options;
}

/**
 * Implementation of hook_user_import_data().
 */
function profile_user_import_data($settings, $column_settings, $module, $field_id, $data, $column_id) {

  if ($module!= 'profile') return;

  return trim($data[$column_id]);
}

/**
 * Implementation of hook_user_import_after_save().
 */
function profile_user_import_after_save($settings, $account, $password, $fields) {
  // import info to profile
  if (is_array($fields['profile'])) {
      while (list ($fid, $data) = each ($fields['profile'])) {
      	profile_user_import_save_profile($fid, $account->uid, trim($data[0]));
      }
  }

  return;
}

function profile_user_import_save_profile($field, $uid, $value) {
  
  $type = db_result(db_query("SELECT type FROM {profile_fields} WHERE fid = %d", $field));
  if ( !empty($type) && ($type == 'date') ) {
    // Assuming MM/DD/YYYY format.
    list($month, $day, $year) = split('/', $value);
    $value = serialize( array('month' => $month, 'day' => $day, 'year' => $year));
  }
  if (db_result(db_query("SELECT * FROM {profile_values} WHERE fid = %d AND uid = %d", $field, $uid))) {
    db_query("UPDATE {profile_values} SET value = '%s' WHERE fid = %d AND uid = %d", $value, $field, $uid);
  }
  else {
    db_query("INSERT INTO {profile_values} (fid, uid, value) VALUES(%d, %d,'%s')", $field, $uid, $value);
  }
}


