<?php
// $Id: cck_facets.inc,v 1.4 2008/02/28 21:13:10 davidlesieur Exp $

/**
 * @file
 * Provides an API for modules to expose facets based on CCK fields.
 */

require_once('./'. drupal_get_path('module', 'faceted_search') .'/faceted_search.inc');

/**
 * A base class for facets for CCK fields.
 *
 * @see cck_facet_category
 */
class cck_facet extends faceted_search_facet {
  /**
   * The field corresponding to this facet.
   */
  var $_field = NULL;

  /**
   * Constructor. 
   *
   * @param $field
   *   The field corresponding to this facet.
   * @param $active_path
   *   Array representing the path leading to the active category, including the
   *   active category itself. Defaults to an empty array, meaning no active
   *   category.
   */
  function cck_facet($field, $active_path = array()) {
    $this->_field = &$field;
    parent::faceted_search_facet($field['field_name'], $active_path);
  }

  /**
   * Return the id of this facet.
   */
  function get_id() {
    return 1;
  }

  /**
   * Return the label of this facet. The implementor is responsible to ensure
   * adequate security filtering.
   */
  function get_label() {
    if (isset($this->_field['widget']['label'])) {
      return check_plain($this->_field['widget']['label']);
    }
    else {
      return check_plain($this->_field['field_name']);
    }
  }

  /**
   * Return the search text for this facet, taking into account this facet's
   * active path.
   */
  function get_text() {
    if ($category = $this->get_active_category()) {
      return $category->_value;
    }
    return '';
  }
  
  /**
   * Return the field name to help site administrators identify fields.
   */
  function get_help() {
    return $this->_field['type_name'];
  }
  
  /**
   * Returns the available sort options for this facet.
   */
  function get_sort_options() {
    $options = parent::get_sort_options();
    if (isset($this->_field['widget']['label'])) {
      $label = $this->_field['widget']['label'];
    }
    else {
      $label = $this->_field['field_name'];
    }
    $options['field'] = check_plain($label);
    return $options;
  }

  /**
   * Handler for the 'count' sort criteria.
   */
  function build_sort_query_count(&$query) {
    $query->add_orderby('count', 'DESC');
    $query->add_orderby($this->_field['field_name'], 'ASC');
  }

  /**
   * Handler for the 'field' sort criteria.
   */
  function build_sort_query_field(&$query) {
    $query->add_orderby($this->_field['field_name'], 'ASC');
  }

  /**
   * Updates a query for retrieving the root categories of this facet and their
   * associated nodes within the current search results. 
   *
   * @param $query
   *   The query object to update.
   * @return
   *   FALSE if this facet can't have root categories.
   */
  function build_root_categories_query(&$query) {
    $db_info = _cck_facets_db_info($this->_field);
    $query->add_table($db_info['table'], 'vid', 'n', 'vid');
    $main_column = array_shift($db_info['columns']);
    $query->add_field($db_info['table'], $main_column['column'], $this->_field['field_name']);
    if ($main_column['type'] == 'longtext' || $main_column['type'] == 'varchar') {
      // Reject empty text.
      $query->add_where($db_info['table'] .'.'. $main_column['column'] ." != ''"); 
    }
    else {
      // Reject null value.
      $query->add_where($db_info['table'] .'.'. $main_column['column'] ." IS NOT NULL");
    }
    $query->add_groupby($this->_field['field_name']);
    return TRUE;
  }

  /**
   * This factory method creates categories given query results that include the
   * fields selected in get_root_categories_query() or get_subcategories_query().
   *
   * @param $results
   *   $results A database query result resource.
   * @return
   *   Array of categories.
   */
  function build_categories($results) {
    $categories = array();
    while ($result = db_fetch_object($results)) {
      $categories[] = new cck_facet_category($this->_field, $result->{$this->_field['field_name']}, $result->count);
    }
    return $categories;
  }
}

/**
 * A base class for facet categories for CCK fields. 
 *
 * @see cck_facet
 */
class cck_facet_category extends faceted_search_category {
  /**
   * The field corresponding to this category's parent facet.
   */
  var $_field = NULL;
  
  /**
   * Value corresponding to this category.
   */
  var $_value = NULL;

  /**
   * Constructor.
   */
  function cck_facet_category($field, $value, $count = NULL) {
    parent::faceted_search_category($count);
    $this->_field = &$field;
    $this->_value = $value;
  }

  /**
   * Return the label of this category.
   *
   * @param $html
   *   TRUE when HTML is allowed in the label, FALSE otherwise. Checking this
   *   flag allows implementors to provide a rich-text label if desired, and an
   *   alternate plain text version for cases where HTML cannot be used. The
   *   implementor is responsible to ensure adequate security filtering.
   */
  function get_label($html = FALSE) {
    $db_info = _cck_facets_db_info($this->_field);
    $label = content_format($this->_field['field_name'], array(key($db_info['columns']) => $this->_value));

    // Note: The label is already filtered by the CCK formatter and does not
    // need to be filtered here.
    return $html ? $label : strip_tags($label);
  }

  /**
   * Updates a query for selecting nodes matching this category.
   *
   * @param $query
   *   The query object to update.
   */
  function build_results_query(&$query) {
    $db_info = _cck_facets_db_info($this->_field);
    $main_column = array_shift($db_info['columns']);
    // Since the same table might be joined multiple times in the query, we use
    // the field's name as the table alias to create a unique table reference.
    $table = $query->add_table($db_info['table'], 'vid', 'n', 'vid', $this->_field['field_name']);
    switch($main_column['type']) {
      case 'int':
      case 'mediumint':
      case 'tinyint':
      case 'bigint':
        $placeholder = '%d';
        break;
      case 'float':
        $placeholder = '%f';
        break;
      default:
        $placeholder = "'%s'";
    }
    $query->add_where($table .'.'. $main_column['column'] .' = '. $placeholder, $this->_value);
  }  
}

