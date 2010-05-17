<?php
// $Id: udplanet_feeds_user.inc,v 1.1.2.5.2.1 2010/04/28 16:03:07 mtecknology Exp $

/**
 * @file
 * User Menu functions
 */

/**
 * Manage Planet Feeds
 */
function udplanet_feeds_user($userid) {
  if ($userid == NULL) {
    global $user;
    $userid = $user->uid;
  }

  $lid = intval(arg(4));

  if (intval($lid) > 0) {
    $edit = db_fetch_array(db_query('SELECT * FROM {udplanet_feeds} WHERE lid = %d AND uid="%s"', intval($lid), $userid));
    $output .= drupal_get_form('udplanet_feeds_user_edit', $edit, 'true', $userid);
  }
  else {
    $output .= drupal_get_form('udplanet_feeds_user_edit', $edit, 'false', $userid);
    $output .= '<h2>User Feeds</h2>';
    $output .= udplanet_feeds_user_list($userid);
  }

  return $output;
}

/**
 * Build table of feeds
 */
function udplanet_feeds_user_list($userid) {
  $feeds = db_query('SELECT * FROM {udplanet_feeds} WHERE uid="%s" ORDER BY uid', $userid);

  $rows = array();

  if (variable_get('udplanet_user_edit', TRUE)) {
    $headers = array('Feed Title', 'Disabled', 'Frozen', 'Edit');
  }
  else {
    $headers = array('Feed Title', 'Disabled', 'Frozen');
  }

  while ($feed = db_fetch_object($feeds)) {
    $lid = strval($feed->lid);
    $disabled = ($feed->disabled == TRUE) ? 'X' : '';
    $frozen = ($feed->frozen == TRUE) ? 'X' : '';
    $edit = l(t('edit'), 'user/' . $userid . '/udplanet/feeds/'. $lid);
    $aggregator = db_fetch_object(db_query_range('SELECT title FROM {aggregator_feed} WHERE fid="%d"', array($feed->fid), 0, 1));
    if (variable_get('udplanet_user_edit', TRUE)) {
      array_push($rows, array($aggregator->title, $disabled, $frozen, $edit));
    }
    else {
      array_push($rows, array($aggregator->title, $disabled, $frozen));
    }
  }

  return theme('table', $headers, $rows);
}

/**
 * Add/Delete/Update Feeds
 */
function udplanet_feeds_user_edit(&$form_state, $edit = array(), $individual = FALSE) {
  // Check permissions from Settings
  if (!variable_get('udplanet_user_edit', TRUE) && !variable_get('udplanet_user_add', TRUE)) return NULL;

  $aggregator = db_fetch_object(db_query_range('SELECT title, url FROM {aggregator_feed} WHERE fid="%d"', array($edit['fid']), 0, 1));
  $feed_title = $aggregator->title;
  $feed_url = $aggregator->url;

  $form = array();

  $form['feed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Feed'),
    '#collapsible' => TRUE,
    '#collapsed' => $edit['lid'] > 0 ? FALSE : TRUE,
    '#tree' => FALSE,
  );
  $form['feed']['lid'] = array(
    '#type' => 'hidden',
    '#value' => $edit['lid'],
  );
  $form['feed']['feed_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Feed Title'),
    '#default_value' => $feed_title,
    '#description' => t('The Title of the feed to be aggregated.'),
  );
  $form['feed']['feed_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Feed URL'),
    '#default_value' => $feed_url,
    '#description' => t('The URL of the feed to be aggregated.'),
  );
  $form['feed']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disabled'),
    '#default_value' => $edit['disabled'],
    '#description' => t('Whether or not the blog will be aggregated. (unchecke overrides frozen)'),
  );

  if (variable_get('udplanet_user_add', TRUE) && $edit['lid'] == 0) {
    $form['feed']['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Add',
    );
  }
  elseif (variable_get('udplanet_user_edit', TRUE) && $edit['lid'] > 0) {
    $form['feed']['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Update',
    );
  }

  if (variable_get('udplanet_user_edit', TRUE) && $edit['lid'] > 0) {
    $form['feed']['delete'] = array(
      '#type' => 'submit',
      '#value' => 'Delete',
    );
  }

  return $form;
}

/**
 * Do the Add/Delete/Update Modification to Feed
 */
function udplanet_feeds_user_edit_submit($form, &$form_state) {
  global $user;
  $form_state['redirect'] = 'user/' . $user->uid . '/udplanet/feeds/';
  $op  = $form_state['clicked_button']['#post']['op'];
  $lid = $form_state['clicked_button']['#post']['lid'];
  $feed_url = $form_state['clicked_button']['#post']['feed_url'];
  $feed_title = $form_state['clicked_button']['#post']['feed_title'];
  $uid = $user->uid;
  $disabled = $form_state['clicked_button']['#post']['disabled'];
  $feed = db_fetch_object(db_query_range('SELECT fid FROM {udplanet_feeds} WHERE lid="%d" AND uid="%s"', array($lid, $uid), 0, 1));
  $fid = $feed->fid;

  // Check Valid URL
  if (!valid_url($feed_url, TRUE)) {
    form_set_error('url', t('The URL %url is invalid. Please enter a fully-qualified URL, ex. http://www.example.com/feed.xml.', array('%url' => $form_state['values']['url'])));
    return NULL;
  }

  // Check for duplicates
  if ($op != 'Delete' && udplanet_check_dupes($feed_url, $feed_title, $fid)) return NULL;

  // Check that user hasn't reached max feeds
  if ($op != 'Delete' && !udplanet_check_limit($uid, $op)) return NULL;

  // Update
  if ($op == 'Update') {
    db_query('UPDATE {udplanet_feeds} SET uid="%s", disabled="%s" WHERE lid=%d', $uid, $disabled, $lid);
    db_query('UPDATE {aggregator_feed} SET url="%s", title="%s" WHERE fid=%d', $feed_url, $feed_title, $fid);
  }

  // Delete
  if ($op == 'Delete') {
    db_query('DELETE FROM {udplanet_feeds} WHERE lid=%d AND uid="%s"', $lid, $uid);
    db_query('DELETE FROM {aggregator_item} WHERE fid=%d', $fid);
    db_query('DELETE FROM {aggregator_feed} WHERE fid=%d', $fid);
    db_query('DELETE FROM {aggregator_category_feed} WHERE fid=%d', $fid);
  }

  // Add
  if ($op == 'Add') {
    db_query("INSERT INTO {aggregator_feed} (title, url, refresh, block, description, image) VALUES ('%s', '%s', 900, 5, '', '')", $feed_title, $feed_url);
    $fid = db_last_insert_id('aggregator_feed', 'fid');
    // Assign category, if set.
    if ($cid = variable_get('udplanet_feed_category', 0)) {
      db_query("INSERT INTO {aggregator_category_feed} (fid, cid)  VALUES (%d, %d)", $fid, $cid);
    }
    db_query('INSERT INTO {udplanet_feeds} (uid, fid, disabled) VALUES ("%s", "%s", "%s")', $uid, $fid, $disabled);
  }

  // Pull feed content if option is enabled
  if ($op != 'Delete') {
    if (variable_get('udplanet_cron_edit', 0)) {
      module_load_include('module', 'aggregator', 'aggregator');
      $feed = db_fetch_array(db_query("SELECT * FROM {aggregator_feed} WHERE fid=%d", $fid));
      aggregator_refresh($feed);
    }
  }

}
