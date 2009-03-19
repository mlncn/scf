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
 * @see biblioreference_pubmed_import_validate()
 * @see biblioreference_pubmed_import_submit()
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
  // Get the vocabularies  attached to the biblio node type ...
  $vocabularies = module_invoke('taxonomy', 'get_vocabularies', 'biblio');
  // ... and print a form to select the terms in each of them
  $form['import_taxonomy'] = array (
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Taxonomy Settings'),
    '#description' => t('Typically you don\'t have to do anything here, however if you wish, you may select terms to be assigned to imported records, this effectively adds a key word to all entries being imported. By default, all incoming key words are added to the Biblio Keywords vcabulary, you may either relate incoming entires to an existing key word OR select a term(s) from another vocabulary to relate all incoming entires to.')
  );
  if (count($vocabularies)) {
    $i = 0;
    foreach ($vocabularies as $vocabulary) {
      $form['import_taxonomy']['vocabulary'. $i] = module_invoke('taxonomy', 'form', $vocabulary->vid, 0);
      $form['import_taxonomy']['vocabulary'. $i++]['#description'] = t("Select taxonomy term(s) to assign to imported entries");
    }
  } else {
    $vocab_msg = t('There are currently no vocabularies assigned to the biblio node type, please go the the !url page to change this', array ('!url' => l(t('admin/content/taxonomy'), 'admin/content/taxonomy')));
    $form['import_taxonomy']['vocabulary_message'] = array (
      '#value' => '<p><div>' . $vocab_msg . '</div></p>'
    );
  }
  $form['button'] = array ('#type' => 'submit', '#value' => t('Import from PubMed'));
  return $form;
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
  $i = 0;
  while (isset($form_state['values']['vocabulary' . $i]) && is_array($form_state['values']['vocabulary' . $i])) {
    // @REVIEW - second check for array unnecessary?
    $term_ids = array_merge($term_ids, $form_state['values']['vocabulary' . $i]);
    $i++;
  }
    // @debug
    drupal_set_message('term_ids: <pre>'.var_export($term_ids,TRUE).'</pre>');
  foreach ($pmids AS $pmid) {
    biblioreference_import_bibtext_by_pmid($pmid, $term_ids);
  }
}

/**
 * Import and save a Pubmed document to Biblio by Pubmed ID.
 *
 * Optionally take an array of Drupal taxonomy term IDs to attach to the biblio
 * node when saving.
 */
function biblioreference_import_bibtext_by_pmid($pmid, $term_ids = array()) {
  $result = biblioreference_load_bibtext_by_pmid($pmid);
  $watchdog_level = (isset($result['watchdog_level'])) ? $result['watchdog_level'] : WATCHDOG_INFO;
  watchdog('biblioreference', $result['watchdog_status'], $watchdog_level);
  $article_bib = $result['article_bib'];
  
  $data = $result['article_bib'];

  $nids = biblioreference_bibtex_import($data);

  foreach ($nids as $node_id) {
    if (count($tid)) {
      module_invoke('taxonomy', 'node_save', $node_id, $term_ids);
    }
    db_query('UPDATE {node} SET uid = %d WHERE nid = %d', $uid, $node_id);
    db_query('UPDATE {node_revisions} SET uid = %d WHERE nid = %d', $uid, $node_id);
    
    // Not worth the db query to get the title, but here's the link.
    drupal_set_message(t('Imported <a href="@url">bibliographic citation</a>.', array('@url' => url('node/' . $node_id))));
  }

}


/**
 * Load a single bubtext by pubmed ID.
 *
 * Taken directly from gopubmed.module (our upgraded to D6 version), function
 * gopubmed_load_bibtext_by_pmid($primaryId, $db = 'pubmed').
 * 
 * @TODO: break into smaller functions, maybe move to another module
 * or best get changes incorporated into gopubmed and keep dependency.
 */
function biblioreference_load_bibtext_by_pmid($primaryId, $db = 'pubmed') {
  $results = array();
  $article_bib = '';
  $sql_pmid = db_result(db_query("SELECT COUNT(*) from {biblio} WHERE `biblio_custom3` = %d", $primaryId));
  if ($sql_pmid == 0) {
    $results['watchdog_status'] = t("This is new primary ID: @pmid", array('@pmid' => $primaryId));
    // ESummary Retrieves document summaries from a list of primary IDs
    // which we got from the ESearch
    $esummary_url = "http://eutils.ncbi.nlm.nih.gov/entrez/eutils/esummary.fcgi?db=$db&id=$primaryId&retmode=xml";
    $esummary_load = simplexml_load_file($esummary_url);
    $summaryid = $esummary_load->DocSum[0]->Id;
    $article_link = biblioreference_pubmed_article_link($primaryId);  
    // Now creating the pubmed.bib file
    $article_bib .= '@article{' . $summaryid . ',' . "\n";

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
        
    $title = str_replace("{"," ",$title);
    $title = str_replace("}"," ",$title);
        
    $article_bib .= 'title = {' . $title  . '},'. "\n";
    $article_bib .= 'year = {' . $pubdate . '},'. "\n";
    $article_bib .= 'journal = {' . $journalname . '},'. "\n";
    $article_bib .= 'volume = {' . $volume . '},'. "\n";
    if ($number != '') {
      $article_bib .= 'number = {' . $number . '},'. "\n";
    }
    $article_bib .= 'pages={' . $pages . '},'. "\n";
    if ($note != '') {
      $article_bib .= 'note = {' . $note . ' Additional Keywords: ' . $keywordlist . '},'. "\n";
    }
    $article_bib .= 'keywords = {' . $keywords . '},'. "\n";  
    $article_bib .= 'isbn={' . $isbn . '},'. "\n";
    $article_bib .= 'language = {' . $language . '},'. "\n";
    $article_bib .= 'abstract = {' . $final_abstract . '},'. "\n";
    $article_bib .= 'url = {' . $article_link . '},' . "\n";
    $article_bib .= 'custom1 = {' . $custom1 . '},' . "\n";
    $article_bib .= 'custom2 = {' . $search_term . '},' . "\n";
    $article_bib .= 'custom3 = {' . $primaryId . '}' . "\n";
    $article_bib .= '}';

    $results['article_bib'] = $article_bib;      
  }
  else {
    // @TODO - override old data by default
    $results['watchdog_status'] = t("Publication not entered. PMID @pmid already there.", array('@pmid' => $primaryId));
  }

  return $results;
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
 * Import bibtex data.
 *
 * @param $data
 *   the contents of a bibtex file passed as one big string
 * @param $node
 *   an array (populated in biblio_import() ), containing the boiler plate
 *   information common to all nodes
 * @return
 *   an array of node ids
 */
function biblioreference_bibtex_import($data, $batch = NULL, $session_id = NULL) {
  $nids = array();
  module_load_include('php', 'biblio', 'bibtexParse/PARSEENTRIES');
  $bibtex = new PARSEENTRIES();  
  $bibtex->loadBibtexString($data);
  
//  $bibtex->openBib($file->filepath);
  $bibtex->extractEntries();
  if ($bibtex->count) {
    $nids = $bibtex->bib2node($batch, $session_id);
  }
  return $nids;
}
