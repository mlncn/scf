<?php

/**
 * Construct and Output node preview securely.
 */
function live_node_preview() {
  global $user;

  $node->type = isset($_POST['node_type']) ? $_POST['node_type'] : '';
  $node->title = isset($_POST['title']) ? $_POST['title'] : '';
  $node->teaser_include = $_POST['teaser_include'] == 0 ? 0 : 1;
  $node->teaser = isset($_POST['teaser_js']) ? trim($_POST['teaser_js']) : '';
  $body = isset($_POST['body']) ? trim($_POST['body']) : '';
  $node->body = check_markup($body, $format);
  $node->name = isset($_POST['username']) ? $_POST['username'] : '';
  $node->created = isset($_POST['date']) ? strtotime($_POST['date']) : time();

  $access = user_access('use live comment preview');
  $token = $_POST['token'] != "" ? $_POST['token'] : "";
  $skip_anonymous = $user->uid == 0 ? true : false;
  $format = isset($_POST['format']) ? $_POST['format'] : 1;
  $token_value = isset($_POST['token_value']) ? $_POST['token_value'] : '';
  $valid_token = drupal_valid_token($token, $token_value, $skip_anonymous);

  if (!filter_access($format) || !$access || !$valid_token || !node_access('create', $node) || !node_access('update', $node)) {
    // SECURITY CHECK!
    // Deny access:
    // 1) If the current user is not allowed to use specified input format; or
    // 2) If user does not have the permission.
    drupal_access_denied();
    return;
  }

  // Avoid debug information(devel.module) from being added to the preview.
  $GLOBALS['devel_shutdown'] = FALSE;

  if (!live_node_validate($node)) {
    // Report error.
    exit(drupal_json(array('html' => 0, 'error' => theme('status_messages', 'error'))));
  }

  // Load the user's name when needed.
  if (isset($node->name)) {
    // The use of isset() is mandatory in the context of user IDs, because
    // user ID 0 denotes the anonymous user.
    if ($user = user_load(array('name' => $node->name))) {
      $node->uid = $user->uid;
      $node->picture = $user->picture;
    }
    else {
      $node->uid = 0; // anonymous user
    }
  }
  else if ($node->uid) {
    $user = user_load(array('uid' => $node->uid));
    $node->name = $user->name;
    $node->picture = $user->picture;
  }

  $node->changed = time();

  // Extract a teaser, if it hasn't been set (e.g. by a module-provided
  // 'teaser' form item).
  if (!isset($node->teaser)) {
    $node->teaser = empty($node->body) ? '' : node_teaser($node->body, $node->format);
    // Chop off the teaser from the body if needed.
    if (!$node->teaser_include && $node->teaser == substr($node->body, 0, strlen($node->teaser))) {
      $node->body = substr($node->body, strlen($node->teaser));
    }
  }
  else if (trim($node->teaser) && $node->teaser_include) {
    // Space the teaser from the body
    $node->body = trim($node->teaser) . "\r\n<!--break-->\r\n" . trim($node->body);
  }
  else {
      // Empty teaser, no spaces.
      $node->body = '<!--break-->' . $node->body;
  }

  // Display a preview of the node.
  // Previewing alters $node so it needs to be cloned.
  $cloned_node = drupal_clone($node);
  $cloned_node->build_mode = NODE_BUILD_PREVIEW;
  $output = theme('live_node_preview', $cloned_node);

  exit(drupal_json(array('html' => $output, 'error' => 0)));
}

/**
 * Display a live node preview for display during node creation and editing.
 *
 * @param $node
 *   The node object which is being previewed.
 *
 * @ingroup themeable
 */
 function theme_live_node_preview($node) {
   $output = '<div class="preview">';

   $preview_trimmed_version = FALSE;
   // Do we need to preview trimmed version of post as well as full version?
   if ($node->teaser != '' && isset($node->body)) {
    $teaser = trim($node->teaser);
    $body = trim(str_replace('<!--break-->', '', $node->body));

    // Preview trimmed version if teaser and body will appear different;
    // also (edge case) if both teaser and body have been specified by the user
    // and are actually the same.
    if ($teaser != $body || ($body && strpos($node->body, '<!--break-->') === 0)) {
      $preview_trimmed_version = TRUE;
    }
  }

  if ($preview_trimmed_version) {
    $output .= '<h3>' . t('Preview trimmed version') . '</h3>';
    $output .= node_view(drupal_clone($node), 1, FALSE, 0);
    $output .= '<h3>' . t('Preview full version') . '</h3>';
    $output .= node_view($node, 0, FALSE, 0);
  }
  else {
    $output = '<h2>' . t('Preview') . '</h2><div class="preview">';
    $output .= node_view($node, 0, FALSE, 0);
    $output .= '</div>';
  }
  $output .= "</div>\n";

  return $output;
}

/**
 * Validate the passed node data.
 */
function live_node_validate($node) {
  // Convert the node to an object, if necessary.
  $node = (object)$node;
  $type = node_get_types('type', $node);
  $errors = array();

  if ($node->title == '') {
    $errors[] = t('Title field is required.');
  }

  // Make sure the body has the minimum number of words.
  // TODO : use a better word counting algorithm that will work in other languages
  if (!empty($type->min_word_count) && isset($node->body) && count(explode(' ', $node->body)) < $type->min_word_count) {
    //form_set_error('body', t('The body of your @type is too short. You need at least %words words.', array('%words' => $type->min_word_count, '@type' => $type->name)));
    $errors[] = t('The body of your @type is too short. You need at least %words words.', array('%words' => $type->min_word_count, '@type' => $type->name));
  }

  if (isset($node->nid) && (node_last_changed($node->nid) > $node->changed)) {
    //form_set_error('changed', t('This content has been modified by another user, changes cannot be saved.'));
    $errors[] = t('This content has been modified by another user, changes cannot be saved.');
  }

  if (user_access('administer nodes')) {
    // Validate the "authored by" field.
    if (!empty($node->name) && !($account = user_load(array('name' => $node->name)))) {
      // The use of empty() is mandatory in the context of usernames
      // as the empty string denotes the anonymous user. In case we
      // are dealing with an anonymous user we set the user ID to 0.
      //form_set_error('name', t('The username %name does not exist.', array('%name' => $node->name)));
      $errors[] = t('The username %name does not exist.', array('%name' => $node->name));
    }

    // Validate the "authored on" field. As of PHP 5.1.0, strtotime returns FALSE instead of -1 upon failure.
    if (!empty($node->date) && strtotime($node->date) <= 0) {
      //form_set_error('date', t('You have to specify a valid date.'));
      $errors[] = t('You have to specify a valid date.');
    }
  }

  // Do node-type-specific validation checks.
  //node_invoke($node, 'validate', $form);
  //node_invoke_nodeapi($node, 'validate', $form);

  if (count($errors)) {
    foreach ($errors as $error) {
      drupal_set_message($error, 'error');
    }
    return;
  }

  return true;
}

