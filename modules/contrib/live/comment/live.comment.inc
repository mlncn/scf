<?php

/**
 * Construct and Output comment preview securely.
 */
function live_comment_preview() {
  global $user;

  $format = isset($_POST['format']) ? $_POST['format'] : 1;
  $body = isset($_POST['body']) ? $_POST['body'] : '';
  $access = user_access('use live comment preview');
  $token = $_POST['token'] != "" ? $_POST['token'] : "";
  $skip_anonymous = $user->uid == 0 ? true : false;
  $token_value = isset($_POST['token_value']) ? $_POST['token_value'] : '';
  $valid_token = drupal_valid_token($token, $token_value, $skip_anonymous);

  if (!filter_access($format) || $body == "" || !$access || !$valid_token) {
    // SECURITY CHECK!
    // Deny access:
    // 1) If the current user is not allowed to use specified input format; or
    // 2) If the comment body is empty; or
    // 3) If user does not have the permission.
    drupal_access_denied();
    return;
  }

  $username = isset($user->name) ? $user->name : '';
  $comment->name = $_POST['name'] ? $_POST['name'] : $username;
  $comment->homepage = $_POST['homepage'] ? $_POST['homepage'] : '';

  $comment->comment = check_markup($body, $format);
  $comment->subject = $_POST['subject'] ? $_POST['subject'] : trim(truncate_utf8(decode_entities(strip_tags($comment->comment)), 29, TRUE));

  $comment->uid = isset($user->uid) ? $user->uid : null;
  $comment->timestamp = time();

  $output = '<h2>'. t('Comment Preview') .'</h2><div class="preview">';
  $output .= theme('comment', $comment, array());
  $output .= '</div>';

  // Avoid debug information(devel.module) from being added to the preview.
  $GLOBALS['devel_shutdown'] = FALSE;

  exit(drupal_json(array('html' => $output)));
}
