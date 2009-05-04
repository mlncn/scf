<?php
/*
	Set which vocab to use; else use whatever vocab is set for child (scf_evidence )
*/
function scf_targets_settings() {
	$vocabs = taxonomy_get_vocabularies();
	foreach ($vocabs as $vocab) {
		$all_vocabs[$vocab->vid] =   $vocab->name ; 
	}
	$form['vocabs'] = array(
	  '#type' => 'checkboxes',
	  '#title' => t('Vocabularies'),
	  '#options' => $all_vocabs,
	  '#default_value' => variable_get('scf_targets_vocabs',array()), 
  	  '#description' => t('Vocabularies to use on target page to for the tree. If none selected defaults to evidence node\'s vocabularies. Please make sure that these vocabularies are set for evidence. '),
	);
	
 	$form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
	return $form; 
}

function scf_targets_settings_submit($form, &$form_state) {
	 variable_set('scf_targets_vocabs',array_filter($form_state['values']['vocabs']));
}