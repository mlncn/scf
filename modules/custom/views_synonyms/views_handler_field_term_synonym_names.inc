<?php
// $Id: views_handler_field_term_node_tid.inc,v 1.2 2008/09/22 18:13:51 merlinofchaos Exp $

/**
 * Field handler for terms.
 */
class views_handler_field_term_synonym_names extends views_handler_field_prerender_list {
  function init(&$view, $options) {
    parent::init($view, $options);
    if ($view->base_table == 'node_revisions') {
      $this->additional_fields['vid'] = array('table' => 'node_revisions', 'field' => 'vid');
    }
    else {
      $this->additional_fields['vid'] = array('table' => 'node', 'field' => 'vid');
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_taxonomy'] = array('default' => TRUE);
    $options['limit'] = array('default' => FALSE);
    $options['vids'] = array('default' => array());
    $options['one_syn'] = array('default' => TRUE);
    $options['fallback_term'] = array('default' => TRUE);
    return $options;
  }

  /**
   * Provide "link to term" option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_taxonomy'] = array(
      '#title' => t('Link this field to its term page'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy']),
    );
	$form['fallback_term'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use Term Name when no Synonyms'),
      '#default_value'=> $this->options['fallback_term'],
    );
    $form['one_syn'] = array(
      '#type' => 'checkbox',
      '#title' => t('One synonym per term'),
      '#default_value'=> $this->options['one_syn'],
    );
    $form['limit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Limit terms by vocabulary'),
      '#default_value'=> $this->options['limit'],
    );
	 
    $options = array();
    $vocabularies = taxonomy_get_vocabularies();
    foreach ($vocabularies as $voc) {
      $options[$voc->vid] = check_plain($voc->name);
    }

    $form['vids'] = array(
      '#prefix' => '<div><div id="edit-options-vids">',
      '#suffix' => '</div></div>',
      '#type' => 'checkboxes',
      '#title' => t('Vocabularies'),
      '#options' => $options,
      '#default_value' => $this->options['vids'],
      '#process' => array('expand_checkboxes', 'views_process_dependency'),
      '#dependency' => array('edit-options-limit' => array(TRUE)),
    );
  }

  /**
   * Add this term to the query
   */
  function query() {
    $this->add_additional_fields();
  }

  function pre_render($values) {
    $this->field_alias = $this->aliases['vid'];
    $vids = array();
    foreach ($values as $result) {
      $vids[] = $result->{$this->aliases['vid']};
    }

    if ($vids) {
      $voc = '';
      if (!empty($this->options['limit']) && !empty($this->options['vids'])) {
        $voc = " AND td.vid IN (" . implode(', ', array_keys(array_filter($this->options['vids']))) . ")";
      }

      $result = db_query("SELECT tn.vid AS node_vid, td.* FROM {term_data} td INNER JOIN {term_node} tn ON td.tid = tn.tid WHERE tn.vid IN (" . implode(', ', $vids) . ")$voc ORDER BY td.weight, td.name");

      while ($term = db_fetch_object($result)) {
        // get SYNONMS instead for each term
        $syns = taxonomy_get_synonyms($term->tid);
        if ($syns) {
          foreach($syns as $key=> $syn) {
          	if (empty($this->options['link_to_taxonomy'])) {
          		$this->items[$term->node_vid][$term->tid .'.'.$key] = check_plain($syn);
          	} else {
          	   $this->items[$term->node_vid][$term->tid .'.'.$key] = l($syn, taxonomy_term_path($term));
          	}
          	if ($this->options['one_syn']) break;
         }
          //$this->items[$term->node_vid][$term->tid] = check_plain($term->name);
        }
        elseif ($this->options['fallback_term']) {
	      	if (empty($this->options['link_to_taxonomy'])) {
	      		$this->items[$term->node_vid][$term->tid] = check_plain($term->name);
	      	} else {
	      	   $this->items[$term->node_vid][$term->tid] = l($term->name, taxonomy_term_path($term));
	      	}
          //$this->items[$term->node_vid][$term->tid] = l($term->name, taxonomy_term_path($term));
        }
      }
    }
  }
}

