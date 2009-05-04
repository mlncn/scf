<?php

/*
 * Menu Callback; execute node 
 */
function exec_node_process_file(){
	//Get node types
	$form['#attributes'] = array('enctype' => "multipart/form-data");
	
 	 $form['file_upload'] = array(
    '#type' => 'file',
    '#title' => 'Filename',
    '#description' => t('Tab deliminated file with field names (IE. Title, Body). When using this along with nat, an additional column named parent can be provided to indicate a tree like structure. Please order terms related to this tree, (IE, don\'t list a child before a parent).'),
 	 );
	$types = node_get_types();
	foreach ($types as $key=>$val) $content_types[$val->type] = $val->name;
	
	$form['content_type'] = array(
	  '#type' => 'select',
	  '#title' => t('Content type'),
	  '#options' => $content_types,
	  '#description' => t('Content type of nodes being imported'),
	);
	$form['default_term'] = array(
    '#type' => 'textfield',
    '#title' => 'Parent term ID',
    '#description'=>'This is usually not needed, however if required is turned on for vocabulary, then a default term needs to be provided.<Br/>This can also be used to populate a list under a given term. It will replace occurrences of parent 0/empty with this.  ',
  	);
	$vocabss = taxonomy_get_vocabularies();
	$vocabs[0] = 'Decide for me'; 
	foreach ($vocabss as $vocab) $vocabs[$vocab->vid] = $vocab->name; 
	$form['vocab'] = array(
	  '#type' => 'select',
	  '#title' => t('Vocabulary'),
	  '#options' => $vocabs,
	  '#description' => t('Vocabulary to use when importing NAT based taxonomy with parents. Use only when content types has more than 1 vocabulary set in NAT; it will pick the first vocabulary with default option "Decide for me".'),
	);
	//Form submit button
	$form['submit'] = array(
  			'#type' => 'submit',
  			'#value' => 'Submit',
		);
	
	
  	return $form;
}
function exec_node_process_file_validate($form, &$form_state) {

}


/*
 * exec_node_process_file Submit function
 
 */
function exec_node_process_file_submit($form, &$form_state) {
	//Read the file and use drupal exec to run form
	global $user;
	module_load_include('inc', 'node', 'node.pages');
	$dir = file_directory_path();
	if (module_exists('user_dir_create')) {
		$dir .='/'.user_dir_create_get_directory();
	}
  	$dir .= '/exec_node';
	if ( !is_dir($dir)) {
		mkdir($dir, 0777);
	}
	
    if( file_check_directory( $dir ) ) {
        $save = file_save_upload('file_upload', array(), $dir);
        if(!$save) {
            drupal_set_message('ERROR downloading the file to ' . $dir);
        } else {
            drupal_set_message('The file: <strong>"' . $save->filename . '"</strong> is successfully uploaded');
        }
       // variable_set('date_pic_font', $dir . '/' . $file);
    } else {
        drupal_set_message( t('WARNING: Server DIR is not accessible. Consult with site admin!', 'status') );
    }
  	
  	//Read the file and put return an array
  	$doc = exec_node_fileload_tds_format($save);
  	// process file and insert nodes with parent relations 
  	$content_type = $form_state['values']['content_type'];
  	$colNames = $doc['columnNames'];
  	$data = $doc['data'];
  	$content_form['values'] = array();
  	/*
  	* Drupal execute for forms needs some basic information supplied in an object
  	*/
  	$node = (object) array(
	    'uid' => (string) $user->uid,
	    'name' => (string) $user->name,
	    'type' => $content_type,
	    'language' => '',
	    'body' => NULL,
	    'title' => NULL,
	    'format' => NULL,
	    'status' => true,
	    'promote' => false,
	    'sticky' => false,
	    'created' => time(),
	    'revision' => false,
	    'comment' => '0',
	  );
	/*
	 *
	 *
	 */
	$nat_exists = module_exists('nat');
	if ($nat_exists) {
		$vid = $form_state['values']['content_type'];
		$nat_config = _nat_variable_get();
		$form_pid =$form_state['values']['default_term']; 
	  	$def_pid =$form_pid?$form_pid:1;
	  	if (!$vid || !in_array($vid,$nat_config['types'][$content_type])) $vid = array_shift($nat_config['types'][$content_type]) ;
	  	$vid_fake = $form_pid?$vid:-1;
	}
	if (!$vid) $vid = 0; 
	srand((float) microtime() * 10000000);
  	foreach ($data as $row) {
  		$content_form = array();
  		$content_form['values'] = array();
	  		
  		foreach ($colNames as $colName => $col){
  		 	if (strtolower($colName) != 'parent') {
	  		 	//$colArr = array_reverse(explode('__',$colName));
	  		 	//$tcol = $data[$col];
	  		 	//foreach ($colArr as $cola) $tcol[$cola] = $tcol;
	  		 	//$content_form['values'] = array_merge_recursive($content_form['values'], $tcol);
	  		 	$content_form['values'][strtolower($colName)] = $row[$col];
	  		 	if (strtolower($colName)=='title') $name=$row[$col];
  		 	} else {
  		 		if (!$nat_exists) continue ;
  		 			/*
  					* Get the parent if NAT is installed. If no parent, set to term 1. 
  					* It should be able to set to 0, but the field is required; nat oops?
  					* This only works if terms are listed in a somewhat logical order. 
  					*/
  		 		$parent = $row[$col];
  		 		if (!$parent) {
  		 			$pid = $def_pid ;
  		 			
  		 			$content_form['values']['taxonomy'][$vid_fake] = array($def_pid);

  		 			continue; 
  		 		}
  		 		$current_parents_copy = $current_parents;
  		 		$found = false; 
  		 		while (count($current_parents_copy)>0) {
  		 		/*
  		 		Check tree, starting from current parent till the terms match
  		 		*/
  		 			/*if (!$current_parents_copy[$pid]['tid']) {
  		 				break;
  		 			}*/
  		 			if (strtolower($current_parents_copy[$pid]['name']) == strtolower($parent)) {
  		 				$found = true;
  		 				break;
  		 			}
  		 			$pid_save = $current_parents_copy[$pid]['pid'] ;
  		 			unset($current_parents_copy[$pid]);
  		 			/*
  		 				This term does not match, remove it
  		 			*/
  		 			
  		 			$pid = $pid_save ;
  		 			if (!$pid || $pid == $def_pid && count($current_parents_copy)>0){
  		 				while(!$current_parents_copy[$pid]['tid']) $pid = array_rand($current_parents_copy) ;	 			
  		 			}
  		 			
  		 		}
  		 		/*
  		 			if no parent found, insert under 'root'
  		 		*/
  		 		if (!$found)$pid = $def_pid;
  		 		$content_form['values']['taxonomy'][$pid!=$def_pid?$vid:$vid_fake] = array($pid);
  		 	}
	 	}
	 	$content_form['values']['op']= 'Save';
	 	/*Make the node*/
	 	drupal_execute(trim($content_type).'_node_form', $content_form,$node);
	 	$nid = $content_form['nid'];
	 	
	 	// get the taxonomy term tid
	 	if ($nid && $nat_exists) {
	 		$term = nat_get_term($nid);
	 		$tid= $term->tid;
	 		$current_parents[$tid] = array('name'=>$name,'pid'=>$pid, 'tid'=>$tid);
	 		$pid = $tid; 
	 	}
	 	
	 	/*Output any form errors. */
	 	$errors = form_get_errors();
		if (count($errors)) {
	  		drupal_set_message('Sorry, issues in creating '.$content_type.'.','error');
	  		continue; 
	  	}
  	}
  	
}

/**
 * Read from TBS file
 * returns an array with two entries
 	columnNames is an array with coloumn name => to column id. 
 	data is arrays of datas, exploded on new line then further on tab
 
 	Use the two togeather to get data data[col number][ columnNames[column name]]
 */
function exec_node_fileload_tds_format($file) {
	ini_set('auto_detect_line_endings', true);
	$data_array = file($file->filepath, FILE_IGNORE_NEW_LINES );
	$colNames =  explode("\t",$data_array[0]);
	array_walk($colNames,'check_plain');
	$colNames = array_flip($colNames);
	for($i = 1; $i < count($data_array); $i++ ) {
			$test = trim(trim($data_array[$i],"\t"));
			if (!$test) continue;
			$datas[$i] = explode("\t",$data_array[$i]);	
			array_walk($datas[$i],'check_plain');
		}
	return array('columnNames'=>$colNames, 'data'=>$datas);
}