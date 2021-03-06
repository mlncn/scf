<?php
// $Id: user.inc,v 1.5 2009/03/21 22:32:42 MegaGrunt Exp $ 

/**
 * @todo move contact options to separate contact.inc
 */

/**
 * Implementation of hook_user_import_form_field_match().
 */
function user_user_import_form_field_match() {

  $options = array();
  $options['user']['email'] = t('Email Address*');
  $options['user']['password'] = t('Password'); 

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
 * Implementation of hook_user_import_form_update_user().
 */
function user_user_import_form_update_user() {
  $form['roles'] = array('title' => t('Roles'), 'description' => t('Affected: roles assigned to user.'));
  $form['password'] = array('title' => t('Password'), 'description' => t('Affected: password.'), 'exclude_add' => TRUE);
  $form['contact'] = array('title' => t('Contact'), 'description' => t('Affected: user contact option.'), 'exclude_add' => TRUE);
  return $form;
}

/**
 * Implementation of hook_user_import_data().
 */  
function user_user_import_data($settings, $update_setting, $column_settings, $module, $field_id, $data, $column_id) {

  if ($module != 'user') return;

  if ($field_id == 'email') {
    $value = trim($data[$column_id]);
  
    _user_import_validate_email($value, $update_setting);
  }

  if ($field_id == 'password') {
    $value = trim($data[$column_id]);
  }

  return $value;
}

/**
 * Implementation of hook_user_import_pre_save().
 */
function user_user_import_pre_save($settings, $account, $fields, $errors, $update_setting_per_module) {
 
  $account_add['mail'] = $fields['user']['email'][0];

  if (!empty($account['uid'])) {     
  
    // update roles
    switch ($update_setting_per_module['roles']) {
      case UPDATE_ADD:
        // include currently assigned roles
        foreach ($account['roles'] as $rid => $role_name) {
          $account_add['roles'][$rid] = $rid;
        }
      
      case UPDATE_REPLACE:
        // update roles
        $account_add['roles'] = array();

        foreach ($settings['roles'] as $rid => $role_set) {
          if (!empty($role_set)) {
						$account_add['roles'][$rid] = $rid;	
					} 
        }

        break;
    }
    
    // update password
    if ($update_setting_per_module['password'] == UPDATE_REPLACE) {
      $account_add['pass'] = (empty($fields['user']['password'][0])) ? user_password() : $fields['user']['password'][0];
    }
    
    // update contact
    if ($update_setting_per_module['contact'] == UPDATE_REPLACE) {
      $account_add['contact'] = $settings['contact'];
    }

  } 
  else {

    $account_add['timezone'] = '-18000';
    $account_add['status'] = 1;
    $account_add['init'] = $fields['user']['email'][0];
    $account_add['pass'] = (empty($fields['user']['password'][0])) ? user_password() : $fields['user']['password'][0];

    if (!empty($settings['options']['activate'])) {
      $account_add['access'] = time();
      $account_add['login'] = time();
    }
  
    // add selected roles
    foreach ($settings['roles'] as $rid => $role_set) {
      if (!empty($role_set)) $account_add['roles'][$rid] = $rid;
    }
  }
  
  return $account_add;
}


/**
 * Implementation of hook_user_import_after_save().
 */
function user_user_import_after_save($settings, $account, $password, $fields, $updated, $update_setting_per_module) {
  /**
   * @todo change hook_user_import_after_save() so that all changes to data are returned and saved in one hit
   */
  user_save($account, array('contact' => $settings['contact']));
  return;
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
        '#description' => t('Welcome message to be sent to imported users. Leave blank to use the default !message. If an existing user account is updated no welcome email will be sent to that user. <strong>Note - if "Activate Accounts" option is enabled !login_url (one time login) will not work.</strong>', array('!message' => l('message', 'admin/user/settings'))),
        '#collapsible' => TRUE,
        '#collapsed' => $collapsed,
    );

    $form['email_message']['subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Message Subject'),
      '#default_value' => $import['options']['subject'],
      '#description' => t('Customize the subject of the welcome e-mail, which is sent to imported members.') .' '. t('Available variables are:') .' !username, !site, !password, !uri, !uri_brief, !mailto, !date, !login_uri, !edit_uri, !login_url' . $profile_string . '.',
    ); 
    
    $form['email_message']['message'] = array(
      '#type' => 'textarea',
      '#title' => t('Message'),
      '#default_value' => $import['options']['message'],
      '#description' => t('Customize the body of the welcome e-mail, which is sent to imported members.') .' '. t('Available variables are:') .' !username, !site, !password, !uri, !uri_brief, !mailto, !login_uri, !edit_uri, !login_url' . $profile_string . '.',
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

  if (empty($email)) {
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

/**
 * Return an existing user ID, if present, for a given email.
 */
function _user_import_existing_uid($email) {
  $uid = db_result(db_query("SELECT uid FROM {users} WHERE mail= '%s' LIMIT 1", $email));
  return $uid;
}
