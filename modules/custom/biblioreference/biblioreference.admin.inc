<?php
// $Id$

/**
 * @file
 * Admin page callbacks for the biblioreference module.
 *
 * Much of this is borrowed from search modules's search.admin.inc.
 * Originally coded by Benjamin Melançon of Agaric Design Collective.
 */

/**
 * Menu callback: confirm wiping of the index.
 */
function biblioreference_wipe_confirm() {
  return confirm_form(array(),
    t('Are you sure you want to re-index bibliography content?'),
    'admin/settings/biblio/biblioreference',
    t('The index will be cleared.  Biblioreference autocomplete may not work on all content until re-indexing is complete, which may take more than one cron run.  This action cannot be undone.'),
    t('Re-index biblio content'),
    t('Cancel'));
}

/**
 * Handler for wipe the biblio index confirmation.
 */
function biblioreference_wipe_confirm_submit(&$form, &$form_state) {
  if ($form['confirm']) {
    biblioreference_index_wipe();
    drupal_set_message(t('The index will be rebuilt.'));
    $form_state['redirect'] = 'admin/settings/biblio/biblioreference';
    return;
  }
}

/**
 * Menu callback; displays settings for biblioreference module.
 *
 * @ingroup forms
 * @see system_settings_form()
 * @see search_admin_settings_validate()
 */
function biblioreference_settings_form() {
  $form = array();
  $sql = 'SELECT COUNT(*) FROM {node} JOIN {biblio} AS b USING (nid) '
       . 'WHERE b.nid NOT IN '
       . '(SELECT bk.nid FROM {biblioreference_keyword} AS bk)';
  $count = db_result(db_query($sql));
  $status = '<p><strong>' . t('%count bibliographies remain to be indexed.',
    array('%count' => $count)) . '</strong></p>';
  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Indexing status'),
  );
  $form['status']['status'] = array(
    '#value' => $status,
  );
  $form['status']['description'] = array(  // type 'markup' by default
    '#value' => '<p>' . t('Re-indexing bibliography content should not be necessary in typical operation of your site.  Only clear the index for contingencies such as an import of content that will have back-dated timestamps for their last change.') . '</p>',
  );
  $form['status']['wipe'] = array(
    '#type' => 'submit',
    '#value' => t('Re-index biblio content'),
  );

  // Indexing throttle:
  $items = drupal_map_assoc(array(10, 20, 50, 100, 200, 500));
  $form['indexing_throttle'] = array(
    '#type' => 'fieldset',
    '#title' => t('Indexing throttle')
  );
  $form['indexing_throttle']['biblioreference_cron_limit'] = array(
    '#type' => 'select',
    '#title' => t('Number of items to index per cron run'),
    '#default_value' => variable_get('biblioreference_cron_limit', 100),
    '#options' => $items,
    '#description' => t('The maximum number of items indexed in each pass of a <a href="@cron">cron maintenance task</a>. If necessary, reduce the number of items to prevent timeouts and memory errors while indexing.', array('@cron' => url('admin/reports/status')))
  );
  return system_settings_form($form);
}

/**
 * Validate callback.
 */
function biblioreference_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Re-index biblio content')) {
    drupal_goto('admin/settings/biblio/biblioreference/wipe');
  }
}
