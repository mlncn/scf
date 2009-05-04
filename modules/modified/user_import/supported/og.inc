<?php
// $Id: $

/**
 * @file
 * Import organic groups module properties.
 */

/**
 * Implementation of hook_user_import_form_fieldsets().
 */
function og_user_import_form_fieldset($import, $collapsed) {

  $groups = user_import_get_groups();

  $groups_description = empty($groups) ? t('No Groups have been defined.') : t('Select which group(s) imported users should be assigned.');

  $form['group_assign'] = array(
    '#type' => 'fieldset',
    '#title' => t('Group Assign'),
    '#description' => $groups_description,
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
  );

  if (!empty($groups)) {
    $form['group_assign']['groups'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Groups'),
      '#options' => $groups,
      '#default_value' => isset($import['options']['groups']) ? $import['options']['groups'] : array() ,
    );

    $form['group_assign']['existing'] = array(
      '#type' => 'fieldset',
      '#title' => t('Existing Users'),
      '#description' => t('Add to selected groups users who already have an account. Optionaly send them an email.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['group_assign']['existing']['existing_og_subscribe'] = array(
      '#type' => 'radios',
      '#title' => t('Subscribe'),
      '#default_value' => $import['options']['existing_og_subscribe'],
      '#options' => array(t('No'), t('Yes')),
    );

    $form['group_assign']['existing']['existing_og_subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Message Subject'),
      '#default_value' => $import['options']['existing_og_subject'],
      '#description' => t('Customize the subject of the email sent to existing users being added to groups.') .' '. t('Available variables are:') .' !username, !site, !uri, !uri_brief, !mailto, !date, !login_uri, !edit_uri, !login_url'. $profile_string .'.',
    );

    $form['group_assign']['existing']['existing_og_markup'] = array(
      '#type' => 'radios',
      '#title' => t('Email Format'),
      '#default_value' => empty($import['options']['existing_og_markup']) ? 0 : $import['options']['existing_og_markup'],
      '#options' => array(t('Plain Text'), t('HTML')),
    );

    $form['group_assign']['existing']['existing_og_message'] = array(
      '#type' => 'textarea',
      '#title' => t('Email Body'),
      '#default_value' => $import['options']['existing_og_message'],
      '#description' => t('Message to send existing users being added to groups. Users who are already in a group will not be sent a message.') .' '. t('Available variables are:') .' !username, !site, !uri, !uri_brief, !mailto, !date, !login_uri, !edit_uri, !login_url'. $profile_string .'.',
    );

    $form['group_assign']['existing']['existing_og_css'] = array(
      '#type' => 'textarea',
      '#title' => t('Email CSS'),
      '#default_value' => $import['options']['existing_og_css'],
      '#description' => t('Use if sending HTML formated email.'),
    );

  }

  return $form;
}


/**
 * Implementation of hook_user_import_pre_save().
 */
function og_user_import_pre_save($settings, $account, $password, $fields, $errors) {

  // user already exists - add to group?
  if (in_array('duplicate email', $errors)) og_user_import_group_add($settings, $email, $fields);

  return;
}

/**
 * Implementation of hook_user_import_after_save().
 */
function og_user_import_after_save($settings, $account, $password, $fields) {

  $groups_selected = $settings['options']['groups'];
  $uid = $account->uid;

  $organic_groups = user_import_get_groups();
  if (empty($organic_groups)) return;

  $og_args = array('is_active' => 1, 'is_admin' => 0, 'mail_type' => 0, 'created' => time());

  while (list($gid, $title) = each($organic_groups)) {

      if (!empty($groups_selected[$gid])) og_save_subscription($gid, $uid, $og_args);
  }

  return;
}


// add user to groups if already a member
function og_user_import_group_add($settings, $email, $fields) {

  if ($settings['setting'] != 'import') return;

  $account = user_load(array('mail' => $email));
  if (!empty($settings['options']['existing_og_message'])) $profile = user_import_profile_load($account);

  $groups_subscribed = og_get_subscriptions($account->uid);
  $og_args = array('is_active' => 1, 'is_admin' => 0, 'mail_type' => 0, 'created' => time());

  foreach ($settings['options']['groups'] as $group) {

    // is user member of group?
    if (!empty($group) && empty($groups_subscribed[$group])) {

      // subscribe to group
      og_save_subscription($group, $account->uid, $og_args);

      if (!empty($settings['options']['existing_og_message'])) _user_import_send_email($account, $password, $profile, $settings['options']['existing_og_subject'], $settings['options']['existing_og_message'], $settings['options']['existing_og_markup'], $settings['options']['existing_og_css'], $settings['options']['subscribed']);

    }
  }

  return;
}
