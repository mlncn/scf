<?php
/**
 * @file pubmed_import.inc
 *
 * The contents of this file should be moved to / merged with the gopubmed
 * module when Ben Sharma of Zaloni finally commits it to Drupal.org CVS.
 */

/**
 * Data is ultimately saved by a drupal_write_record() in the node hooks
 * (hook_insert and hook_update) of biblio.module, so all that is needed to
 * save an additional piece of information is to add it to the $b array keyed
 * with the name of a column from the {biblio} table.

 * What are these column names?  We thought you'd never ask:
 +-----------------------------+--------------+------+-----+---------+-------+
| Field                       | Type         | Null | Key | Default | Extra |
+-----------------------------+--------------+------+-----+---------+-------+
| nid                         | int(11)      | NO   | MUL | 0       |       |
| vid                         | int(11)      | NO   | PRI | 0       |       |
| biblio_type                 | int(11)      | NO   |     | 0       |       |
| biblio_number               | varchar(24)  | YES  |     | NULL    |       |
| biblio_other_number         | varchar(24)  | YES  |     | NULL    |       |
| biblio_secondary_title      | varchar(255) | YES  |     | NULL    |       |
| biblio_tertiary_title       | varchar(255) | YES  |     | NULL    |       |
| biblio_edition              | varchar(255) | YES  |     | NULL    |       |
| biblio_publisher            | varchar(255) | YES  |     | NULL    |       |
| biblio_place_published      | varchar(255) | YES  |     | NULL    |       |
| biblio_year                 | int(11)      | NO   | MUL | 9999    |       |
| biblio_volume               | varchar(24)  | YES  |     | NULL    |       |
| biblio_pages                | varchar(128) | YES  |     | NULL    |       |
| biblio_date                 | varchar(16)  | YES  |     | NULL    |       |
| biblio_isbn                 | varchar(24)  | YES  |     | NULL    |       |
| biblio_lang                 | varchar(24)  | YES  |     | eng     |       |
| biblio_abst_e               | text         | YES  |     | NULL    |       |
| biblio_abst_f               | text         | YES  |     | NULL    |       |
| biblio_full_text            | int(11)      | YES  |     | 0       |       |
| biblio_url                  | varchar(255) | YES  |     | NULL    |       |
| biblio_issue                | varchar(24)  | YES  |     | NULL    |       |
| biblio_type_of_work         | varchar(100) | YES  |     | NULL    |       |
| biblio_accession_number     | varchar(30)  | YES  |     | NULL    |       |
| biblio_call_number          | varchar(30)  | YES  |     | NULL    |       |
| biblio_notes                | text         | YES  |     | NULL    |       |
| biblio_custom1              | text         | YES  |     | NULL    |       |
| biblio_custom2              | text         | YES  |     | NULL    |       |
| biblio_custom3              | text         | YES  |     | NULL    |       |
| biblio_custom4              | text         | YES  |     | NULL    |       |
| biblio_custom5              | text         | YES  |     | NULL    |       |
| biblio_custom6              | text         | YES  |     | NULL    |       |
| biblio_custom7              | text         | YES  |     | NULL    |       |
| biblio_research_notes       | text         | YES  |     | NULL    |       |
| biblio_number_of_volumes    | varchar(24)  | YES  |     | NULL    |       |
| biblio_short_title          | varchar(255) | YES  |     | NULL    |       |
| biblio_alternate_title      | varchar(255) | YES  |     | NULL    |       |
| biblio_original_publication | varchar(255) | YES  |     | NULL    |       |
| biblio_reprint_edition      | varchar(255) | YES  |     | NULL    |       |
| biblio_translated_title     | varchar(255) | YES  |     | NULL    |       |
| biblio_section              | varchar(24)  | YES  |     | NULL    |       |
| biblio_citekey              | varchar(255) | YES  |     | NULL    |       |
| biblio_coins                | text         | YES  |     | NULL    |       |
| biblio_doi                  | varchar(255) | YES  |     | NULL    |       |
| biblio_issn                 | varchar(24)  | YES  |     | NULL    |       |
| biblio_auth_address         | text         | YES  |     | NULL    |       |
| biblio_remote_db_name       | varchar(255) | YES  |     | NULL    |       |
| biblio_remote_db_provider   | varchar(255) | YES  |     | NULL    |       |
| biblio_label                | varchar(255) | YES  |     | NULL    |       |
| biblio_access_date          | varchar(255) | YES  |     | NULL    |       |
| biblio_md5                  | varchar(32)  | YES  | MUL | NULL    |       |
+-----------------------------+--------------+------+-----+---------+-------+
 *
 * In addition, contributors (authors/editors etc.) save in a separate table.
 */

/**
 * Replace all other separators in a string with spaces and split into arary.
 *
 * Agaric Utility function most practical for numeric IDs, rather than text.
 *
 * @param $string
 *  String value containing one or more simple values, separated by anything.
 * @return
 *  Array of values from the split string.
 */
function biblioreference_au_rationalize_separators($string) {
  // transform all separators into spaces
  // @REVIEW for pubmed ids we could replace all non-numeric with space
  $seps = array(',', ';', ':', '|', '.', '\r\n', '\n', '\r');
  $string = str_replace($seps, ' ', $string);
  // turns extra spaces into single space.  @TODO replace with preg
  $string = trim(preg_replace('/\s\s+/', ' ', $string));
  return explode(' ', $string);
}

/**
 * Return a form used to import files into biblio.
 *
 * @ingroup forms
 * @see biblioreference_pubmed_import_form_validate()
 * @see biblioreference_pubmed_import_form_submit()
 *
 * @return
 *   An array which will be used by the form builder to build the import form
 */
function biblioreference_pubmed_import_form() {
  global $user;
  $form['biblioreference_pubmed_id'] = array(
    '#type' => 'textarea',
    '#title' => t('PubMed ID'),
    '#description' => '<p>' .
      t('Enter the PMID of your reference(s). If entering more than one ID, separate by spaces, commas, semicolons, or line returns.')
      . '</p>',
    '#required' => TRUE,
  );
  $form['biblioreference_popup_pubmed_message'] = array(
    '#type' => 'markup',
    '#value' => '<p></p><p>If you do not know the PMID, <a href="http://www.ncbi.nlm.nih.gov/pubmed/" title="' . t("Open PubMed in a new window or tab to find PubMed IDs") . '" target="_blank">' . t("Go to PubMed") . '</a>.</p>',
    '#weight'=>10
  );
  // this matches the check in biblio.import.export.inc's biblio_save_node().
  if (user_access('administer nodes')) {
    module_load_include('inc', 'biblio', 'biblio.import.export');
    if (function_exists('_biblio_admin_build_user_select')) {
      // if statement redundant; no need to risk a parse error for this gravy
      // @TODO honestly an autocomplete would be a better interface
      $form['userid'] = _biblio_admin_build_user_select($user->uid);
      $form['userid']['#title'] = t('Set user credited as posting biblios:');
    } else {
      watchdog('biblioreference', 'Expected function _biblio_admin_build_user_select not available from biblio module in file biblio.import.export.inc.', WATCHDOG_ERROR);
    }

  }
  $form['import_taxonomy'] = biblioreference_pubmed_import_form_taxonomy();
  $form['button'] = array(
    '#type' => 'submit',
    '#value' => t('Import from PubMed')
  );
  return $form;
}

/**
 * Provide the taxonomy portion of the pubmed import form.
 */
function biblioreference_pubmed_import_form_taxonomy() {
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
  $c = db_query(db_rewrite_sql("SELECT v.* FROM {vocabulary} v INNER JOIN {vocabulary_node_types} n ON v.vid = n.vid WHERE n.type = '%s' ORDER BY v.weight, v.name", 'v', 'vid'), $node->type);

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
        $form_taxonomy[$vid] = biblioreference_taxonomy_manager_tree($vocabulary);
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
}

function biblioreference_taxonomy_manager_tree($vocabulary) {
  $vid = $vocabulary->vid;
  $new_options = array();
  $form_taxonomy_manager_tree = array(
    '#type' => 'taxonomy_manager_tree',
	  '#vid' => $vid,
			    '#default_value' => is_array($old_tax['#default_value']) ? $old_tax['#default_value'] : array(),
			    '#render_whole_tree' => TRUE,
			    '#pager' => FALSE,
			    '#parent' => 0,
			    '#add_term_info' => FALSE,
			    '#expand_all' => 0,
			    '#tree_is_required' => $old_tax['#required'],
			    '#required' => FALSE,
			    '#multiple' => $supress_multi?FALSE:(isset($old_tax['#multiple']) ? $old_tax['#multiple'] : FALSE),
			    '#title' => $old_tax['#title'],
			    '#field_name' => $vid,
			    '#columns' => 0,
			    '#description' => $old_tax['#description'],
			  );
}

/**
 * Implementation of validation for biblioreference_pubmed_import_form.
 */
function biblioreference_pubmed_import_form_validate($form, &$form_state) {
  // allow multiple pubmed ids to be input in the most flexible way possible
  $pmids = biblioreference_au_rationalize_separators($form_state['values']['biblioreference_pubmed_id']);
  if (!$c = count($pmids)) {
    form_set_error('biblioreference_pubmed_id', t('Please enter at least one PubMed ID.'));
  } else {
    $errors = array();
    for ($i = 0; $i < $c; $i++) {
      if (!ctype_digit($pmids[$i]) || $pmids[$i] <= 0) {
        // "@" sends the input through check_plain to prevent security breaches
        $errors[] = t('A value entered, %pmid, is not a positive integer.', array('%pmid' => $pmids[$i]));
      }
    }
    if ($errors) {
      // Report how many and which entered pubmed IDs were rejected
      form_set_error('biblioreference_pubmed_id', implode(' ', $errors) . ' ' . t('Please ensure you input valid PubMed IDs.'));
    }
    else {
      // add the cleaned-up pubmed IDs to the form values
      // these are passed to submit by reference
      $form_state['values']['pmids'] = $pmids;
    }
  }
}

/**
 * Implementation of submit for the biblioreference_pubmed_import_form.
 */
function biblioreference_pubmed_import_form_submit($form, &$form_state) {
  // clean array of Pubmed IDs from the validate function.
  $pmids = $form_state['values']['pmids'];
  if (isset($form_state['values']['userid'])) {
    $uid = $form_state['values']['userid'];
  }
  else {
    global $user;
    $uid = $user->uid;
  }
  // if we allow creation of free tagging terms, this will become complicated.
  // we will have to know the vocabulary and create the terms first
  $term_ids = array();
  $taxonomy = $form_state['values']['taxonomy']; // works because form tree true
  if (isset($taxonomy) && is_array($taxonomy)) {
    foreach ($taxonomy AS $vid => $vocabulary) {
      $vocab_term_ids = array_keys($vocabulary);  // this changes for free tags
      $term_ids = array_merge($term_ids, $vocab_term_ids);
    }
  }
  $node_info = array(
    'uid' => $uid,
    'taxonomy' => $term_ids,
  );
  if (count($pmids) < 5) {
    foreach ($pmids AS $pmid) {
      biblioreference_import_pubmed_by_pmid($pmid, $node_info);
    }
  }
  else { // go to batch mode
//    biblioreference_import_pubmed_by_pmids($pmids, $node_info, $destination);
    $destination = 'biblioreference/add/citation';
    $size = 5; // number of articles to import and save per cycle
    $batch_op = array(
      'title' => t('Importing @count Pubmed articles', 
        array('@count' => count($pmids))),
      'operations' => array(
        array(
          'biblioreference_import_batch',
          array($pmids, $node_info, $size)
        ),
      ),
      'progressive' => TRUE,
      'finished' => 'biblioreference_import_batch_finished',
      'init_message' => t('Processing bibliography imports from PubMed.'),
      'progress_message' => t('Importing from PubMed and saving bibliographies.'),
      'file' => './'. drupal_get_path('module', 'biblioreference') .'/pubmed_import.inc'
    );
    batch_set($batch_op);
    batch_process($destination);
  }
}

/**
 * Batch callback operation: Import PubMed articles into Biblio.
 *
 * @param $pmids
 *   Array of PubMed IDs to import.
 * @param $node_info
 *   Array of node information to act as default for imported bibliographies.
 * @param $size
 *   Number of articles to import in each operation.
 * @param $context
 *   Batch context containing state information.
 */
function biblioreference_import_batch($pmids, $node_info, $size, &$context) {
  // Initialize sandbox the first time through
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($pmids);
    $context['results']['nids'] = array();
  }
  $max = min(
    $context['sandbox']['max'],
    $context['sandbox']['progress'] + $size
  );
  // we count on array always keyed 0,1,2 etc and pass the current key on
  for ($i = $context['sandbox']['progress']; $i < $max; $i++) {
    $pmid = $pmids[$i];
    watchdog('biblioreference', $i . 'pmid ' . $pmid); //@debug
    $nid = biblioreference_import_pubmed_by_pmid($pmid, $node_info);
    if ($nid) {
      $context['results']['nids'][] = $nid;
    }
  }
  $context['sandbox']['progress'] = $i;
  // Multistep processing : report progress.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

function biblioreference_import_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural(count($results['nids']), 'One bibliography saved.', '@count bibliographies saved.');
  }
  else {
    $message = t('Import finished with an error.');
  }
  drupal_set_message($message);
/*  
  foreach ($results['nids'] as $node_id) {
    if (count($results['terms'])) module_invoke('taxonomy', 'node_save', $node_id, $results['terms']);
    db_query('UPDATE {node} SET uid = %d WHERE nid = %d', $results['userid'], $node_id);
    db_query('UPDATE {node_revisions} SET uid = %d WHERE nid = %d', $results['userid'], $node_id);
  }
*/
}

/**
 * Import and save a Pubmed document to Biblio by Pubmed ID.
 *
 * Optionally take an array of Drupal taxonomy term IDs to attach to the biblio
 * node when saving.
 */
function biblioreference_import_pubmed_by_pmid($pmid, $node_info = array()) {
  $pmxml = biblioreference_load_pubmed_medline_xml_by_pmid($pmid);
  if (!$pmxml)  return;  // error message already set
  $biblio = biblioreference_parse_pubmed_medline_xml_for_biblio($pmxml, $pmid);
  if (!$biblio)  return;  // error message should already be triggered
  $nodelet = db_fetch_array(db_query("SELECT nid, vid from {biblio} WHERE `biblio_citekey` = %d LIMIT 1", $pmid));
  if (!$nodelet) {
    drupal_set_message(t("This is new primary ID: @pmid", array('@pmid' => $pmid)));
    $nodelet = array();
  }
  else {
    drupal_set_message(t("A bibliographic entry exists for @pmid; it will be updated with this import.", array('@pmid' => $pmid)));
    // the nodelet array has 'nid' and 'vid' in it already
    // vid is important for drupal_write_record to work right in node_save
    //@TODO - do a full node_load here to retain all non-biblio info?  such as?
    $nodelet['taxonomy'] = taxonomy_node_get_terms((object)$nodelet);
  }
  // @TODO deal with merging taxonomy so that node_info defaults retained
  $biblio = array_merge(
    biblioreference_biblio_node_defaults(),
    $node_info,
    $nodelet,
    $biblio
  );
  $biblio = (object)$biblio;
  // needs to be included
  module_load_include('inc', 'biblio', 'biblio.keywords');
  module_load_include('inc', 'biblio', 'biblio.import.export');
  node_save($biblio);
  // biblio_save_node calls node_save which triggers biblio_insert or _update
  // no need to call biblio_save_node, we are handling defaults and our batches 
/* this is being done?  It should be!  or in a wrapper save_node function if we must
  foreach ($nids as $node_id) {
    if (count($tid)) {
      module_invoke('taxonomy', 'node_save', $node_id, $term_ids);
    }
    db_query('UPDATE {node} SET uid = %d WHERE nid = %d', $uid, $node_id);
    db_query('UPDATE {node_revisions} SET uid = %d WHERE nid = %d', $uid, $node_id);
*/
  drupal_set_message(t('Imported bibliographic citation') . ' ' . l($biblio->title, 'node/' . $biblio->nid));
  return $biblio->nid;
}

/**
 * Biblio node defaults for use as the base of merging in other values.
 *
 * In general use user-configurable settings afforded by biblio module.
 */
function biblioreference_biblio_node_defaults() {
  $options = variable_get('node_options_biblio', array('status'));
  $node_defaults = array(
    'type' => 'biblio',
    'uid' => 1,
    'status' => in_array('status', $options),
    'promote' => in_array('promote', $options),
    'moderate' => in_array('moderate', $options), // note: not used in Drupal core
    'sticky' => in_array('sticky', $options),
    'format' => 0,
    'comment' => variable_get('comment_biblio', 0),
    'taxonomy' => array(),
    'biblio_type' => 129, // miscellaneous
  );
  return $node_defaults;
}

/**
 * Load a single pubmed article Medline Citation XML given a pubmed ID.
 *
 * ESummary Retrieves document summaries from a list of primary IDs
 * which we got from the ESearch
 * $esummary_url = "http://eutils.ncbi.nlm.nih.gov/entrez/eutils/esummary.fcgi?db=$db&id=$pmid&retmode=xml";
 * $esummary_load = simplexml_load_file($esummary_url);
 */
function biblioreference_load_pubmed_medline_xml_by_pmid($pmid) {
  $pmurl = 'http://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?'
         . "db=pubmed&id={$pmid}&retmode=xml";
  $pmxml = simplexml_load_file($pmurl);
  if (!is_object($pmxml)) {  // make sure the connection worked
    watchdog('biblioreference', 'Failed to fetch document summary object from %url.', array('%url' => $pmurl));
    drupal_set_message(t("Failed to fetch document summary object from %url.",
      array('%url' => $pmurl)), 'error');
    return;
  }
  $medcit = $pmxml->PubmedArticle->MedlineCitation;
  if (!is_object($medcit)) { // make sure the object has a pubmed citation
    $msg = t('The object for PubMed ID %pmid does not have a Medline Citation', array('%pmid' => $pmid));
    watchdog('biblioreference', $msg, WATCHDOG_ERROR);
    drupal_set_message($msg, 'error');
  }
  return $medcit;
}

/**
 * Parse single PubMed article MedlineCitation XML and assign values to biblio.
 *
 * Heavily modified from gopubmed.module (our upgraded to D6 version), function
 * gopubmed_load_bibtext_by_pmid($primaryId, $db = 'pubmed').
 *
 * @return array an array of values suitable for import into a biblio node
 */
function biblioreference_parse_pubmed_medline_xml_for_biblio($pm) {
  $pmid = (string)$pm->PMID;
  $b = array();
  $b['title'] = (string)$pm->Article->ArticleTitle;
  $b['biblio_citekey'] = $pmid;
  $b['biblio_contributors'] = biblioreference_pubmed_contributors($pm);
  // MedlineCitations are always articles from journals or books
  $b['biblio_type'] = 102;
  $b['biblio_date'] = biblioreference_pubmed_pubdate($pm);
  $b['biblio_year'] = substr($b['biblio_date'], 0, 4);
  $b['biblio_secondary_title'] = (string)$pm->Article->Journal->Title;
  $b['biblio_alternate_title'] = (string)$pm->Article->Journal->ISOAbbreviation;
  $b['biblio_volume'] = (string)$pm->Article->Journal->JournalIssue->Volume;
  $b['biblio_issue'] = (string)$pm->Article->Journal->JournalIssue->Issue;
  $b['biblio_issn'] = (string)$pm->Article->Journal->Issn;
  $b['biblio_pages'] = (string)$pm->Article->Pagination->MedlinePgn;
  // not used?    $b['language = {' . $language . '},'. "\n";
  $b['biblio_abst_e'] = (string)$pm->Article->Abstract->AbstractText;
  $b['biblio_url'] = biblioreference_pubmed_article_link($pmid);
  $b['biblio_custom1'] = "http://www.ncbi.nlm.nih.gov/pubmed/$pmid?dopt=Abstract";
//  $b['biblio_custom7'] = $pmurl;  // would have to be reconstructed or passed in
  $doi = $pm->xpath('//ELocationID[@EIdType="doi"]/text()');
  if (!empty($doi)) {
    $b['biblio_doi'] = $doi[0];
  }
  return $b;
}

/**
 * Returns the list of contributors for import obtained from the given
 * MedlineCitation element.
 *
 * @param object $pm
 * @return array
 */
function biblioreference_pubmed_contributors($pm) {
  if (!isset($pm->Article->AuthorList->Author)) {
    watchdog(
      'biblioreference',
      'For PubMed ID %pmid Article->AuthorList->Author not set in Articles: !articles.', 
      array(
        '%pmid' => (string)$pm->PMID,
        '!articles' => '<pre>' . var_export($pm->Article, TRUE) . '</pre>',
      )
    );
    return array();
  }
  foreach ($pm->Article->AuthorList->Author as $author) {
    if (isset($author->CollectiveName)) {
      $category = 5; // corporate author
      $name = (string)$author->CollectiveName;
    } else {
      $category = 1; //primary (human) author
      $lastname = (string)$author->LastName;
      if (isset($author->ForeName)) {
        $name = $lastname . ', ' . (string)$author->ForeName;
      } elseif (isset($author->FirstName)) {
        $name = $lastname . ', ' . (string)$author->FirstName;
      } elseif (isset($author->Initials)) {
        $name = $lastname . ', ' . (string)$author->Initials;
      }
    }
    $contributors[$category][] = array('name' => $name);
  }
  return $contributors;
}

/**
 * Returns the publication date obtained from the given MedlineCitation's
 * PubDate element. See the reference documentation for possible values:
 * http://www.nlm.nih.gov/bsd/licensee/elements_descriptions.html#pubdate
 * According to the above source it always begins with a four digit year.
 *
 * @param object $pm
 * @return string
 */
function biblioreference_pubmed_pubdate($pm) {
  $pub_date = $pm->Article->Journal->JournalIssue->PubDate;
  if (isset($pub_date->MedlineDate)) {
    $date = $pub_date->MedlineDate;
  } else {
    $date = implode(' ', (array)$pub_date);
  }
  return $date;
}

/**
 * Get the full article link through ELink.
 *
 * ELink Checks for the existence of an external or Related Articles link from
 * a list of one or more primary IDs.  Retrieves primary IDs and relevancy
 * scores for links to Entrez databases or Related Articles;  creates
 * a hyperlink to the primary LinkOut provider for a specific ID and database,
 * or lists LinkOut URLs and Attributes for multiple IDs
 */
function biblioreference_pubmed_article_link($pmid, $db = 'pubmed') {
//@TODO - this has been hanging and delaying testing.  check what's what.
return '';
  $llink_url = "http://eutils.ncbi.nlm.nih.gov/entrez/eutils/elink.fcgi?dbfrom=$db&id=$pmid&cmd=prlinks";
  $llink_load = simplexml_load_file($llink_url);
  if (!is_object($llink_load)) {
    drupal_set_message(t("Failed to fetch object from %url.", array('%url' => $llink_url)), 'error');
    return;
  } // we have an object, continue on
  $llink_array = $llink_load->ObjUrl->xpath("//Url");
  $article_link = $llink_array[0];
  return $article_link;
}

/**
 * Get abstract through EFetch given a PubMed Id.
 *
 * Retrieves records in the requested format from a list of one or
 * more primary IDs or from the user's environment.
 * @TODO - why is this a separate link and load?  abstract is in XML
 */
function biblioreference_pubmed_abstract($pmid) {
  $efetch = "http://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?db=pubmed&id=$pmid&retmode=xml";
  $efetch_load = simplexml_load_file($efetch);
  if (!is_object($efetch_load)) {
    drupal_set_message(t("Failed to fetch object from %url.", array('%url' => $efetch)), 'error');
    return;
  }

  $abstract = $efetch_load->Abstract->xpath("//AbstractText");
  $final_abstract = $abstract[0];
  return $final_abstract;
}
