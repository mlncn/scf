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
 * Implementation of hook_help().
 */
function form_tweak_ui_help($path) {
  switch ($path) {
    case 'admin/help#form_tweak_ui':
      return '<p>' . t('Form tweak offers the following ') . '</p>';
  }
}

/**
 * Implementation of hook_form_alter().
 */
function form_tweak_ui_form_alter(&$form, &$form_state, $form_id) {
  if (isset($form['type']) && isset($form['#node'])
    && ($form_id == $form['type']['#value'] .'_node_form') 
    ) {
    // we are on a node edit or add form 
    ///////////////////////////////////////////////////
    // Rename any submit buttons, default for all forms.
    // leave out translation for now
    ///////////////////////////////////////////////////
    $buttons = fs_variables_load('form_tweak', 'buttons');
    drupal_set_message('<pre>'.var_export($buttons,TRUE).'</pre>');
    foreach ($buttons as $button => $value) {
      if (isset($form['buttons'][$button])) {
        $form['buttons'][$button]['#value'] = $value;
      }
    }
  }
  // 
}

