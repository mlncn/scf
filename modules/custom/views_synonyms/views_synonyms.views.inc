<?php

function views_synonyms_views_data() {
  $data['term_synonym']['names'] = array(
    'field' => array(
      'title' => t('Term Synonyms'),
      'help' => t('All term Synonyms'),
      'handler' => 'views_handler_field_term_synonym_names',
    ),
  );
  return $data;
}

function views_synonyms_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_synonyms'),
    ),
    'handlers' => array(
		'views_handler_field_term_synonym_names' => array(
        	'parent' => 'views_handler_field_prerender_list',
      ),
    ),
  );
}
