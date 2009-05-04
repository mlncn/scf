<?php

function scf_targets_get_target() {
$content[type]  = array (
  'name' => 'Target',
  'type' => 'scf_target',
  'description' => '',
  'title_label' => 'Title',
  'body_label' => 'Body',
  'min_word_count' => '0',
  'help' => '',
  'node_options' => 
  array (
    'status' => true,
    'promote' => true,
    'sticky' => false,
    'revision' => false,
  ),
  'save_as_draft' => '0',
  'upload' => '1',
  'notifications_node_ui' => 
  array (
    'links' => true,
    'form' => false,
    'comment' => false,
    'block' => false,
  ),
  'old_type' => 'scf_target',
  'orig_type' => '',
  'module' => 'node',
  'custom' => '1',
  'modified' => '1',
  'locked' => '0',
  'og_content_type_usage' => 'omitted',
  'content_profile_use' => 0,
  'rdf_schema_class' => '',
  'fivestar' => 0,
  'fivestar_stars' => 5,
  'fivestar_labels_enable' => 1,
  'fivestar_label_0' => 'Cancel rating',
  'fivestar_label_1' => 'Poor',
  'fivestar_label_2' => 'Okay',
  'fivestar_label_3' => 'Good',
  'fivestar_label_4' => 'Great',
  'fivestar_label_5' => 'Awesome',
  'fivestar_label_6' => 'Give it @star/@count',
  'fivestar_label_7' => 'Give it @star/@count',
  'fivestar_label_8' => 'Give it @star/@count',
  'fivestar_label_9' => 'Give it @star/@count',
  'fivestar_label_10' => 'Give it @star/@count',
  'fivestar_style' => 'average',
  'fivestar_text' => 'dual',
  'fivestar_title' => 1,
  'fivestar_feedback' => 1,
  'fivestar_unvote' => 0,
  'fivestar_position_teaser' => 'hidden',
  'fivestar_position' => 'below',
  'feedapi' => 
  array (
    'enabled' => 0,
    'refresh_on_create' => 0,
    'update_existing' => 1,
    'skip' => 0,
    'items_delete' => '0',
    'parsers' => 
    array (
      'parser_simplepie' => 
      array (
        'enabled' => 0,
        'weight' => '0',
      ),
      'parser_common_syndication' => 
      array (
        'enabled' => 0,
        'weight' => '0',
      ),
    ),
    'processors' => 
    array (
      'feedapi_node' => 
      array (
        'enabled' => 0,
        'weight' => '0',
        'content_type' => 'biblio',
        'node_date' => 'feed',
        'promote' => '3',
        'x_dedupe' => '0',
      ),
      'feedapi_inherit' => 
      array (
        'enabled' => 0,
        'weight' => '0',
        'inherit_og' => 1,
        'inherit_taxonomy' => 1,
      ),
    ),
  ),
);
$content[fields]  = array (
  0 => 
  array (
    'label' => 'Rationale',
    'field_name' => 'field_scf_target_rationale',
    'type' => 'text',
    'widget_type' => 'text_textarea',
    'change' => 'Change basic information',
    'weight' => '31',
    'rows' => '5',
    'size' => 60,
    'description' => '',
    'default_value' => 
    array (
      0 => 
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_scf_target_rationale][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' => NULL,
    'group' => false,
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' => 
    array (
      'value' => 
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' => 
    array (
      'label' => 
      array (
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      4 => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      2 => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      3 => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
  ),
);
$content[extra]  = array (
  'title' => '-5',
  'body_field' => '0',
  'menu' => '-2',
  'taxonomy' => '-3',
  'attachments' => '30',
);
	return $content; 

}
function scf_targets_get_evidence() {
$content[type]  = array (
  'name' => 'Evidence',
  'type' => 'scf_evidence',
  'description' => '',
  'title_label' => 'Title',
  'body_label' => 'Body',
  'min_word_count' => '0',
  'help' => '',
  'node_options' => 
  array (
    'status' => true,
    'promote' => true,
    'sticky' => false,
    'revision' => false,
  ),
  'save_as_draft' => '0',
  'upload' => '1',
  'notifications_node_ui' => 
  array (
    'links' => true,
    'form' => false,
    'comment' => false,
    'block' => false,
  ),
  'old_type' => 'scf_evidence',
  'orig_type' => '',
  'module' => 'node',
  'custom' => '1',
  'modified' => '1',
  'locked' => '0',
  'og_content_type_usage' => 'omitted',
  'content_profile_use' => 0,
  'fivestar' => 0,
  'fivestar_stars' => 5,
  'fivestar_labels_enable' => 1,
  'fivestar_label_0' => 'Cancel rating',
  'fivestar_label_1' => 'Poor',
  'fivestar_label_2' => 'Okay',
  'fivestar_label_3' => 'Good',
  'fivestar_label_4' => 'Great',
  'fivestar_label_5' => 'Awesome',
  'fivestar_label_6' => 'Give it @star/@count',
  'fivestar_label_7' => 'Give it @star/@count',
  'fivestar_label_8' => 'Give it @star/@count',
  'fivestar_label_9' => 'Give it @star/@count',
  'fivestar_label_10' => 'Give it @star/@count',
  'fivestar_style' => 'average',
  'fivestar_text' => 'dual',
  'fivestar_title' => 1,
  'fivestar_feedback' => 1,
  'fivestar_unvote' => 0,
  'fivestar_position_teaser' => 'hidden',
  'fivestar_position' => 'below',
  'feedapi' => 
  array (
    'enabled' => 0,
    'refresh_on_create' => 0,
    'update_existing' => 1,
    'skip' => 0,
    'items_delete' => '0',
    'parsers' => 
    array (
      'parser_simplepie' => 
      array (
        'enabled' => 0,
        'weight' => '0',
      ),
      'parser_common_syndication' => 
      array (
        'enabled' => 0,
        'weight' => '0',
      ),
    ),
    'processors' => 
    array (
      'feedapi_node' => 
      array (
        'enabled' => 0,
        'weight' => '0',
        'content_type' => 'biblio',
        'node_date' => 'feed',
        'promote' => '3',
        'x_dedupe' => '0',
      ),
      'feedapi_inherit' => 
      array (
        'enabled' => 0,
        'weight' => '0',
        'inherit_og' => 1,
        'inherit_taxonomy' => 1,
      ),
    ),
  ),
);
$content[fields]  = array (
  0 => 
  array (
    'label' => 'Target',
    'field_name' => 'field_scf_target_node',
    'type' => 'nodereference',
    'widget_type' => 'nodereference_url',
    'change' => 'Change basic information',
    'weight' => '32',
    'fallback' => 'page_not_found',
    'node_link' => 
    array (
      'enabled' => 0,
      'title' => '',
      'hover_title' => '',
    ),
    'description' => '',
    'group' => false,
    'required' => 1,
    'multiple' => '0',
    'referenceable_types' => 
    array (
      'scf_target' => 'scf_target',
      'article' => 0,
      'biblio' => 0,
      'commentary' => 0,
      'ct_1' => 0,
      'ct_2' => 0,
      'discussion' => 0,
      'event' => 0,
      'scf_evidence' => 0,
      'feed' => 0,
      'framework_term' => 0,
      'grant' => 0,
      'news_item' => 0,
      'news' => 0,
      'page' => 0,
      'profile' => 0,
      'question' => 0,
      'workgroup' => 0,
    ),
    'advanced_view' => '--',
    'advanced_view_args' => '',
    'op' => 'Save field settings',
    'module' => 'nodereference',
    'widget_module' => 'nodereference_url',
    'columns' => 
    array (
      'nid' => 
      array (
        'type' => 'int',
        'unsigned' => true,
        'not null' => false,
      ),
    ),
    'display_settings' => 
    array (
      'label' => 
      array (
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      4 => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      2 => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      3 => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
  ),
  1 => 
  array (
    'label' => 'Biblio Reference',
    'field_name' => 'field_scf_target_biblio',
    'type' => 'nodereference',
    'widget_type' => 'nodereference_autocomplete',
    'change' => 'Change basic information',
    'weight' => '33',
    'autocomplete_match' => 'contains',
    'description' => '',
    'default_value' => 
    array (
      0 => 
      array (
        'nid' => NULL,
        '_error_element' => 'default_value_widget][field_scf_target_biblio][0][nid][nid',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' => 
    array (
      'field_scf_target_biblio' => 
      array (
        0 => 
        array (
          'nid' => 
          array (
            'nid' => '',
            '_error_element' => 'default_value_widget][field_scf_target_biblio][0][nid][nid',
          ),
          '_error_element' => 'default_value_widget][field_scf_target_biblio][0][nid][nid',
        ),
      ),
    ),
    'group' => false,
    'required' => 0,
    'multiple' => '1',
    'referenceable_types' => 
    array (
      'biblio' => 'biblio',
      'article' => 0,
      'commentary' => 0,
      'discussion' => 0,
      'event' => 0,
      'scf_evidence' => 0,
      'feed' => 0,
      'framework_term' => 0,
      'grant' => 0,
      'news_item' => 0,
      'news' => 0,
      'page' => 0,
      'profile' => 0,
      'question' => 0,
      'scf_target' => 0,
      'workgroup' => 0,
    ),
    'advanced_view' => '--',
    'advanced_view_args' => '',
    'op' => 'Save field settings',
    'module' => 'nodereference',
    'widget_module' => 'nodereference',
    'columns' => 
    array (
      'nid' => 
      array (
        'type' => 'int',
        'unsigned' => true,
        'not null' => false,
      ),
    ),
    'display_settings' => 
    array (
      'label' => 
      array (
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      4 => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      2 => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      3 => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => 
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
  ),
);
$content[extra]  = array (
  'title' => '-5',
  'body_field' => '0',
  'menu' => '-2',
  'taxonomy' => '-3',
  'attachments' => '30',
);
return $content; 
}