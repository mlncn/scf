<?php
// Original File:       ./cite/styles/cite_AMA.php
// Original Author(s):  Matthias Steffens <mailto:refbase@extracts.de> and
//                      Richard Karnesky <mailto:karnesky@gmail.com>

// This is a citation style file (which must reside within the 'cite/styles/' sub-directory of your refbase root directory). It contains a
// version of the 'citeRecord()' function that outputs a reference list from selected records according to the citation style used by
// the American Medical Association (AMA)

// based on 'cite_Chicago.php'

// TODO: - abstracts, conference proceedings, patents, reports
//       - book/volume/report/etc titles should be formatted in heading caps
//       - don't add a dot if the abbreviated journal (or series title) ends with a dot!

// Modified for use in biblio by Ron Jerome
//
// $Id: biblio_style_ama.inc,v 1.1.2.6 2009/03/26 13:59:25 rjerome Exp $
/**
 * Get the style information
 *
 * @return
 *   The name of the style
 */
function biblio_style_ama_info() {
  return array (
    'ama' => 'American Medical Association (AMA)'
    );
}
function biblio_style_ama_author_options() {
  $author_options = array(
    'BetweenAuthorsDelimStandard'       => ', ',
    'BetweenAuthorsDelimLastAuthor'     => ', ',
    'AuthorsInitialsDelimFirstAuthor'   => ' ',
    'AuthorsInitialsDelimStandard'      => ' ',
    'betweenInitialsDelim'              => '',
    'initialsBeforeAuthorFirstAuthor'   => FALSE,
    'initialsBeforeAuthorStandard'      => FALSE,
    'shortenGivenNames'                 => TRUE,
    'numberOfAuthorsTriggeringEtAl'     => 6,
    'includeNumberOfAuthors'            => 3,
    'customStringAfterFirstAuthors'     => ', et al.',
    'encodeHTML'                        => true
  );

  return $author_options;
}

function biblio_style_ama($node, $base = 'biblio', $inline = false) {
  $markupPatternsArray = array("italic-prefix" => '<i>',
                               "italic-suffix" => '</i>',
                               "endash" => '-');
  $author_options = biblio_style_ama_author_options();
  $authors = theme('biblio_format_authors', $node->biblio_contributors[1], $author_options, $inline);
  //$editors = theme('biblio_format_authors', $node->biblio_contributors[2], $author_options, $inline);
  //if (empty($authors)) $authors = theme('biblio_authors', $node->biblio_contributors[5], 'mla', 5, $inline);  // if no authors substitute corp author if available
  //if (empty($authors)) $authors = '[' . t('Anonymous') . ']';  // use anonymous if we still have nothing.
  //$output .= '<span class="biblio-authors">' . $authors . "</span>.&nbsp; \n";
  if (!empty ($node->biblio_citekey)&&(variable_get('biblio_display_citation_key',0))) {
    $output .= '[' . check_plain($node->biblio_citekey) . '] ';
  }

  switch ($node->biblio_type) {
    case 102: //Journal Article
    case 105: //Newspaper Article
    case 106: //Magazine Article
      if(!empty($authors)) {
        if (!ereg("\. *$", $authors))
        $output .= $authors . ".";
        else
        $output .= $authors;
      }

      if (!empty($node->title))      // title
      {
        if (!empty($authors)) $output .= " ";
        $output .= '"' ;
        $link = ((variable_get('biblio_link_title_url', 0) && !empty($node->biblio_url)) ? $node->biblio_url : ($inline ? "$base/viewinline/$node->nid":"node/$node->nid" ) ) ;
        $options['attributes'] = (variable_get('biblio_links_target_new_window',null) && variable_get('biblio_link_title_url', 0) && !empty($node->biblio_url)) ? array('target'=>'_blank') : null;
        $output .= l($node->title, $link, $options);
        //$output .= $inline ? l("$node->title", "$base/viewinline/$node->nid") : l("$node->title", "node/$node->nid");
        if (!ereg("[?!.]$", $node->title)) $output .= ".";
        $output .= '"';
      }

      // From here on we'll assume that at least either the 'author' or the 'title' field did contain some contents
      // if this is not the case, the output string will begin with a space. However, any preceding/trailing whitespace will be removed at the cleanup stage (see below)

      if (!empty($node->biblio_alternate_title))      // abbreviated journal name
      $output .= " " . $markupPatternsArray["italic-prefix"] . $node->biblio_alternate_title . $markupPatternsArray["italic-suffix"];

      // if there's no abbreviated journal name, we'll use the full journal name
      elseif (!empty($node->biblio_secondary_title))      // publication (= journal) name
      $output .= " " . $markupPatternsArray["italic-prefix"] . $node->biblio_secondary_title . $markupPatternsArray["italic-suffix"];

      if ($node->biblio_type == 105 || $node->biblio_type == 106 AND !preg_match("/^\d+$/", $node->biblio_volume)) // for newspaper articles (and magazine articles if w/o volume number), volume (=month) and issue (=day) information is printed before the year
      {
        if (!empty($node->biblio_volume))      // volume (=month)
        $output .= ". " . $node->biblio_volume;

        if (!empty($node->biblio_issue))      // issue (=day)
        $output .= " " . $node->biblio_issue;

        if (!empty($node->biblio_year))      // year
        $output .= ", " . $node->biblio_year;
      }
      else // journal article (or a magazine article with volume numbers)
      {
        if (!empty($node->biblio_year))      // year
        $output .= ". " . $node->biblio_year;

        if (!empty($node->biblio_volume) || !empty($node->biblio_issue))
        $output .= ";";

        if (!empty($node->biblio_volume))      // volume
        $output .= $node->biblio_volume;

        if (!empty($node->biblio_issue))      // issue
        $output .=  "(" . $node->biblio_issue . ")";
      }

      if (!empty($node->biblio_pages))      // pages
      {
        if (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) // only add ": " if either year, volume, issue, abbrev_journal or publication isn't empty
        $output .= ":";

        $output .= theme_biblio_page_number($node->biblio_pages, $markupPatternsArray["endash"]); // function 'formatPageInfo()' is defined in 'cite.inc.php'
      }

      if ($row['online_publication'] == "yes") // this record refers to an online article
      {
        // append an optional string (given in 'online_citation') plus the current date and the DOI (or URL):

        $today = date("F j, Y");

        if (!empty($row['online_citation']))      // online_citation
        {
          if (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) // only add ":" or "," if either year, volume, issue, abbrev_journal or publication isn't empty
          {
            if (empty($node->biblio_pages))
            $output .= ":"; // print instead of pages
            else
            $output .= ","; // append to pages
          }

          $output .= $row['online_citation'];
        }

        if (!empty($row['doi']))      // doi
        {
          if (!empty($row['online_citation']) OR (empty($row['online_citation']) AND (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)))) // only add "." if online_citation isn't empty, or else if either year, volume, issue, abbrev_journal or publication isn't empty
          $output .= ".";

          if ($encodeHTML)
          $output .= " " . encodeHTML("http://dx.doi.org/" . $row['doi']) . ". Accessed " . $today;
          else
          $output .= " " . "http://dx.doi.org/" . $row['doi'] . ". Accessed " . $today;
        }
        elseif (!empty($row['url']))      // url
        {
          if (!empty($row['online_citation']) OR (empty($row['online_citation']) AND (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)))) // only add "." if online_citation isn't empty, or else if either year, volume, issue, abbrev_journal or publication isn't empty
          $output .= ".";

          if ($encodeHTML)
          $output .= " " . encodeHTML($row['url']) . ". Accessed " . $today;
          else
          $output .= " " . $row['url'] . ". Accessed " . $today;
        }

      }

      if (!ereg("\. *$", $output))
      $output .= ".";

      break;
    case 101: //Book Chapter
    case 103: //Conference Paper
      if(!empty($authors)) {
        if (!ereg("\. *$", $authors)) {
          $output .= $authors . ".";
        }
        else {
          $output .= $authors;
        }
      }
      if (!empty($node->title))      // title
      {
        if (!empty($authors))  $output .= " ";

        $output .= '"<i>' ;
        $link = ((variable_get('biblio_link_title_url', 0) && !empty($node->biblio_url)) ? $node->biblio_url : ($inline ? "$base/viewinline/$node->nid":"node/$node->nid" ) ) ;
        $options['attributes'] = (variable_get('biblio_links_target_new_window',null) && variable_get('biblio_link_title_url', 0) && !empty($node->biblio_url)) ? array('target'=>'_blank') : null;
        $output .= l($node->title, $link, $options);
        //$output .= $inline ? l("$node->title", "$base/viewinline/$node->nid") : l("$node->title", "node/$node->nid");
        $output .= '</i>';
        if (!ereg("[?!.]$", $node->title)) $output .= ".";
        $output .= '"';
      }


      if (!empty($node->biblio_contributors[2]))      // editor
      {
        $editor_options = array(
          'BetweenAuthorsDelimStandard'       => ', ',
          'BetweenAuthorsDelimLastAuthor'     => ', ',
          'AuthorsInitialsDelimFirstAuthor'   => ' ',
          'AuthorsInitialsDelimStandard'      => ' ',
          'betweenInitialsDelim'              => '',
          'initialsBeforeAuthorFirstAuthor'   => FALSE,
          'initialsBeforeAuthorStandard'      => FALSE,
          'shortenGivenNames'                 => TRUE,
          'numberOfAuthorsTriggeringEtAl'     => 6,
          'includeNumberOfAuthors'            => 3,
          'customStringAfterFirstAuthors'     => ' et al.',
          'encodeHTML'                        => true
        );

        $editor = theme('biblio_format_authors', $node->biblio_contributors[2], $editor_options, $inline);

        $output .= " In: " . $editor . ", ";
        if (count ($node->biblio_contributors[2]) > 1) // there are at least two editors (separated by ';')
        $output .= "eds";
        else // there's only one editor (or the editor field is malformed with multiple editors but missing ';' separator[s])
        $output .= "ed";
      }

      $publication = ereg_replace("[ \r\n]*\(Eds?:[^\)\r\n]*\)", "", $node->biblio_secondary_title);
      if (!empty($publication))      // publication
      {
        if (!ereg("[?!.] *$", $output))
        $output .= ".";

        if (empty($editor))
        $output .= " In:";

        // TODO: container titles should be formatted in heading caps, however, this doesn't yet work correctly if the publication title contains HTML entities
        $output .= " " . $markupPatternsArray["italic-prefix"] . $publication . $markupPatternsArray["italic-suffix"];
        //        $output .= " " . $markupPatternsArray["italic-prefix"] . changeCase("heading", $publication) . $markupPatternsArray["italic-suffix"]; // function 'changeCase()' is defined in 'include.inc.php'
      }

      if (!empty($node->biblio_volume))      // volume
      {
        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $output))
        $output .= ".";

        $output .= " Vol. " . $node->biblio_volume;
      }

      if (!empty($node->biblio_edition) && !preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition))      // edition
      {
        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $output))
        $output .= ".";

        if (preg_match("/^\d{1,3}$/", $node->biblio_edition)) // if the edition field contains a number of up to three digits, we assume it's an edition number (such as "2nd ed.")
        {
          if ($node->biblio_edition == "2")
          $editionSuffix = "nd";
          elseif ($node->biblio_edition == "3")
          $editionSuffix = "rd";
          else
          $editionSuffix = "th";
        }
        else
        $editionSuffix = "";

        if (!preg_match("/( ed\.?| edition)$/i", $node->biblio_edition))
        $editionSuffix .= " ed.";

        $output .= " " . $node->biblio_edition . $editionSuffix;
      }

      if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $output))
      $output .= ".";

      if (!empty($node->biblio_place_published))      // place
      $output .= " " . $node->biblio_place_published;

      if (!empty($node->biblio_publisher))      // publisher
      {
        if (!empty($node->biblio_place_published))
        $output .= ":";

        $output .= " " . $node->biblio_publisher;
      }

      if (!empty($node->biblio_year))      // year
      $output .= "; " . $node->biblio_year;

      if (!empty($node->biblio_pages))      // pages
      $output .= ":" . theme('biblio_page_number', $node->biblio_pages, $markupPatternsArray["endash"]); // function 'formatPageInfo()' is defined in 'cite.inc.php'

      if (!empty($node->biblio_alternate_title) OR !empty($node->biblio_tertiary_title)) // if there's either a full or an abbreviated series title
      {
        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $output))
        $output .= ".";

        $output .= " ";

        if (!empty($node->biblio_alternate_title))
        $output .= $node->biblio_alternate_title;      // abbreviated series title

        // if there's no abbreviated series title, we'll use the full series title instead:
        elseif (!empty($node->biblio_tertiary_title))
        $output .= $node->biblio_tertiary_title;      // full series title

        if (!empty($node->biblio_volume)||!empty($node->biblio_issue))
        $output .= " ";

        if (!empty($node->biblio_volume))      // series volume
        $output .= $node->biblio_volume;

        if (!empty($node->biblio_issue))      // series issue (I'm not really sure if -- for this cite style -- the series issue should be rather omitted here)
        $output .= "(" . $node->biblio_issue . ")"; // is it correct to format series issues similar to journal article issues?
      }

      if (!ereg("\. *$", $output))
      $output .= ".";

      break;

    default : // all other types
      //TODO
      //        if (ereg("[ \r\n]*\(ed\)", $node->author)) // single editor
      //          $author = $author . ", ed";
      //        elseif (ereg("[ \r\n]*\(eds\)", $node->author)) // multiple editors
      //          $author = $author . ", eds";

      if (!empty($authors))      // author
      {
        if (!ereg("\. *$", $authors))
        $output .= $authors . ".";
        else
        $output .= $authors;
      }

      if (!empty($node->title))      // title
      {
        if (!empty($authors))
        $output .= " ";

        $output .= '<i>';
        // TODO: book/volume/report/etc titles should be formatted in heading caps, however, this doesn't yet work correctly if the publication title contains HTML entities
        $link = ((variable_get('biblio_link_title_url', 0) && !empty($node->biblio_url)) ? $node->biblio_url : ($inline ? "$base/viewinline/$node->nid":"node/$node->nid" ) ) ;
        $options['attributes'] = (variable_get('biblio_links_target_new_window',null) && variable_get('biblio_link_title_url', 0) && !empty($node->biblio_url)) ? array('target'=>'_blank') : null;
        $output .= l($node->title, $link, $options);
        //$output .= $inline ? l("$node->title", "$base/viewinline/$node->nid") : l("$node->title", "node/$node->nid");
        $output .= '</i>';
      }
      if ($node->biblio_type == "Software") // for software, add software label
      $output .= " [computer program]";

      if (!empty($node->biblio_volume) AND ($node->biblio_type != "Software"))      // volume
      {
        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $output))
        $output .= ".";

        $output .= " Vol. " . $node->biblio_volume;
      }

      if (!empty($node->biblio_edition))      // edition
      {
        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $output))
        $output .= ".";

        if ($row['type'] == "Software")      // software edition (=version)
        {
          $output .= " Version " . $node->biblio_edition;
        }
        elseif (!preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition))      // edition
        {
          if (preg_match("/^\d{1,3}$/", $node->biblio_edition)) // if the edition field contains a number of up to three digits, we assume it's an edition number (such as "2nd ed.")
          {
            if ($node->biblio_edition == "2")
            $editionSuffix = "nd";
            elseif ($node->biblio_edition == "3")
            $editionSuffix = "rd";
            else
            $editionSuffix = "th";
          }
          else
          $editionSuffix = "";

          if (!preg_match("/( ed\.?| edition)$/i", $node->biblio_edition))
          $editionSuffix .= " ed.";

          $output .= " " . $node->biblio_edition . $editionSuffix;
        }
      }

      if (count($node->biblio_contributors[2]))      // editor (if different from author, see note above regarding the check for ' (ed)' or ' (eds)')
      {

        $editor_options = array(
          'BetweenAuthorsDelimStandard'     => ', ',
          'BetweenAuthorsDelimLastAuthor'   => ', ',
          'AuthorsInitialsDelimFirstAuthor' => ' ',
          'AuthorsInitialsDelimStandard'    => ' ',
          'betweenInitialsDelim'            => '',
          'initialsBeforeAuthorFirstAuthor' => FALSE,
          'initialsBeforeAuthorStandard'    => FALSE,
          'shortenGivenNames'               => TRUE,
          'numberOfAuthorsTriggeringEtAl'   => 6,
          'includeNumberOfAuthors'          => 3,
          'customStringAfterFirstAuthors'   => ' et al.',
          'encodeHTML'                      => true
        );

        $editor = theme('biblio_format_authors', $node->biblio_contributors[2], $editor_options, $inline);
        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $output))
        $output .= ".";

        $output .= " " . $editor;
        if (count ($node->biblio_contributors[2]) > 1) // there are at least two editors (separated by ';')
        $output .= ", eds";
        else // there's only one editor (or the editor field is malformed with multiple editors but missing ';' separator[s])
        $output .= ", ed";
      }

      if (!empty($row['thesis']))      // thesis
      // TODO: do we need to use the term "[dissertation]" instead of "[Ph.D. thesis]", etc? What about other thesis types then?
      $output .= " [" . $row['thesis'] . "]";

      if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $output))
      $output .= ".";

      if (!empty($node->biblio_place_published))      // place
      $output .= " " . $node->biblio_place_published;

      if (!empty($node->biblio_publisher))      // publisher
      {
        if (!empty($node->biblio_place_published))
        $output .= ":";

        $output .= " " . $node->biblio_publisher;
      }

      $output .= ";";

      if ($row['type'] == "Software")      // for software, volume (=month) and issue (=day) information is printed before the year (similar to newspaper articles)
      {
        if (!empty($node->biblio_volume))      // volume (=month)
        $output .= " " . $node->biblio_volume;

        if (!empty($node->biblio_issue))      // issue (=day)
        $output .= " " . $node->biblio_issue;

        $output .= ",";
      }

      if (!empty($node->biblio_year))      // year
      $output .= " " . $node->biblio_year;

      if (!empty($node->biblio_alternate_title) OR !empty($node->biblio_tertiary_title)) // if there's either a full or an abbreviated series title
      {
        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $output))
        $output .= ".";

        $output .= " ";

        if (!empty($node->biblio_alternate_title))
        $output .= $node->biblio_alternate_title;      // abbreviated series title

        // if there's no abbreviated series title, we'll use the full series title instead:
        elseif (!empty($node->biblio_tertiary_title))
        $output .= $node->biblio_tertiary_title;      // full series title

        if (!empty($node->biblio_volume)||!empty($node->biblio_issue))
        $output .= " ";

        if (!empty($node->biblio_volume))      // series volume
        $output .= $node->biblio_volume;

        if (!empty($node->biblio_issue))      // series issue (I'm not really sure if -- for this cite style -- the series issue should be rather omitted here)
        $output .= "(" . $node->biblio_issue . ")"; // is it correct to format series issues similar to journal article issues?
      }

      if ($row['online_publication'] == "yes" || $row['type'] == "Software") // this record refers to an online article, or a computer program/software
      {
        // append an optional string (given in 'online_citation') plus the current date and the DOI (or URL):

        $today = date("F j, Y");

        if (!empty($row['online_citation']))      // online_citation
        {
          if (!ereg("\. *$", $output))
          $output .= ".";

          $output .= $row['online_citation'];
        }

        if (!empty($row['doi']))      // doi
        {
          if (!ereg("\. *$", $output))
          $output .= ".";

          if ($encodeHTML)
          $output .= " " . encodeHTML("http://dx.doi.org/" . $row['doi']) . ". Accessed " . $today;
          else
          $output .= " " . "http://dx.doi.org/" . $row['doi'] . ". Accessed " . $today;
        }
        elseif (!empty($row['url']))      // url
        {
          if (!ereg("\. *$", $output))
          $output .= ".";

          if ($encodeHTML)
          $output .= " " . encodeHTML($row['url']) . ". Accessed " . $today;
          else
          $output .= " " . $row['url'] . ". Accessed " . $today;
        }

      }

      if (!ereg("\. *$", $output))
      $output .= ".";
      break;
  }
  return $output;
}

