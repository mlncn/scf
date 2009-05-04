<?


/*
 * Menu Callback; block taxonomy admin settings form.
 */
function block_taxonomy_settings(){
	//Get node types
	$types = node_get_types();
	//Load settings into settings variable
	$settings = variable_get('block_taxonomy_block_content_types',array());
	if (!is_array($settings)) $settings =array();
	//Define variable that will hold checkbox default values
	$default = array();
	//For each of the types, create a checkbox
  	foreach($types as $type=>$type_object) {
  		//Set default for specific content type
  		if (in_array($type,$settings)) $default[] = $type;
  		//Setup option based on content type
  		$ops[$type]=$type;
	}
	
	//Create an element of checkboxes, one for each content type using the previously retrieved settings
	$form['block_content_types'] = array(
  	'#type' => 'checkboxes',
  	'#title' => t('Content Types to display block on'),
  	'#default_value' =>$default,
  		'#options' => $ops,
  		'#description' => t('Types that will contain the taxonomy block'),
	);
	$form['block_content_max'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Max'),
  	'#default_value' =>variable_get('block_taxonomy_block_content_max',3),
  	'#size'=>4,
  		'#description' => t('Max to display that are not in node\'s taxonomy (-1 to disable)'),
	);
	$form['block_links'] = array(
  	'#type' => 'checkbox',
  	'#title' => t('Minimize links'),
  	'#default_value' =>variable_get('block_taxonomy_block_links',1),
 	'#description' => t('Minimize the linked element; only link to relevant taxonomy'),
	);
	$form['block_menu'] = array(
  	'#type' => 'checkbox',
  	'#title' => t('Menu classes'),
  	'#default_value' =>variable_get('block_taxonomy_block_menu',1),
 	'#description' => t('Use drupal menu classes for ULs(dhtml functionality requires this)'),
	);
	
	$form['block_dhtml_menu'] = array(
  	'#type' => 'checkbox',
  	'#title' => t('Use DHTML functionality'),
  	'#default_value' =>variable_get('block_taxonomy_block_dhtml_menu',1),
 	'#description' => t('Include dhtml-menu classes so items can be expanded/contracted when dhtml menu is enabled. <br>'
 		.'<b>Note</b> Under dhtml menu settings, please do not use "Close menus when their parent menu is closed" with this option.'),
	);
	
	$form['block_retain_padding'] = array(
  	'#type' => 'checkbox',
  	'#title' => t('Retain padding'),
  	'#default_value' =>variable_get('block_taxonomy_block_retain_padding',0),
 	'#description' => t('Keep correct padding on visible items in relevent to hidden items. '),
	);
	$form['block_nat_term'] = array(
  	'#type' => 'checkbox',
  	'#title' => t('Emphasis NAT Terms at it\'s node'),
  	'#default_value' =>variable_get('block_taxonomy_block_nat_term',1),
 	'#description' => t('Emphasis NAT term in list if viewing node it\'s connected to.'),
	);
	$form['block_not_parent'] = array(
  	'#type' => 'checkbox',
  	'#title' => t('No emphasis on parents of NAT terms at it\'s node'),
  	'#default_value' =>variable_get('block_taxonomy_block_not_parent',1),
 	'#description' => t('When emphasizing NAT terms, don\'t emphasis immediate parent of term.'),
	);
	$form['block_nat_childsib'] = array(
  	'#type' => 'checkbox',
  	'#title' => t('Display children/sibling of term on NAT pages'),
  	'#default_value' =>variable_get('block_taxonomy_block_nat_childsib',1),
 	'#description' => t('When on a node page associated with term via NAT display children/sibling of that term.'),
	);	
	$vocabss = taxonomy_get_vocabularies();
	foreach ($vocabss as $vocab) $vocabs[$vocab->vid] = $vocab->name; 
	$form['vocabs'] = array(
	  '#type' => 'checkboxes',
	  '#title' => t('Vocabularies to display'),
	  '#default_value' => variable_get('block_taxonomy_vocabs', array_keys($vocabs)),
	  '#options' => $vocabs,
	  '#description' => t('Users with the <em>administer nodes</em> permission will be able to override these options.'),
	);
	//Form submit button
	$form['submit'] = array(
  			'#type' => 'submit',
  			'#value' => 'Submit',
		);
	 
	 
  	return $form;
}

function block_taxonomy_settings_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['block_content_max']) && $form_state['values']['block_content_max'] < 0 ) {
  	form_set_error('block_content_max','Max should be numeric and 0 or above');
  }
}
/*
 * Admin Setting Submit function
 */
function block_taxonomy_settings_submit($form, &$form_state) {
  //FIXME use form_state['value'] not form to populate, change $arr - not a good variable name. All variable names should hint what they hold.
  $types_to_use =array();
  $types =$form_state['values']['block_content_types'];
 if (is_array($types)) {
 	 foreach($types as $key => $val) {
	  	if ($val) {
	  		/*Add this to display block*/
	  		$types_to_use[]=$key;
	  	}
	  }
  }
  cache_clear_all();
  variable_set('block_taxonomy_block_content_max',$form_state['values']['block_content_max']);
  variable_set('block_taxonomy_block_content_types',$types_to_use);
  variable_set('block_taxonomy_block_links',$form_state['values']['block_links']);
  variable_set('block_taxonomy_block_menu',$form_state['values']['block_menu']);
  variable_set('block_taxonomy_block_dhtml_menu',$form_state['values']['block_dhtml_menu']);
  variable_set('block_taxonomy_block_retain_padding',$form_state['values']['block_retain_padding']);
  variable_set('block_taxonomy_block_nat_term',$form_state['values']['block_nat_term']);
  variable_set('block_taxonomy_block_not_parent',$form_state['values']['block_not_parent']);
  variable_set('block_taxonomy_block_nat_childsib',$form_state['values']['block_nat_childsib']);  
  variable_set('block_taxonomy_vocabs',$form_state['values']['vocabs']);
  drupal_set_message('block taxonomy saved');
}


?>