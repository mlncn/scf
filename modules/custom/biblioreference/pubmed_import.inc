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
  $seps = array(',', ';', ':', '|', '.');
  $string = str_replace($seps, ' ', $string);
  $string = trim(ereg_replace(' +', ' ', $string));
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
    '#type' => 'textfield',
    '#title' => t('PubMed ID'),
    '#description' => t('One or more PubMed unique identifiers of articles.  If more than one, separate by spaces, commas, or semicolons.'),
    '#required' => TRUE,
  );
  $form['biblioreference_popup_pubmed_message'] = array(
    '#type' => 'markup',
    '#value' => '<p><a href="http://www.ncbi.nlm.nih.gov/pubmed/" title="' . t("Open PubMed in a new window or tab to find PubMed IDs") . '" target="_blank">' . t("Go to PubMed") . '</a></p>',
  );

  // this matches the check in biblio.import.export.inc's biblio_save_node().
  if (user_access('administer nodes')) {
    module_load_include('inc', 'biblio', 'biblio.import.export');
    if (function_exists('_biblio_admin_build_user_select')) {
      // if statement redundant; no need to risk a parse error for this gravy
      // @TODO honestly an autocomplete would be a better interface
      $form['userid'] = _biblio_admin_build_user_select($user->uid);
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
  $uid = $form_state['values']['userid'];
  // if we allow creation of free tagging terms, this will become complicated.
  // we will have to know the vocabulary and create the terms first
  $term_ids = array();
  $taxonomy = $form_state['values']['taxonomy']; // works because form tree true
  foreach ($taxonomy AS $vid => $vocabulary) {
    $vocab_term_ids = array_keys($vocabulary);  // this changes for free tags
    $term_ids = array_merge($term_ids, $vocab_term_ids);
  }
  // @debug
  //drupal_set_message('term_ids: <pre>'.var_export($term_ids,TRUE).'</pre>');
  foreach ($pmids AS $pmid) {
    biblioreference_import_pubmed_by_pmid($pmid, $uid, $term_ids);
  }
}

/**
 * Import and save a Pubmed document to Biblio by Pubmed ID.
 *
 * Optionally take an array of Drupal taxonomy term IDs to attach to the biblio
 * node when saving.
 */
function biblioreference_import_pubmed_by_pmid($pmid, $uid = 1, $term_ids = array()) {
  $biblio = biblioreference_parse_pubmed_for_biblio($pmid);
  if (!$biblio)  return;  // error message should already be triggered
  // need to pass in an object with a version id to get terms, so make nodelet
  $nodelet = db_fetch_object(db_query("SELECT nid, vid from {biblio} WHERE `biblio_citekey` = %d LIMIT 1", $pmid));
  if (!$nodelet) {
    drupal_set_message(t("This is new primary ID: @pmid", array('@pmid' => $pmid)));
  }
  else {
    drupal_set_message(t("A bibliographic entry exists for @pmid; it will be updated with this import.", array('@pmid' => $pmid)));
    $biblio['nid'] = $nodelet->nid;
    // vid is important for drupal_write_record to work right in node_save
    $biblio['vid'] = $nodelet->vid;
    //@TODO - do a full node_load here to retain all non-biblio info?  such as?
    $terms = taxonomy_node_get_terms($nodelet);
    //@DEBUG
    //drupal_set_message('terms:<pre>'.var_export($terms,TRUE).'</pre>');
    if (!empty($terms)) {
      if (!isset($biblio['taxonomy'])) $biblio['taxonomy'] = array();
      $biblio['taxonomy'] = array_merge($terms, $biblio['taxonomy']);
    }
  }
  $biblio['uid'] = $uid;
//@DEBUG
//drupal_set_message('taxonomy:<pre>'.var_export($biblio['taxonomy'],TRUE).'</pre>');
  if (!isset($biblio['taxonomy']))  $biblio['taxonomy'] = array();
    //@DEBUG
    //drupal_set_message('term_ids:<pre>'.var_export($term_ids,TRUE).'</pre>');
  $biblio['taxonomy'] = array_merge($term_ids, $biblio['taxonomy']);
//@DEBUG
//drupal_set_message('node_array:<pre>'.var_export($biblio,TRUE).'</pre>');

  // needs to be included
  module_load_include('inc', 'biblio', 'biblio.keywords');
  // biblio_save_node calls node_save which triggers biblio_insert or _update
  $nid = biblio_save_node($biblio); // , $batch, $session_id);
/* this is being done by biblio?
  foreach ($nids as $node_id) {
    if (count($tid)) {
      module_invoke('taxonomy', 'node_save', $node_id, $term_ids);
    }
    db_query('UPDATE {node} SET uid = %d WHERE nid = %d', $uid, $node_id);
    db_query('UPDATE {node_revisions} SET uid = %d WHERE nid = %d', $uid, $node_id);
*/
    // Not worth the db query to get the title, but we have it already still.
  drupal_set_message(t('Imported bibliographic citation') . ' ' . l($biblio['title'], 'node/' . $nid));

}


/**
 * Load a single pubmed article XML by pubmed ID.
 *
 * Heavily modified from gopubmed.module (our upgraded to D6 version), function
 * gopubmed_load_bibtext_by_pmid($primaryId, $db = 'pubmed').
 *
 * Ideally the fetching the xml would be a different step from parsing it, but
 * the simplexml_load_file function does it all with a .
 * or best get changes incorporated into gopubmed and keep dependency.
 */
function biblioreference_parse_pubmed_for_biblio($pmid) {
  $b = array();  // an array of values suitable for import into a biblio node
  $db = 'pubmed';

  // ESummary Retrieves document summaries from a list of primary IDs
  // which we got from the ESearch
//  $esummary_url = "http://eutils.ncbi.nlm.nih.gov/entrez/eutils/esummary.fcgi?db=$db&id=$pmid&retmode=xml";
//  $esummary_load = simplexml_load_file($esummary_url);
  $pmurl = 'http://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?db=pubmed&id=' . $pmid . '&retmode=xml';
// @debug local file
// $pmurl = '';
  $pmxml = simplexml_load_file($pmurl);
  if (!is_object($pmxml)) {  // make sure the connection worked
    drupal_set_message(t("Failed to fetch document summary object from %url.", array('%url' => $pmurl)), 'error');
    // use au_alert - watchdog('biblioreference', t("
    return;
  }
  // drill down to what we need
  $pm = $pmxml->PubmedArticle->MedlineCitation;
  // aliases to further drilling down, but note this hides the true hierarchy
  $pma = $pm->Article;
  $pmj = $pma->Journal;
  $pmji = $pmj->JournalIssue;
//@/debug
// drupal_set_message('summary:<pre>'.var_export($pm,TRUE).'</pre>');


  $article_link = biblioreference_pubmed_article_link($pmid);
//@TODO replace below with robust functions that will try alternate versions
  $title = (string) $pma->ArticleTitle;
  $journal = (string) $pmj->Title;
  $volume = (string) $pmji->Volume;
  $issue = (string) $pmji->Issue;
  $pages = (string) $pma->Pagination->MedlinePgn;
  $pubdate_values = (array) $pmji->Pubdate;
  $pubdate = implode(' ', $pubdate_values);
  $abstract = $pma->Abstract->AbstractText;
  $contributors = array();
  // cat 1 = type 1 = ordinary authors for biblio articles
  $cat = 1;
  $contributors[$cat] = array();
  foreach ($pma->AuthorList->Author as $author) {
    $author = (array) $author;
    $valid = array_shift($author);
    $lastname = array_shift($author);
    $forename = array_shift($author);
    $initials = array_shift($author);
    if (count($author)) {
      //@debug
      drupal_set_message('Remaining parts of author name:<pre>'.var_export($author,TRUE).'</pre>');
    }
    // a little silly, but we have to combine author names for biblio to parse
    $name = $lastname . ', ' . $forename;
    $contributors[$cat][] = array('name' => $name);
  }
/*
      else if($summary['Name'] == "ePubDate")
        $epubdate        = $summary;
      else if($summary['Name'] == "Pagination")
        $pagination      = $summary;
      else if($summary['Name'] == "SortDate")
        $sortdate        = $summary;
      else if($summary['Name'] == "Links")
        $links           = $summary;
      else if($summary['Name'] == "PmcLiveDate")
        $pmclivedate     = $summary;
      else if($summary['Name'] == "XmlData")
        $xmldata         = $summary;
      else if($summary['Name'] == "FullJournalName")
        $fullJournalName = $summary;
      
    }
  if (!$fullJournalName) {
    $journalname = $journal;
  }
  else {
    $journalname = $fullJournalName;
  }
*/
// Create pubmed url
  $custom1 = "http://www.ncbi.nlm.nih.gov/pubmed/$pmid?dopt=Abstract";
  // fetch abstract - we get this direct from the xml
//  $abstract = biblioreference_pubmed_abstract($pmid);

  // we have all our data, now assemble the biblio node array
  $b['title'] = $title;
  $b['biblio_citekey'] = $pmid;
  // this is perfect for handling mesh terms, which should not be accessible
  // by the user interface
  if ($keywords) {
    $b['biblio_keywords'] = $keywords;
    if (strpos($keywords,';')) $keywords = str_replace(';', ',', $keywords);
    $vid = variable_get('biblio_keyword_vocabulary', 0);
    if ($vid  && variable_get('biblio_keyword_freetagging', 0)) {
      $biblio['taxonomy']['tags'][$vid] .= $keywords;
    }
    $node['biblio_keywords'] = explode(',', $keywords);
  }
  $b['biblio_contributors'] = $contributors;
  // @CLEANUP delete legacy here in comments once shown no use
  //      $authorArray = preg_split("/\s(and|&)\s/i", trim($entry['author']));
  //        $node['biblio_contributors'][1][]= array('name' => $author, 'auth_type' => _biblio_get_auth_type(1, $node['biblio_type']));
  //    if (!empty($entry['editor']))
  //        $node['biblio_contributors'][2][]= array('name' => $author, 'auth_type' => _biblio_get_auth_type(2, $node['biblio_type']));

    // historically this was automatically coded to 'article'
    // See "Now creating the pubmed.bib file" part of gopubmed.module
    //  $article_bib .= '@article{' . $summaryid . ',' . "\n";
    // we can skip the entire switch ($entry['bibtexEntryType'] statement from
    // biblio/bibtexParse/PARSEENTRIES.php but we should review to make sure
    // "article" is the only type PubMed ever has
    // (note that changing biblio_type changes the meaning of some fields.
    $b['biblio_type'] = 102; // "article"
    $b['biblio_date'] = biblioreference_au_if($pubdate);
    $b['biblio_year'] = biblioreference_au_if($pubdate_values['Year']);
    $b['biblio_secondary_title'] = biblioreference_au_if($journal);
    $b['biblio_volume'] = biblioreference_au_if($volume);
    $b['biblio_issue'] = biblioreference_au_if($issue);
    $b['biblio_pages'] = biblioreference_au_if($pages);
    if ($note != '') {  // @TOREVIEW this can result in no keywords if no note?
      $b['biblio_notes'] = $note . ' Additional Keywords: ' . $keywordlist;
    }
    $b['biblio_isbn'] = $isbn;
// not used?    $b['language = {' . $language . '},'. "\n";
    $b['biblio_abst_e'] = $abstract; // linked by 'abstract' in prev impl
    $b['biblio_url'] = biblioreference_au_if($article_link);  // 'url'
    $b['biblio_custom1'] = $custom1;
    $b['biblio_custom7'] = $pmurl; // the very xml used to lead everything
// not used!!  PARSEENTRIES, which saves the file, has no saving of custom!
//    $article_bib .= 'custom1 = {' . $custom1 . '},' . "\n";
//    $article_bib .= 'custom2 = {' . $search_term . '},' . "\n";
//    $article_bib .= 'custom3 = {' . $primaryId . '}' . "\n";
// also not used!  Saved by PARSEENTRIES, but were never entered by gopubmed
//      if (!empty($entry['booktitle'])) $node['biblio_secondary_title'] =  $entry['booktitle'];
//      if (!empty($entry['series']))    $node['biblio_secondary_title'] =  $entry['series'];
//      $node['biblio_date']            = (!empty($entry['month'])) ? $entry['month'] : NULL;
//      $node['biblio_type_of_work']    .= (!empty($entry['type'])) ? $entry['type'] : NULL;
//      $node['biblio_edition']         = (!empty($entry['edition'])) ? $entry['edition'] : NULL;
//      $node['biblio_section']         = (!empty($entry['chapter'])) ? $entry['chapter'] : NULL;
//      $node['biblio_place_published'] = (!empty($entry['address'])) ? $entry['address'] : NULL;
//      $node['biblio_doi']             = (!empty($entry['doi'])) ? $entry['doi'] : NULL;
// @TODO add the above, possibly, and any other values we want


// @TODO do warnings right
//    $results['watchdog_status'] = t("Publication not entered. PMID @pmid already there.", array('@pmid' => $primaryId));
//  $watchdog_level = (isset($result['watchdog_level'])) ? $result['watchdog_level'] : WATCHDOG_INFO;
//  watchdog('biblioreference', $result['watchdog_status'], $watchdog_level);
  return $b;
}

function biblioreference_au_if($value) {
  if (!isset($value)) {
    return NULL;  // not sure what's going on
  }
  // this makes no sense?  will it treat it as the array thing being passed in?
  $value = (!empty($value)) ? $value : NULL;
  return $value;
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


/**
 * Import pubmed data.
 *
 * @TODO remove or put it somewhere it makes sense
 * @param $data
 *   the contents of a bibtex file passed as one big string
 * @param $node
 *   an array (populated in biblio_import() ), containing the boiler plate
 *   information common to all nodes
 * @return
 *   an array of node ids
 */


//// UTILITY FUNCTIONS - @TODO replace with module dependency

if (!function_exists('au_alert')) {

/**
 * Triggers a drupal_set_message and a watchdog alert at the same time.
 */
function au_alert() {
// @TODO
}

}
