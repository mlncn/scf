<?php


// $Id: biblio_style_apa.inc,v 1.9.2.13 2009/03/26 13:59:25 rjerome Exp $
/**
 * Get the style information
 *
 * @return
 *   The name of the style
 */
function biblio_style_apa_info() {
  return array(
    'apa' => 'American Psychological Association (APA)'
  );
}

function biblio_style_apa_author_options() {
  $author_options = array(
    'BetweenAuthorsDelimStandard'     =>  ', ',      //4
    'BetweenAuthorsDelimLastAuthor'   =>  ', & ',    //5
    'AuthorsInitialsDelimFirstAuthor' =>  ', ',      //7
    'AuthorsInitialsDelimStandard'    =>  ' ',       //8
    'betweenInitialsDelim'            =>  '. ',      //9
    'initialsBeforeAuthorFirstAuthor' =>  FALSE,     //10
    'initialsBeforeAuthorStandard'    =>  TRUE,      //11
    'shortenGivenNames'               =>  FALSE,     //12
    'numberOfAuthorsTriggeringEtAl'   =>  6,         //13
    'includeNumberOfAuthors'          =>  6,         //14
    'customStringAfterFirstAuthors'   =>  ', et al.',//15
    'encodeHTML'                      =>  true
  );
  return $author_options;
}

/**
 * Apply a bibliographic style to the node
 *
 *
 * @param $node
 *   An object containing the node data to render
 * @param $base
 *   The base URL of the biblio module (defaults to /biblio)
 * @param $inline
 *   A logical value indicating if this is being rendered within the
 *   Drupal framwork (FALSE) or we are just passing back the html (true)
 * @return
 *   The styled biblio entry
 */
function biblio_style_apa($node, $base = 'biblio', $inline = FALSE) {
  $author_options = biblio_style_apa_author_options();
  $authors = theme('biblio_format_authors', $node->biblio_contributors[1], $author_options, $inline);
  if (empty($authors)) $authors = theme('biblio_format_authors', $node->biblio_contributors[5], $author_options, $inline);  // if no authors substitute corp author if available
  if (empty($authors)) $authors = '[' . t('Anonymous') . ']';  // use anonymous if we still have nothing.
  if (!empty ($node->biblio_citekey)&&(variable_get('biblio_display_citation_key',0))) {
    $output .= '[' . check_plain($node->biblio_citekey) . '] ';
  }
  $output .= '<span class="biblio-authors">' . $authors . "</span> \n";
  $output .= (strrpos($authors, '.') == strlen($authors)) ? ".&nbsp;&nbsp;" : " ";
  switch ($node->biblio_type) {
    case 1 : // Journal Article
    case 2 : //Conference Paper
    case 3 : // are all
    case 4 :
    case 5 :
    case 6 :
    case 7 :
    case 8 :
    case 9 :
    default :
      if (isset ($node->biblio_year)) {
        $output .= "(" . check_plain($node->biblio_year) . ").&nbsp;&nbsp;";
      }
      $output .= '<span class="biblio-title-apa">';
        $link = ((variable_get('biblio_link_title_url', 0) && !empty($node->biblio_url)) ? $node->biblio_url : ($inline ? "$base/viewinline/$node->nid":"node/$node->nid" ) ) ;
        $options['attributes'] = (variable_get('biblio_links_target_new_window',null) && variable_get('biblio_link_title_url', 0) && !empty($node->biblio_url)) ? array('target'=>'_blank') : null;
        $output .= l($node->title, $link, $options);
        //$output .= $inline ? l("$node->title", "$base/viewinline/$node->nid") : l("$node->title", "node/$node->nid");
      $output .= ". </span> \n";
      $output .= !empty ($node->biblio_contributors[2]) ? '(' . theme('biblio_authors', $node->biblio_contributors[2], 'apa', 2, $inline) . ', Ed.).' : "";
      $output .= ($node->biblio_secondary_title) ? '<u>' . check_plain($node->biblio_secondary_title) . '. ' : '<u>';
      $output .= ($node->biblio_volume) ? check_plain($node->biblio_volume) . ($node->biblio_issue ? '</u>(' . check_plain($node->biblio_issue) . '),&nbsp;' : ',</u> ') : '</u> ';
      //  $output .= ($node->biblio_issue) ? '('. check_plain($node->biblio_issue).')' :'';
      $output .= ($node->biblio_pages) ? check_plain($node->biblio_pages) . '.' : '';
      break; // generic
  }
  /*  if ($node->biblio_date) $output .= ', '. check_plain($node->biblio_date);
    if ($node->biblio_number) $output .= ', Number '. check_plain($node->biblio_number);

    if ($node->biblio_place_published) $output .= ', '. check_plain($node->biblio_place_published);
  */
  return $output;
}
/**
 *
 */
function _apa_format_author($author) {
  $format = $author['prefix'] . ' ' . $author['lastname'];
  $format .= !empty ($author['firstname']) ? ' ' . drupal_substr($author['firstname'], 0, 1) . '.' : '';
  $format .= !empty ($author['initials']) ? $author['initials'] . '.' : '';
  return $format;
}