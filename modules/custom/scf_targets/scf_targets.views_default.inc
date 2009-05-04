<?php

/**
 * Implementation of hook_views_default_views().
 */
function scf_targets_views_default_views() {
  $view = new view;
  $view->name = 'scf_targets_nodes';
  $view->description = 'Gets the nodes connected to a node based on term, node reference ID';
  $view->tag = 'scf';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'field_scf_target_node_nid' => array(
      'label' => 'scf target node reference',
      'required' => 1,
      'delta' => -1,
      'id' => 'field_scf_target_node_nid',
      'table' => 'node_data_field_scf_target_node',
      'field' => 'field_scf_target_node_nid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Title',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'type' => array(
      'label' => 'Type',
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'not found',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'node',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'field_scf_target_node_nid',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'biblio' => 0,
        'article' => 0,
        'commentary' => 0,
        'ct_1' => 0,
        'ct_2' => 0,
        'discussion' => 0,
        'event' => 0,
        'feed' => 0,
        'framework_term' => 0,
        'grant' => 0,
        'news' => 0,
        'news_item' => 0,
        'page' => 0,
        'profile' => 0,
        'question' => 0,
        'workgroup' => 0,
      ),
      'validate_argument_node_access' => 1,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '1' => 0,
        '5' => 0,
        '6' => 0,
        '2' => 0,
        '3' => 0,
        '4' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_is_member' => 0,
      'validate_argument_php' => '',
    ),
    'tid' => array(
      'default_action' => 'not found',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'taxonomy_term',
      'validate_fail' => 'not found',
      'break_phrase' => 1,
      'add_table' => 0,
      'require_value' => 0,
      'reduce_duplicates' => 1,
      'set_breadcrumb' => 0,
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'biblio' => 0,
        'article' => 0,
        'commentary' => 0,
        'ct_1' => 0,
        'ct_2' => 0,
        'discussion' => 0,
        'event' => 0,
        'feed' => 0,
        'framework_term' => 0,
        'grant' => 0,
        'news' => 0,
        'news_item' => 0,
        'page' => 0,
        'profile' => 0,
        'question' => 0,
        'workgroup' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '1' => 0,
        '5' => 0,
        '6' => 0,
        '2' => 0,
        '3' => 0,
        '4' => 0,
      ),
      'validate_argument_type' => 'tids',
      'validate_argument_is_member' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $views[$view->name] = $view;

  return $views;
}


