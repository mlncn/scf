<?php
// $Id$
/**
 * @file entrez.admin.inc
 * Provides Entrez' administrative interface.
 */

/**
 * Main Entrez settings.
 */
function entrez_admin_settings_form() {
  $form = array();
  // Update settings.
  $form['cron_update'] = array(
    '#type' => 'fieldset',
    '#title' => t('Automated update'),
    '#description' => t('Toggle to enable or disable automatic updates of all PubMed bibliographies.'),
    '#collapsible' => TRUE,
  );
  if (!variable_get('entrez_cron_update', 0)) {  // If zero
    $msg = t('Running automatic updates is currently <strong>disabled</strong>.');
    $button_cron_update = t('Enable cron update of PubMed bibliographies');
  }
  else {
    $msg = t('Running automatic updates currently <strong>enabled</strong>.');
    $button_cron_update = t('Suspend cron update of PubMed bibliographies');
  }
  $form['cron_update']['status'] = array(
    '#type' => 'markup',
    '#value' => '<p>' . $msg . '</p>',
  );
  $form['cron_update']['button_cron_update'] = array(
    '#type' => 'submit',
    '#value' => $button_cron_update,
  );
  // Import settings.
  $form['cron_import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Automated import'),
    '#description' => t('Toggle to enable or disable automatic nightly runs of the search query defined at !link.',
      array('!link' => l(t('Import by search terms'), 'admin/content/entrez/search_import'))),
    '#collapsible' => TRUE,
  );
  if (!variable_get('entrez_cron_import', 0)) {  // If zero
    $msg = t('Running the search query on cron is currently <strong>disabled</strong>.');
    $button_cron_import = t('Enable cron import from PubMed');
  }
  else {
    $msg = t('Running the search query on cron is currently <strong>enabled</strong>.');
    $button_cron_import = t('Suspend cron import from PubMed');
  }
  $form['cron_import']['status'] = array(
    '#type' => 'markup',
    '#value' => '<p>' . $msg . '</p>',
  );
  $form['cron_import']['button_cron_import'] = array(
    '#type' => 'submit',
    '#value' => $button_cron_import,
  );
  if ($query = entrez_get_search_query()) {
    $query = check_plain($query);
  }
  else {
    $query = t('No search query defined.');
  }
  $form['cron_import']['entrez_query_current'] = array(
    '#type' => 'markup',
    '#value' => '<p>' . t('The current query:') . '<br />' .
      '<code>' . $query . '</code></p>',
  );
  return $form;
}

/**
 * Implementation of submit hook for entrez_admin_settings_form.
 */
function entrez_admin_settings_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#parents'][0] == 'button_cron_import') { 
    // We implement this as a toggle so the text of the button can change.
    if (!variable_get('entrez_cron_import', 0)) {
      $val = 1;
      $msg = t('Cron import enabled.');
    }
    else {
      $val = 0;
      $msg = t('Cron import disabled.');
    }
    variable_set('entrez_cron_import', $val);
    drupal_set_message($msg);
  }
  if ($form_state['clicked_button']['#parents'][0] == 'button_cron_update') { 
    // We implement this as a toggle so the text of the button can change.
    if (!variable_get('entrez_cron_update', 0)) {
      $val = 1;
      $msg = t('Enabled cron update of PubMed bibliographies.');
    }
    else {
      $val = 0;
      $msg = t('Disabled cron update of PubMed bibliographies.');
    }
    variable_set('entrez_cron_update', $val);
    drupal_set_message($msg);
  }
}
