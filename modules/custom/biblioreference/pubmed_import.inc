<?php
/**
 * @file pubmed_import.inc
 *
 * The contents of this file should be moved to / merged with the gopubmed 
 * module when Ben Sharma of Zaloni finally commits it to Drupal.org CVS.
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

  // this matches the check in biblio.import.export.inc's biblio_save_node().
  if (user_access('administer nodes')) {
    module_load_include('inc', 'biblio', 'biblio.import.export');
    if (function_exists('_biblio_admin_build_user_select')) {
      // redundant if statement; no need to risk a parse error for this gravy
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
    drupal_set_message('term_ids: <pre>'.var_export($term_ids,TRUE).'</pre>');
  foreach ($pmids AS $pmid) {
    biblioreference_import_pubmed_by_pmid($pmid, $term_ids);
  }
}

/**
 * Import and save a Pubmed document to Biblio by Pubmed ID.
 *
 * Optionally take an array of Drupal taxonomy term IDs to attach to the biblio
 * node when saving.
 */
function biblioreference_import_pubmed_by_pmid($pmid, $term_ids = array()) {
  $biblio = biblioreference_parse_pubmed_for_biblio($pmid);
  $article_bib = $result['article_bib'];
  
  $data = $result['article_bib'];

  $nids = biblioreference_bibtex_import($data, $term_ids);

/* this is being done by biblio?
  foreach ($nids as $node_id) {
    if (count($tid)) {
      module_invoke('taxonomy', 'node_save', $node_id, $term_ids);
    }
    db_query('UPDATE {node} SET uid = %d WHERE nid = %d', $uid, $node_id);
    db_query('UPDATE {node_revisions} SET uid = %d WHERE nid = %d', $uid, $node_id);
*/    
    // Not worth the db query to get the title, but here's the link.
  drupal_set_message(t('Imported <a href="@url">bibliographic citation</a>.', array('@url' => url('node/' . $node_id))));

}


/**
 * Load a single bibtext by pubmed ID.
 *
 * Taken directly from gopubmed.module (our upgraded to D6 version), function
 * gopubmed_load_bibtext_by_pmid($primaryId, $db = 'pubmed').
 * 
 * Ideally the fetching the xml would be a different step from parsing it, but
 * the simplexml_load_file function does it all with a . 
 * or best get changes incorporated into gopubmed and keep dependency.
 */
function biblioreference_parse_pubmed_for_biblio($pmid) {
  $b = array();  // an array of values suitable for import into a biblio node
  $sql_pmid = db_result(db_query("SELECT COUNT(*) from {biblio} WHERE `biblio_custom3` = %d", $pmid));
  if ($sql_pmid == 0) {
    $results['watchdog_status'] = t("This is new primary ID: @pmid", array('@pmid' => $primaryId));
    // ESummary Retrieves document summaries from a list of primary IDs
    // which we got from the ESearch
    $esummary_url = "http://eutils.ncbi.nlm.nih.gov/entrez/eutils/esummary.fcgi?db=$db&id=$primaryId&retmode=xml";
    $esummary_load = simplexml_load_file($esummary_url);
    $summaryid = $esummary_load->DocSum[0]->Id;
    // @debug
    drupal_set_message('summaryid: '.$summaryid);
    $article_link = biblioreference_pubmed_article_link($primaryId);  


    foreach ($esummary_load->DocSum[0]->Item as $summary) {
      $authorlist = "";
      $author_all = "";
      if ($summary['Name'] == "AuthorList") {
// @TODO I think this is what must change for proper author import            
        foreach ($summary->Item as $authorlist) {
          // @debug
          drupal_set_message('authorlist:<pre>'.var_export($authorlist,TRUE).'</pre>');
          $author_all .=  $authorlist . ' , '. "\n";
        }
        $article_bib .= 'author = {' . $author_all . '},'. "\n";
      }
      else {
        if ($summary['Name'] == "Author") {
          $article_bib .= 'author = {' . $summary . '},'. "\n";
        // @debug
        drupal_set_message('authorname:<pre>'.var_export($summary,TRUE).'</pre>');
        }
      }
// @TODO change this to a switch or something less ugly.  I did not write this.
      if ($summary['Name'] == "Title")
        $title           = $summary;
      else if($summary['Name'] == "PubDate")
        $pubdate         = $summary;
      else if($summary['Name'] == "ePubDate")
        $epubdate        = $summary;
      else if($summary['Name'] == "Source")
        $journal         = $summary;
      else if($summary['Name'] == "Volume")
        $volume          = $summary;
      else if($summary['Name'] == "Pages")
        $pages	     = $summary;
      else if($summary['Name'] == "Issue")
        $issue           = $summary;
      else if($summary['Name'] == "Pagination")
        $pagination      = $summary;
      else if($summary['Name'] == "PmId")
        $pmid            = $summary;
      else if($summary['Name'] == "SortDate")
        $sortdate        = $summary;
      else if($summary['Name'] == "Links")
        $links           = $summary;
      else if($summary['Name'] == "PmcLiveDate")
        $pmclivedate     = $summary;
      else if($summary['Name'] == "Abstract")
        $Abstract        = $summary;
      else if($summary['Name'] == "XmlData")
        $xmldata         = $summary;
      else if($summary['Name'] == "FullJournalName")
        $fullJournalName = $summary;
      
      if($fullJournalName == "") {
        $journalname = $journal;
      }
      else {
        $journalname = $fullJournalName;
      }
    }
    // Creating pubmed url
    $custom1 = "http://www.ncbi.nlm.nih.gov/pubmed/$primaryId?dopt=Abstract";
        
    // Getting abstract through EFetch.
    // Retrieves records in the requested format from a list of one or 
    // more primary IDs or from the user's environment.
    $efetch = "http://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?db=pubmed&id=$primaryId&retmode=xml";
    $efetch_load = simplexml_load_file($efetch);
    if (!is_object($efetch_load)) {
      drupal_set_message(t("Failed to fetch object from %url.", array('%url' => $efetch)), 'error');      
      return;
    }
      
    $abstract = $efetch_load->Abstract->xpath("//AbstractText");
    $final_abstract = $abstract[0];
        
    $b['title'] = $title;

    // historically this was automatically coded to 'article'
    // See "Now creating the pubmed.bib file"
    //  $article_bib .= '@article{' . $summaryid . ',' . "\n";
    // we can skip the entire switch ($entry['bibtexEntryType'] statement from
    // biblio/bibtexParse/PARSEENTRIES.php but we should review to make sure 
    // "article" is the only type PubMed ever has
    $b['biblio_type'] = 102; // "article"
    $b['biblio_year'] = biblioreference_au_if($pubdate);
    $b['biblio_secondary_title'] = biblioreference_au_if($journalname);
    $b['biblio_volume'] = biblioreference_au_if($volume);
    $b['biblio_number'] = biblioreference_au_if($number);
    $b['biblio_pages'] = biblioreference_au_if($pages);
    if ($note != '') {  // @TOREVIEW this can result in no keywords if no note?
      $b['biblio_notes'] = $note . ' Additional Keywords: ' . $keywordlist;
    }
    $b['biblio_keywords'] = $keywords;  
    $b['biblio_isbn'] = $isbn;
// not used?    $b['language = {' . $language . '},'. "\n";
    $b['biblio_abst_e'] = $final_abstract; // linked by 'abstract' in prev impl
    $article_bib .= 'url = {' . $article_link . '},' . "\n";
    $article_bib .= 'custom1 = {' . $custom1 . '},' . "\n";
    $article_bib .= 'custom2 = {' . $search_term . '},' . "\n";
    $article_bib .= 'custom3 = {' . $primaryId . '}' . "\n";

    $results['article_bib'] = $article_bib;      
  }
  else {
    // @TODO - override old data by default
  }


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
function biblioreference_pubmed_article_link($primaryId, $db = 'pubmed') {
    $llink_url = "http://eutils.ncbi.nlm.nih.gov/entrez/eutils/elink.fcgi?dbfrom=$db&id=$primaryId&cmd=prlinks";
  $llink_load = simplexml_load_file($llink_url);
  if (!is_object($llink_load)) {
    drupal_set_message(t("Failed to fetch object from eutils.ncbi.nlm.nih.gov."), 'error');
    // @debug
    drupal_set_message('Link load: <pre>'.var_export($llink_load,TRUE).'</pre>');   
    return;
  } // we have an object, continue on
  $llink_array = $llink_load->ObjUrl->xpath("//Url");
  $article_link = $llink_array[0];
  return $article_link;
}

/**
 * Import pubmed data modeled on biblio_bibtex_import but using data option.
 *
 * @param $data
 *   the contents of a bibtex file passed as one big string
 * @param $node
 *   an array (populated in biblio_import() ), containing the boiler plate
 *   information common to all nodes
 * @return
 *   an array of node ids
 */
function biblioreference_bibtex_import($data, $terms, $batch = NULL, $session_id = NULL) {
  $nids = array();
  
  return $nids;
}


      $node['biblio_contributors'] = array();

      if (!empty($entry['author'])){
        // split on ' and '
        $authorArray = preg_split("/\s(and|&)\s/i", trim($entry['author']));
        foreach ($authorArray as $key => $author)
        {
          $node['biblio_contributors'][1][]= array('name' => $author, 'auth_type' => _biblio_get_auth_type(1, $node['biblio_type']));
        }
      }

      $node['biblio_citekey'] = (!empty($entry['bibtexCitation'])) ? $entry['bibtexCitation'] : NULL;
      if (!empty($entry['editor']))
      {
        $authorArray = preg_split("/\s(and|&)\s/i", trim($entry['editor']));
        foreach ($authorArray as $key => $author)
        {
          $node['biblio_contributors'][2][]= array('name' => $author, 'auth_type' => _biblio_get_auth_type(2, $node['biblio_type']));
        }
      }

      if (!empty($entry['booktitle'])) $node['biblio_secondary_title'] =  $entry['booktitle'];
      if (!empty($entry['series']))    $node['biblio_secondary_title'] =  $entry['series'];
      $node['biblio_date']            = (!empty($entry['month'])) ? $entry['month'] : NULL;
      $node['title']                  = (!empty($entry['title'])) ? $entry['title'] : NULL;
      $node['biblio_type_of_work']    .= (!empty($entry['type'])) ? $entry['type'] : NULL;
      $node['biblio_edition']         = (!empty($entry['edition'])) ? $entry['edition'] : NULL;
      $node['biblio_section']         = (!empty($entry['chapter'])) ? $entry['chapter'] : NULL;
      $node['biblio_place_published'] = (!empty($entry['address'])) ? $entry['address'] : NULL;
      if (!empty($entry['keywords'])){
        if (strpos($entry['keywords'],';')) $entry['keywords'] = str_replace(';',',',$entry['keywords']);
        $vid = variable_get('biblio_keyword_vocabulary', 0);
        if ($vid  && variable_get('biblio_keyword_freetagging', 0)) {
          $node['taxonomy']['tags'][$vid] .= $entry['keywords'];
        }
        $node['biblio_keywords'] = explode(',', $entry['keywords']);
      }
      $node['biblio_url']             = (!empty($entry['url'])) ? $entry['url'] : NULL;
      $node['biblio_doi']             = (!empty($entry['doi'])) ? $entry['doi'] : NULL;
      if (!empty($terms)) {
        if (!isset($node['taxonomy'])) $node['taxonomy'] = array();
        $node['taxonomy'] = array_merge($terms,$node['taxonomy']);
      }
      $nids[] = biblio_save_node($node, $batch, $session_id);
    }
    return (!empty($nids)) ? $nids : NULL;
*/ 
