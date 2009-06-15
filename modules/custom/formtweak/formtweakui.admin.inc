<?php

/**
 * @file
 *  Having a file that loaded only for people who have permission to tweak 
 * forms didn't do anything getting included within a form_tweak_form_alter,
 * and since everything can be edited in config files anyway, it made a lot of
 * sense to make the user interface for tweaking forms a separate module.
 *
 * But we will still have most of that module only load for privileged users.
 */

/**
 * Implementation of hook_menu().
 */
function formtweakui_menu() {
  $items = array();
  $items['admin/settings/formtweak'] = array(
    'title' => t('Form tweak configuration'),
    'description' => t('Tweak your forms'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('fs_raw_config_form', 'form_tweak', ''),
    'access arguments' => array('tweak forms'),
    'type' => MENU_CALLBACK,
  );
}

/**
 * Implementation of hook_form_alter().
 */
function formtweakui_form_alter(&$form, &$form_state, $form_id) {
  if (isset($form['type']) && isset($form['#node'])
    && ($form_id == $form['type']['#value'] .'_node_form')
    ) {
    // we are on a node edit or add form 
    $buttons = $form['buttons'];
    drupal_set_message('<pre>'.var_export($form,TRUE).'</pre>');
    foreach ($buttons as $button => $value) {
      // $name = $value['#value'];
      $form['buttons'][$button]['#prefix'] = $button;
    }
  }
  // 
}
