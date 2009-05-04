<?php
// $Id: $

/**
 * @file
 * Include user module properties.
 */

/**
 * Implementation of hook_user_import_form_field_match().
 */
function user_user_import_form_field_match() {

  $options = array();

  $options['user']['email'] = t('Email Address *');
  $options['user']['password'] = t('Password');
  $options['user']['role'] = t('Role');
  $options['user']['language'] = t('Language');
  $options['user']['delete'] = t('Delete');

  return $options;
}


/**
 * Implementation of hook_user_import_form_fieldsets().
 */
function user_user_import_form_fieldset($import, $collapsed) {

  $form = array();
  user_user_import_edit_roles_fields($form, $import, $collapsed);
  user_user_import_edit_email_fields($form, $import, $collapsed);

  return $form;
}

/**
 * Implementation of hook_user_import_data().
 */
function user_user_import_data($settings, $column_settings, $module, $field_id, $data, $column_id) {

  if ($module != 'user') return;

  if ($field_id == 'email') {
    $value = trim($data[$column_id]);
    _user_import_validate_email($value, $settings['options']['update_users']);
  }

  if ($field_id == 'password') {
    $value = trim($data[$column_id]);
  }

  if ($field_id == 'role') {
    $value = drupal_strtolower(trim($data[$column_id]));
  }

  if ($field_id == 'language') {
    $value = drupal_strtolower(trim($data[$column_id]));
  }

  if ($field_id == 'delete') {
    $value = drupal_strtolower(trim($data[$column_id]));
  }

  return $value;
}

/**
 * Implementation of hook_user_import_pre_save().
 */
function user_user_import_pre_save($settings, $account, $password, $fields, $errors) {

  $account_add['mail'] = $fields['user']['email'][0];

  // get roles from csv
  $role = $fields['user']['role'][0];

  if (isset($role)) {
    //check if role exists:
    $rid = db_result(db_query("SELECT rid FROM {role} WHERE name = '%s'", $role));
    if ($rid) {
      $account_add['roles'][$rid] = $rid;
    }
    else {
      // it doesn't exist, create it
      db_query("INSERT INTO {role} (name) VALUES ('%s')", $role);
      $rid = db_last_insert_id('role', 'rid');
      $account_add['roles'][$rid] = $rid;
    }
  }
    
  if (empty($settings['options']['update_users'])) {

    $account_add['timezone'] = variable_get('date_default_timezone', '-18000');
    $account_add['status'] = 1;
    $account_add['init'] = $fields['user']['email'][0];
    $account_add['pass'] = (empty($fields['user']['password'][0])) ? user_password() : $fields['user']['password'][0];
    $account_add['language'] = $fields['user']['language'][0];

    // get enabled roles
    if (is_array($settings['roles'])) {
      while (list ($rid, $role_set) = each ($settings['roles'])) {
        if (!empty($role_set)) $account_add['roles'][$rid] = $rid;
      }
    }

    if (!empty($settings['options']['activate'])) {
      $account_add['access'] = time();
      $account_add['login'] = time();
    }

    $account_add['contact_setting'] = isset($settings['contact']) ? $settings['contact'] : 0;
  }

  return $account_add;
}

/**
 * Implementation of hook_user().
 */
function user_import_user($op, &$edit, &$account, $category = NULL) {
  if ($op == 'insert') {
    // overwrite the default contact status
    $edit['contact'] = $edit['contact_setting'];
    unset($edit['contact_setting']);
  }
}

function user_user_import_edit_roles_fields(&$form, $import, $collapsed) {

  $roles_data = user_roles();
  // remove 'anonymous user' option
  while (list ($rid, $role_name) = each ($roles_data)) {
      if ($role_name != 'anonymous user' && $role_name != 'authenticated user') $roles[$rid] = $role_name;
  }

  // roles selected
  if ( !empty($import['roles']) ) {
      foreach ($import['roles'] as $rid) {
          if ($rid != 0) $roles_selected[] = $rid;
      }
  }

  if ( empty($roles_selected) ) $roles_selected[] = 2;

  $form['role_selection'] = array(
      '#type' => 'fieldset',
      '#title' => t('Role Assign'),
      '#description' => t("Select which role(s) imported users should be assigned. The role 'authenticated user' is assigned automatically."),
      '#weight' => -80,
      '#collapsible' => TRUE,
      '#collapsed' => $collapsed,
  );

  $form['role_selection']['roles'] = array(
      '#type' => 'checkboxes',
      '#options' => $roles,
      '#default_value' => $roles_selected,
  );

  return;
}

function user_user_import_edit_email_fields(&$form, $import, $collapsed) {

  $form['email_message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email Message'),
    '#description' => t('Welcome message to be sent to imported users. Leave blank to use the default !message.<br /><strong>Warning</strong>: if you copy and paste directly from Word some characters may get garbled.', array('!message' => l('message', 'admin/user/settings'))),
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
  );

  $form['email_message']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Message Subject'),
    '#default_value' => $import['options']['subject'],
    '#description' => t('Customize the subject of the welcome e-mail, which is sent to imported members.') .' '. t('Available variables are:') .' !username, !site, !password, !uri, !uri_brief, !mailto, !date, !login_uri, !edit_uri, !login_url'. $profile_string .'.',
  );

  $form['email_message']['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => $import['options']['message'],
    '#description' => t('Customize the body of the welcome e-mail, which is sent to imported members.') .' '. t('Available variables are:') .' !username, !site, !password, !uri, !uri_brief, !mailto, !login_uri, !edit_uri, !login_url'. $profile_string .'.',
  );

  $form['email_message']['message_format'] = array(
    '#type' => 'radios',
    '#title' => t('Email Format'),
    '#default_value' => empty($import['options']['message_format']) ? 0 : $import['options']['message_format'],
    '#options' => array(t('Plain Text'), t('HTML')),
  );

  $form['email_message']['message_css'] = array(
    '#type' => 'textarea',
    '#title' => t('CSS'),
    '#default_value' => $import['options']['message_css'],
    '#description' => t('Use if sending HTML formated email.'),
  );

  return;
}


function _user_import_validate_email($email = NULL, $duplicates_allowed = FALSE) {
  if (!$email) {
    user_import_errors(t('no email'));
  }
  elseif (!valid_email_address($email)) {
    user_import_errors(t('invalid email'));
  }
  // Handle duplicate emails.
  elseif (!$duplicates_allowed && _user_import_existing_uid($email)) {
    user_import_errors(t('duplicate email'));
  }
  
  return;
}
