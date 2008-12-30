<?php
// $Id$

/**
 * @file
 * Administrative page for switching to and from the SCF installation profile. 
 *
 * Point your browser to "http://www.example.com/profiles/scf/switch.php" and
 * follow the instructions.
 *
 * If you are not logged in as administrator, you will need to modify the access
 * check statement inside your settings.php file. After finishing the upgrade,
 * be sure to open settings.php again, and change it back to its original state!
 *
 * Modeled on update.php with inspiration from page 92 of John VanDyk's Pro
 * Drupal Development book, in the Working with Databases chapter.
 */

/**
 * Global flag to identify update.php run, and so avoid various unwanted
 * operations, such as hook_init() and hook_exit() invokes, css/js preprocessing
 * and translation, and solve some theming issues. This flag is checked on several
 * places in Drupal code (not just update.php).
 */
define('MAINTENANCE_MODE', 'update');

/**
 * In which we jump through some hoops to make Drupal treat
 * profiles/scf/switch.php just like the root-level index.php or update.php
 */
// remove '/profiles/scf' from current working directory and set our directory
$base_dir = substr(getcwd(), 0, -13);
chdir($base_dir);
// ini_set('include_path', '.:' . $base_dir); - use of ./ makes this not work
global $base_url;
// duplicated from bootstrap.inc conf_init().
    // Create base URL
    $base_root = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') ? 'https' : 'http';

    // As $_SERVER['HTTP_HOST'] is user input, ensure it only contains
    // characters allowed in hostnames.
    $base_url = $base_root .= '://'. preg_replace('/[^a-z0-9-:._]/i', '', $_SERVER['HTTP_HOST']);

    // $_SERVER['SCRIPT_NAME'] can, in contrast to $_SERVER['PHP_SELF'], not
    // be modified by a visitor.
    if ($dir = trim(dirname($_SERVER['SCRIPT_NAME']), '\,/')) {
      $base_path = "/$dir";
      $base_url .= $base_path;
    }
$base_url = substr($base_url, 0, -13);

require_once './includes/bootstrap.inc';

// we need the $user variable, theming...  i think we have to go all the way
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL); // DRUPAL_BOOTSTRAP_CONFIGURATION

drupal_maintenance_theme();

$output = '';
$title = 'SCF Profile Switching Utility';

$op = isset($_REQUEST['op']) ? $_REQUEST['op'] : '';
if (empty($op)) {
  // explain what's going on
  $output .= scf_switch_info_text();
}

// Access check:
if (!empty($update_free_access) || $user->uid == 1) {
  switch ($op) {
    // switch.php ops
    default:
      $output .= '<p>Unknown operation requested.</p>';
      // default, reportedly, can be used anywhere in a switch statement:
      // http://us2.php.net/manual/en/control-structures.switch.php#78725
    case '':
      $output .= scf_switch_profile_text();
      break;
    case 'scf':
    case 'default':  // note:  this refers to the 'default' profile!
      $output .= scf_switch_profile($op);
      break;
  }
}
else {
  $output .= scf_switch_access_denied_text();
}
if (isset($output) && $output) {
  // We defer the display of messages until all updates are done.
  drupal_set_title($title);
  print theme('update_page', $output, FALSE);
}


/**
 * Send the user to a different installer page. This issues an on-site HTTP
 * redirect. Messages (and errors) are erased.
 *
 * @param $path
 *   An installer path.
 */
function scf_switch_goto($path) {
  global $base_url;
  header('Location: '. $base_url .'/'. $path);
  header('Cache-Control: no-cache'); // Not a permanent redirect.
  exit();
}


/**
 *  See which profile is enabled and offer to switch it to scf or to default.
 */
function scf_switch_profile_text() {
  global $base_url;
  $output = '';
//  $profile = db_result(db_query('SELECT   
  $install_profile_current = variable_get('install_profile', 'none');
  $output .= '<p>' . t('The currently enabled profile is <var>@install_profile_current</var>.', array('@install_profile_current' => $install_profile_current)) . '</p>';
  $output .= '<p>';
  if ($install_profile_current == 'default') {
    $install_profile_new = 'scf';
    $output .= t('To change to the Science Collaboration Framework (SCF) profile, click');
  }
  else {
    $install_profile_new = 'default';
    $output .= t('To stop using the Science Collaboration Framework (SCF) profile and change to the Drupal default profile (comes with no additional modules), click');
  }
  $submit_value = t('Switch to @install_profile_new', array('@install_profile_new' => $install_profile_new));
  
  $output .= ' "' . $submit_value . '".</p>';
//  $output .= '<a href="' . $base_url . '/profiles/scf/switch.php?op=' . $install_profile_new . '">' . $submit_value . '</a>';
  
  $output .= '
      <form method="post" action="' . $base_url . '/profiles/scf/switch.php?op=' . $install_profile_new . '"><input type="submit" value="' . $submit_value . '" /></form>
      ';

      $output .= "\n";
   return $output;
}

function scf_switch_profile($op) {
  global $base_url;
  $output = '';
  
  variable_set('install_profile', $op);
  // http://api.drupal.org/api/function/system_modules/6 returns a form
  // but we just want to refresh everything
  system_modules();
  // NOTE: we can't use l() here because the URL would point to 'update.php?q=admin'.
  $links[] = '<a href="'. $base_url .'">Main page</a>';
  $links[] = '<a href="'. $base_url .'?q=admin">Administration pages</a>';

  $output .= '<p>The profile switch was successful.  You may proceed happily to the <a href="'. $base_url .'?q=admin">administration pages</a>.</p>';

  if (!empty($GLOBALS['update_free_access'])) {
    $output .= "<p><strong>Reminder: don't forget to set the <code>\$update_free_access</code> value in your <code>settings.php</code> file back to <code>FALSE</code>.</strong></p>";
  }

  $output .= theme('item_list', $links);

  return $output;
}

function scf_switch_info_text() {
  $output = '<p>To use the modules packaged inside an installation profile, Drupal requires that the profile be active.  This utility helps make the <em>one-time</em> switch to the SCF profile for sites which were started from scratch or from a different installation profile.  We do not recommend switching back again, as your site will suddenly lose access to all modules in the SCF profile.</p>';
  return $output;
}

function scf_switch_access_denied_text($page = FALSE) {
  if ($page) {
    drupal_set_title('Access denied');
  }
  return '<p><strong>Access denied.</strong></p>
  <p>You are not authorized to access this page. Please log in as the admin user (the first user you created). If you cannot log in, you will have to edit <code>settings.php</code> to bypass this access check. To do this:</p>
<ol>
 <li>With a text editor find the settings.php file on your system. From the main Drupal directory that you installed all the files into, go to <code>sites/your_site_name</code> if such directory exists, or else to <code>sites/default</code> which applies otherwise.</li>
 <li>There is a line inside your settings.php file that says <code>$update_free_access = FALSE;</code>. Change it to <code>$update_free_access = TRUE;</code>.</li>
 <li>As soon as the update.php script is done, you must change the settings.php file back to its original form with <code>$update_free_access = FALSE;</code>.</li>
 <li>To avoid having this problem in future, remember to log in to your website as the admin user (the user you first created) before you backup your database at the beginning of the update process.</li>
</ol>';
}
