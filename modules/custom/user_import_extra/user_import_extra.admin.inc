<?php
/*
	Warning: this will reset users passwords
*/
function user_import_extra_mailblocked() {
	$in_uids = user_import_extra_get_all_uids(); 
	$uids = array();
	foreach ($in_uids as $us) {
		$uids[$us->uid] =   l($us->name,'user/'.$us->uid) ; 
	}
	$form['help'] = array(
		'#type' => 'markup',
		'#value'=>'This lists users that have been imported via user import with "temporarily block" checked.<br/>Use to email them their account details.'
	 );
	$form['uids'] = array(
	  '#type' => 'checkboxes',
	  '#title' => t('Users'),
	  '#options' => $uids,
	  '#default_value' => array_keys($uids), 
  	'#description' => t('Users to mail (uses default mail settings and resets the password). When using this settings users that have not yet been processed still appear on this form.'),
	);
	$form['cron'] = array(
	  '#type' => 'checkbox',
	  '#title' => t('Use user import CRON settings to stagger mails'),
	);
 	$form['submit'] = array('#type' => 'submit', '#value' => t('Mail'));
	return $form; 
}
function user_import_extra_mailblocked_validate($form, &$form_state) {
	
}
function user_import_extra_mailblocked_submit($form, &$form_state) {
	$uids =  $form_state['values']['uids'];
	$uids_process= user_import_extra_get_process_uids($uids,$form_state['values']['cron']);
	if ($uids_process) user_import_extra_process($uids_process);
}

function user_import_sendmail() {
	module_load_include('inc','user_import','supported/user_import');
	$uid = arg(2);
	if (is_numeric($uid) && user_import_extra_check_uids($uid)) {
		user_import_extra_mail_single($uid);
		return 'Mail sent.';
	}
	return 'User not found, mail not sent.';
}
