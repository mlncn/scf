<?php
// $Id: crossref_unixref_parser.inc,v 1.1.2.2 2009/04/01 18:12:18 rjerome Exp $
function unixref_startElement($parser, $name, $attrs) {
  global $node, $element, $auth_category, $contrib_count, $org_count, $titles, $periodical, $dates, $urls, $keyword_count;
  switch ($name) {
    case 'doi_record' :
      $node = array();
      break;
    case 'error':
    case 'book':
    case 'journal':
    case 'standard':
    case 'conference':
    case 'report-paper':
    case 'dissertation':
    case 'database':
    case 'sa_component':
      $node['biblio_type'] = _unixref_type_map($name);
      break;
    case 'person_name' :
      $auth_category = _unixref_get_contributor_category($attrs['contributor_role']);
      if (!isset($contrib_count))
        $contrib_count = 0;
      break;
    case 'organization':
      if (!isset($org_count))
        $org_count = 0;
      break;
    case 'year' :
    case 'pub-dates' :
    case 'copyright-dates' :
      $dates = $name;
      break;
    case 'web-urls' :
    case 'pdf-urls' :
    case 'text-urls' :
    case 'related-urls' :
    case 'image-urls' :
      $urls = $name;
      break;
    default :
      $element = $name;
  }
}
function unixref_endElement($parser, $name) {
  global $node, $nids, $element, $terms, $batch_proc, $session_id, $auth_category, $contrib_count, $org_count, $dates, $urls, $keyword_count;
  switch ($name) {
    case 'doi_record' :
      break;
    case 'person_name' :
      $node['biblio_contributors'][$auth_category][$contrib_count]['auth_type'] = _biblio_get_auth_type($auth_category, $node['biblio_type']);
      $auth_category = '';
      $contrib_count++;
      break;
    case 'organization' :
      $node['biblio_contributors'][5][$org_count]['auth_type'] = _biblio_get_auth_type(5, $node['biblio_type']);
      $org_count++;
      break;
    case 'year' :
    case 'pub-dates' :
    case 'copyright-dates' :
      $dates = '';
      break;
    case 'web-urls' :
    case 'pdf-urls' :
    case 'text-urls' :
    case 'related-urls' :
    case 'image-urls' :
      $urls = '';
      break;
    case 'style' :
      break;
    default :
  }
  $element = '';

}
function unixref_characterData($parser, $data) {
  global $node, $element, $auth_category, $contrib_count, $org_count, $dates, $urls, $keyword_count;
  if (trim($data)) {
    switch ($element) {
      case 'doi':
        $node['biblio_doi'] .= $data;
        break;
        //Author information
      case 'surname' :
        $node['biblio_contributors'][$auth_category][$contrib_count]['lastname'] .= $data;
        break;
      case 'given_name' :
        $node['biblio_contributors'][$auth_category][$contrib_count]['firstname'] .= $data;
        break;
      case 'suffix':
        $node['biblio_contributors'][$auth_category][$contrib_count]['suffix'] .= $data;
        break;
      case 'affiliation' :
        $node['biblio_contributors'][$auth_category][$contrib_count]['affiliation'] .= $data;
        break;
      case 'organization':
        $node['biblio_contributors'][5][$org_count]['name'] .= $data;
        break;
      case 'title' :
        $node['title'] .= $data;
        break;
      case 'conference_name' :
        $node['biblio_secondary_title'] .= $data;
        break;
      case 'tertiary-title' :
        $node['biblio_tertiary_title'] .= $data;
        break;
      case 'alt-title' :
        $node['biblio_alternate_title'] .= $data;
        break;
      case 'short-title' :
        $node['biblio_short_title'] .= $data;
        break;
      case 'translated-title' :
        $node['biblio_translated_title'] .= $data;
        break;
        // Periodical information
      case 'full-title' :
        break;
      case 'abbr-1' :
        break;
      case 'abbr-2' :
        break;
      case 'abbr-3' :
        $node[$periodical] .= $data;
        break;
      case 'pages' :
        $node['biblio_pages'] .= $data;
        break;
        //Volume, issue, and edition information
      case 'volume' :
        $node['biblio_volume'] .= $data;
        break;
      case 'number' :
        $node['biblio_number'] .= $data;
        break;
      case 'issue' :
        $node['biblio_issue'] .= $data;
        break;
      case 'secondary-volume' :
        break;
      case 'secondary-issue' :
        break;
      case 'num-vols' :
        $node['biblio_number_of_volumes'] .= $data;
        break;
      case 'edition_number' :
        $node['biblio_edition'] .= $data;
        break;
      case 'section' :
        $node['biblio_section'] .= $data;
        break;
      case 'reprint-edition' :
        $node['biblio_reprint_edition'] .= $data;
        break;
      case 'reprint-status' :
        break;
      case 'dates' :
        switch ($dates) {
          case 'year' :
            $node['biblio_year'] .= $data;
            break;
        }
        break;
      case 'date' :
        switch ($dates) {
          case 'pub-dates' :
            $node['biblio_date'] .= $data;
            break;
          case 'copyright-dates' :
            break;
        }
        break;
      case 'publisher_place' :
      case 'conference_location':
        $node['biblio_place_published'] .= $data;
        break;
      case 'publisher_name' :
        $node['biblio_publisher'] .= $data;
        break;
      case 'orig-pub' :
        break;
      case 'isbn' :
        $node['biblio_isbn'] .= $data;
        break;
      case 'issn' :
        $node['biblio_issn'] .= $data;
        break;
      case 'accession-num' :
        $node['biblio_accession_number'] .= $data;
        break;
      case 'call-num' :
        $node['biblio_call_number'] .= $data;
        break;
      case 'report-id' :
        break;
      case 'coden' :
        break;
      case 'electronic-resource-num' :
        break;
      case 'abstract' :
        $node['biblio_abst_e'] .= $data;
        break;
      case 'label' :
        $node['biblio_label'] .= $data;
        break;
      case 'image' :
        break;
      case 'caption' :
        break;
      case 'notes' :
        $node['biblio_notes'] .= $data;
        break;
      case 'research-notes' :
        $node['biblio_notes'] .= $data;
        break;
      case 'work-type' :
        $node['biblio_type_of_work'] .= $data;
        break;
      case 'remote-database-name' :
        $node['biblio_remote_db_name'] .= $data;
        break;
      case 'remote-database-provider' :
        $node['biblio_remote_db_provider'] .= $data;
        break;
      case 'language' :
        $node['biblio_lang'] .= $data;
        break;
      case 'urls' :
      case 'url' :
        switch ($urls) {
          case 'web-urls' :
          case 'pdf-urls' :
          case 'text-urls' :
          case 'image-urls' :
            break;
          case 'related-urls' :
            $node['biblio_url'] .= $data;
            break;
        }
        break;
      case 'access-date' :
        $node['biblio_access_date'] .= $data;
        break;
      case 'modified-date' :
        break;
    }
  }
}

function _unixref_type_map($type){
  static $map = array();
  if (empty($map)) {
    $map = array(
    'error'        => 0,
    'book'         => 100, // Book
    'journal'      => 102, // Journal Article
    'standard'     => 129, // Generic
    'conference'   => 103, // conference_paper
    'report-paper' => 109, // Report
    'dissertation' => 108, // Thesis
    'database'     => 125, // online database
    'sa_component' => 129
   );
  }
  return (isset($map[$type]))?$map[$type]:129; //return the biblio type or 129 (Misc) if type not found
}
function _unixref_get_contributor_category($role) {
  if ($role == 'author') return 1;
  if ($role == 'editor') return 2;
  if ($role == 'chair') return 3;
  if ($role == 'translator') return 4;
  return NULL;
}