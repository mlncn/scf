<?php
// $Id: taxonomy_tree.admin.inc,v 1.2 2008/12/12 21:54:10 darthclue Exp $

/**
 * @file
 * Administrative page callbacks for the taxonomy DHTML module.
 */

/**
 * Form Builder to manage settings for taxonomy DHTML.
 *
 * @ingroup forms
 * @see taxonomy_tree_admin_submit()
 * @see theme_taxonomy_tree_admin()
 */
function taxonomy_tree_admin() {
  $vocabularies = taxonomy_tree_get_vocabularies();
  $form = array('#tree' => TRUE);
  foreach ($vocabularies as $vocabulary) {
    $types = array();
    $form[$vocabulary->vid]['#vocabulary'] = (array)$vocabulary;
    $form[$vocabulary->vid]['name'] = array('#value' => check_plain($vocabulary->name));
    $form[$vocabulary->vid]['noi'] = array('#type' => 'textfield', '#size' => 3, '#maxlength' => 3, '#default_value' => (is_null($vocabulary->noi) ? 25 : $vocabulary->noi));
    $form[$vocabulary->vid]['exposeBlock'] = array('#type' => 'checkbox', '#default_value' => is_null($vocabulary->expblock) ? FALSE : $vocabulary->expblock);
    $form[$vocabulary->vid]['count'] = array('#type' => 'select', '#default_value' => $vocabulary->count, '#options'=>array('0'=>'Default','1'=>'Immidiate term count','2'=>'No Count'));
    $form[$vocabulary->vid]['depth'] = array('#type' => 'select', '#default_value' => is_null($vocabulary->depth) ? 0 : $vocabulary->depth, '#options' => array('0' => t('None'), '1' => '1', '2' => '2', '3' => '3', '4' => '4', '5' => '5', '6' => '6', '7' => '7', '8' => '8', '9' => '9', '10' => 'All'));
  }
  $form['overview_title'] = array(
  		'#title' => 'Overview page title',
  		'#type'=>'textfield',
  		'#required'=>TRUE,
  		'#default_value' => variable_get('taxonomy_tree_overview_title', 'Taxonomy Tree'),
  		'#description' => 'You may need to clear cache before this takes effect.'
  );
  
  if (module_exists('workflow')) {
  	$states = workflow_get_states();
  	
  	$form['workflow_excludes'] = array(
  		'#type'=>'checkboxes',
  		'#options'=>$states,
  		'#title'=> 'Workflow states to exclude from count',
  		'#default_value'=> variable_get('taxonomy_tree_workflow',array())
  	);
  }
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

/**
 * Theme the taxonomy dhtml admin as a sortable list of vocabularies.
 *
 * @ingroup themeable
 * @see taxonomy_tree_admin()
 */
function theme_taxonomy_tree_admin($form) {
  $rows = array();
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['name'])) {
      $vocabulary = &$form[$key];

      $row = array();
      $row[] = drupal_render($vocabulary['name']);
      $row[] = drupal_render($vocabulary['noi']);
      $row[] = drupal_render($vocabulary['depth']);
      $row[] = drupal_render($vocabulary['exposeBlock']);
      $row[] = drupal_render($vocabulary['count']);
      $rows[] = array('data' => $row, 'class' => 'draggable');
    }
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No vocabularies available.'), 'colspan' => '3'));
  }

  $header = array(t('Name'), t('Number of items to display'), t('Depth'), t('Show on page'), t('Count type'));
  return theme('table', $header, $rows, array('id' => 'taxonomy')) . drupal_render($form);
}

/**
 * Submit handler for the taxonomy dhtml settings form.
 *
 *
 * @see taxonomy_tree_admin()
 */
function taxonomy_tree_admin_submit($form, &$form_state) {
  foreach (array_keys($form_state['values']) as $ak) {
    if (is_array($form_state['values'][$ak])) {
     /*
      switch($form_state['values'][$ak]['count']) {
      	case 'none':
      		$form_state['values'][$ak]['noNumbers'] = true;
      		$form_state['values'][$ak]['minNumbers'] = false;
      		break;
      	case 'term':
      		$form_state['values'][$ak]['noNumbers'] = false;
      		$form_state['values'][$ak]['minNumbers'] = true;
      		break;	
      	default:
      		$form_state['values'][$ak]['noNumbers'] = false;
      		$form_state['values'][$ak]['minNumbers'] = false;
      		break;
      }
      */
      
     

      db_query("DELETE FROM {taxonomy_tree} WHERE vid=%d", $ak);
      db_query("INSERT INTO {taxonomy_tree} (vid, noi, depth, expblock, count) VALUES (%d, %d, %d, %d, %d)", $ak, $form_state['values'][$ak]['noi'], $form_state['values'][$ak]['depth'], $form_state['values'][$ak]['exposeBlock'], $form_state['values'][$ak]['count']);
    }
  }
  variable_set('taxonomy_tree_workflow',array_keys(array_filter($form_state['values']['workflow_excludes'])));
  variable_set('taxonomy_tree_overview_title', $form_state['values']['overview_title']);
  cache_clear_all();
  drupal_set_message('Taxonomy DHTML settings saved.');
}
