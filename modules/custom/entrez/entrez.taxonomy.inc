<?php
// $Id$
/**
 * @file entrez.admin.inc
 * Provides Entrez' administrative interface.
 */

/**
 * Provides the taxonomy portion of the pubmed import form.
 */
function entrez_form_taxonomy() {
  $form_taxonomy = array (
    '#type' => 'fieldset',
    '#tree' => TRUE, // retain array structure when processing
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Taxonomy Settings'),
    '#description' => t('If you wish, you may select terms to be assigned to imported records.  Terms or key words will apply to all entries being imported at one time.')
  );
  // Get the vocabularies attached to the biblio node type
  // $vocabularies = module_invoke('taxonomy', 'get_vocabularies', 'biblio');
  // taxonomy.module uses the below instead of above
  $c = db_query(db_rewrite_sql("SELECT v.* FROM {vocabulary} v INNER JOIN {vocabulary_node_types} n ON v.vid = n.vid WHERE n.type = '%s' ORDER BY v.weight, v.name", 'v', 'vid'), 'biblio');

  while ($vocabulary = db_fetch_object($c)) {
    if ($vocabulary->tags) {
      // this is a free tagging vocabulary
      // taken directly from taxonomy.module, removing node specific parts
      if ($vocabulary->help) {
        $help = $vocabulary->help;
      }
      else {
        $help = t('A comma-separated list of terms describing this content. Example: funny, bungee jumping, "Company, Inc.".');
      }
      $form_taxonomy['tags'][$vocabulary->vid] = array('#type' => 'textfield',
        '#title' => $vocabulary->name,
        '#description' => $help,
        '#required' => $vocabulary->required,
        '#autocomplete_path' => 'taxonomy/autocomplete/'. $vocabulary->vid,
        '#weight' => $vocabulary->weight,
        '#maxlength' => 255,
      );
    }
    else {
      // not a free-tagging vocabulary.
      $vid = $vocabulary->vid;
      if (in_array($vid, variable_get('taxonomy_treecheck_vids', array()))) {
        // Large vocabularies better with taxonomy_manager_tree form element
        // @TODO remove dependency on taxonomy_treecheck, perhaps make both
        // use a common variable; alternatively make treecheck do more here
        // such as be able to provide this form?
        $form_taxonomy[$vid] = entrez_taxonomy_manager_tree($vocabulary);
      }
      else {
        $form_taxonomy[$vid] = module_invoke('taxonomy', 'form', $vocabulary->vid, array(), $vocabulary->help);
        $form_taxonomy[$vid]['#required'] = $vocabulary->required;
      }
      $form_taxonomy[$vid]['#weight'] = $vocabulary->weight;
    }
  }
  /*
   $vocab_msg = t('There are currently no vocabularies assigned to the biblio node type, please go the the !url page to change this', array ('!url' => l(t('admin/content/taxonomy'), 'admin/content/taxonomy')));
   $form_taxonomy['vocabulary_message'] = array (
   '#value' => '<p><div>' . $vocab_msg . '</div></p>'
   );
   */
  return $form_taxonomy;
}

/**
 * Provide a taxonomy_manager-style tree for selecting terms.
 */
function entrez_taxonomy_manager_tree($vocabulary) {
  $vid = $vocabulary->vid;
  $new_options = array();
  $form_taxonomy_manager_tree = array(
    '#type' => 'taxonomy_manager_tree',
    '#vid' => $vid,
    '#default_value' => array(),
    '#render_whole_tree' => TRUE,
    '#pager' => FALSE,
    '#parent' => 0,
    '#add_term_info' => FALSE,
    '#expand_all' => 0,
    '#tree_is_required' => $vocabulary->required,
    '#required' => FALSE,
    '#multiple' => (isset($vocabulary->multiple) ? $vocabulary->multiple : FALSE),
    '#title' => $vocabulary->title,
    '#field_name' => $vid,
    '#columns' => 0,
    '#description' => $vocabulary->description,
  );
  return $form_taxonomy_manager_tree;
}


