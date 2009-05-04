<?php
// $Id: $

/**
 * @file
 * Import watchdog properties.
 */

/**
 * Implementation of hook_user_import_after_save().
 */
function watchdog_user_import_after_save($settings, $account, $password, $fields) {

  watchdog('user', 'New user: %name %email.', array('%name' => theme('placeholder', check_plain($account->name)), '%email' => theme('placeholder', '<'. check_plain($account->mail) .'>')), WATCHDOG_NOTICE, l(t('edit'), 'user/'. $account->uid .'/edit'));

  return;
}