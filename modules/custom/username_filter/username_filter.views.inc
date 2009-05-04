<?php

function username_filter_views_data() {
	$data['users']['partial_username'] = array(
	  'real field' => 'uid',
	  'title' => t('Partial Username'),
	  'help' => t('Filter view based on a partial filter.'),
	  'filter' => array(
	    'handler' => 'views_handler_filter_user_name_partial',
	  ),
	);

	return $data;
}

function username_filter_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'username_filter'),
    ),
    'handlers' => array(
      'views_handler_filter_user_name_partial' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}
