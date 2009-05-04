<?php
// $Id: $

/**
 * @file
 * Import users from a comma separated file (csv).
 */

/**
 * Implementation of hook_user_import_form_fieldsets().
 * Add fieldsets to an import settings form.
 */
function user_import_user_import_form_fieldset($import, $collapsed) {

  $form = array();
  _user_import_edit_template_fields($form, $import);
  _user_import_edit_settings_fields($form, $import, $collapsed);
  _user_import_edit_remove_fields($form, $import);

  return $form;
}


/**
 * Implementation of hook_user_import_after_save().
 */
function user_import_user_import_after_save($settings, $account, $password, $fields) {

  if (!empty($settings['send_email'])) _user_import_send_email($account, $password, $fields, $settings['options']['subject'], $settings['options']['message'], $settings['options']['message_format'], $settings['options']['message_css'], $settings['options']['subscribed']);

  watchdog('user', 'New user: %name %email.', array('%name' => check_plain($account->name), '%email' => '<'. check_plain($account->mail) .'>'), WATCHDOG_NOTICE, l(t('edit'), 'user/'. $account->uid .'/edit'));

  return;
}

// Send email when account is created
function _user_import_send_email($account, $password, $profile, $subject, $body, $format, $css, $subscribed) {

  global $base_url;
  $from = variable_get('site_mail', ini_get('sendmail_from'));

  $options['absolute'] = TRUE;

  $variables = array(
    '!username' => $account->name,
    '!uid' => $account->uid,
    '!site' => variable_get('site_name', 'drupal'),
    '!login_url' => user_pass_reset_url($account),
    '!password' => $password,
    '!uri' => $base_url,
    '!uri_brief' => drupal_substr($base_url, drupal_strlen('http://')),
    '!mailto' => $account->mail,
    '!date' => format_date(time()),
    '!login_uri' => url('user', $options),
    '!edit_uri' => url('user/'. $account->uid .'/edit', $options),
  );

  _user_import_publication_email($variables, $account, $subscribed, $format);

  // import info to profile
  if (module_exists('profile') && is_array($profile)) {
    $profile_name = _user_import_profile('fid', 'name');
    while (list ($fid, $field_name) = each ($profile_name)) {
      $variables['!'. $field_name] = $profile[$fid];
    }
  }

  $subject = (empty($subject)) ? _user_mail_text('register_admin_created_subject', NULL, $variables) : strtr($subject, $variables);
  $subject = mime_header_encode($subject);
  $body = (empty($body)) ? _user_mail_text('register_admin_created_body', NULL,  $variables) : strtr($body, $variables);
  $body = str_replace("\r", '', $body);

  $header = "From: $from\n";
  $header .= "Reply-to: $from\n";
  $header .= "X-Mailer: Drupal\n";
  $header .= "Return-path: $from\n";
  $header .= "Errors-to: $from\n";
  $header .= 'MIME-Version: 1.0';
  $header .= "\n";

  if ($format == 1) {

    $header .= 'Content-Type: text/html; charset=UTF-8; Content-transfer-encoding: 8Bit';

    $body_head = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
              <html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
              <head>
              <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />';

    if (!empty($css)) $body_head .= '<style type="text/css">'. check_plain($css) .'</style>';

    $body = $body_head .'</head><body>'. $body .'<body></html>';

  }
  else {
    $header .= 'Content-Type: text/plain; charset=UTF-8; format=flowed Content-transfer-encoding: 8Bit';
  }

  mail($account->mail, $subject, $body, $header);
  return;
}

function _user_import_edit_settings_fields(&$form, $import, $collapsed) {

  $form['optional'] = array(
      '#type' => 'fieldset',
      '#title' => t('Options'),
      '#weight' => -85,
      '#collapsible' => TRUE,
      '#collapsed' => $collapsed,
  );

  $form['optional']['update_users'] = array(
      '#type' => 'checkbox',
      '#title' => t('Update Existing Users'),
      '#default_value' => $import['options']['update_users'],
      '#description' => t('If the email address of a user being imported already exists, update the existing user details with the imported data.'),
  );

  $form['optional']['first_line_skip'] = array(
      '#type' => 'checkbox',
      '#title' => t('Ignore First Line'),
      '#default_value' => $import['first_line_skip'],
      '#description' => t('If the first line is the names of the data columns, set to ignore first line.'),
  );

  $form['optional']['contact'] = array(
      '#type' => 'checkbox',
      '#title' => t('Contact'),
      '#default_value' => $import['contact'],
      '#description' => t("Set each user's personal contact form to 'allowed'."),
  );

  $form['optional']['send_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send Email'),
    '#default_value' => $import['send_email'],
    '#description' => t('Send email to users when their account is created.'),
  );

  $form['optional']['username_space'] = array(
      '#type' => 'checkbox',
      '#title' => t('Username Space'),
      '#default_value' => $import['username_space'],
      '#description' => t("Include spaces in usernames, e.g. 'John' + 'Smith' => 'John Smith'."),
  );

  $form['optional']['activate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Activate Accounts'),
      '#default_value' => $import['options']['activate'],
      '#description' => t("User accounts will not be visible to other users until their owner logs in. Select this option to make all imported user accounts visible."),
  );

  return;
}

function _user_import_edit_template_fields(&$form, $import) {

  // settings template update controls
  if (empty($import['name'])) {

     // new settings template save controls

      $form['save'] = array(
          '#type' => 'fieldset',
          '#title' => t('Save Settings'),
          '#description' => t('Save settings for re-use on other imports.'),
          '#weight' => 90,
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
      );

      $form['save']['name'] = array(
          '#type' => 'textfield',
          '#title' => t('Settings Name'),
          '#size' => 26,
          '#maxlength' => 25,
          '#description' => t('Name to identify these settings by.'),
      );

      $form['save'][] = array(
          '#type' => 'submit',
          '#value' => t('Save'),
      );

  }
  else {

      $form['save'] = array(
          '#type' => 'fieldset',
          '#title' => t('Saved Settings'),
          '#description' => t("If changes have neen made to the settings since they where last saved you can update the saved template, or save them as a new template."),
          '#weight' => 90,
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
      );

      $form['save']['name'] = array(
          '#type' => 'value',
          '#value' => $import['name']
      );

      $form['save']['update'] = array(
          '#type' => 'fieldset',
          '#title' => t('Update'),
          '#description' => t("Update '%name' settings template", array('%name' => $import['name'])),
      );

      $form['save']['update']['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Update'),
      );

      $form['save']['new'] = array(
          '#type' => 'fieldset',
          '#title' => t('Create New'),
          '#description' => t("Save as new settings template"),
      );

      $form['save']['new']['new_name'] = array(
          '#type' => 'textfield',
          '#title' => t('Save As New'),
          '#size' => 30,
          '#maxlength' => 25,
          '#description' => t('Name to identify these settings by.'),
      );

      $form['save']['new'][] = array(
          '#type' => 'submit',
          '#value' => t('Save As New'),
      );

  }

  return;
}


function _user_import_edit_remove_fields(&$form, $import) {

  $form['remove'] = array(
      '#type' => 'fieldset',
      '#title' => t('Use Different CSV File'),
      '#description' => t('Remove file to use a different file. All settings on this page will be deleted.'),
      '#weight' => -100,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );

  $form['remove']['file'] = array(
      '#type' => 'item',
      '#title' => t('Uploaded file'),
      '#value' => $import['filename']
  );

  $form['remove']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Remove file')
  );

  return;
}

function _user_import_publication_email(&$variables, $account, $subscribed, $format) {

  if (!module_exists('publication') || !module_exists('schedule') || !module_exists('identity_hash')) {
    return;
  }



  $id_hash = identity_hash_select_hash($account->uid);
  $variables['!id_hash'] = $id_hash->hash;

  while (list($type, $subscriptions) = each($subscribed)) {

    while (list($publication_id, $shedule) = each($subscriptions)) {

      if (!empty($shedule[0])) {

        $publication = publication_select_publications($type, $publication_id);

        $update_link = url('subscribed/preferences/'. $publication_id .'/'. $id_hash->hash, NULL, NULL, TRUE);
        $unsubscribe_link = url('subscribed/delete/'. $publication_id .'/'. $id_hash->hash, NULL, NULL, TRUE);

        if ($format == 1) {

          $variables['!subscribed_links'] .= '<strong>'. $publication->title .'</strong><br />'.
          '<a href="'. $update_link .'">'. t('Update Preferences') .'</a> | <a href="'. $unsubscribe_link .'">'. t('Unsubscribe') .'</a><br />';

        }
        else {

          $variables['!subscribed_links'] .= $publication->title .'\n'.
          ' - '. t('Update Preferences') .' '. $update_link .'\n'.
          ' - '. t('Unsubscribe') .' '. $unsubscribe_link .'\n';
        }
      }
    }
  }
}
