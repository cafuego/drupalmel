<?php
// $Id: udplanet_feeds_admin.inc,v 1.1.2.5.2.1 2010/04/28 16:03:07 mtecknology Exp $

/**
 * @file
 * Adminsitrative Menu functions
 */

/**
 * Manage Planet Feeds
 */
function udplanet_feeds_admin() {
  $lid = intval(arg(4));

  if (intval($lid) > 0) {
    $edit = db_fetch_array(db_query('SELECT * FROM {udplanet_feeds} WHERE lid = %d', intval($lid)));
    $output .= drupal_get_form('udplanet_feeds_admin_edit', $edit, TRUE);
  }
  else {
    $output .= drupal_get_form('udplanet_feeds_admin_edit', $edit, FALSE);
    $output .= '<h2>User Feeds</h2>';
    $output .= udplanet_feeds_admin_list();
  }

  return $output;
}

/**
 * Build table of feeds
 */
function udplanet_feeds_admin_list() {
  // Get list of available users
  $users_array = db_query('SELECT uid, name FROM {users} WHERE uid > 0');
  while ($user_array = db_fetch_object($users_array)) {
    $users[$user_array->uid] = $user_array->name;
  }

  $feeds = db_query('SELECT * FROM {udplanet_feeds} ORDER BY uid');

  $rows = array();

  $headers = array('User', 'Feed Title', 'Disabled', 'Frozen', 'Edit');

  while ($feed = db_fetch_object($feeds)) {
    $lid = strval($feed->lid);
    $username = $users[$feed->uid];
    $disabled = ($feed->disabled == TRUE) ? 'X' : '';
    $frozen = ($feed->frozen == TRUE) ? 'X' : '';
    $edit = l(t('edit'), 'admin/settings/udplanet/feeds/'. $lid);
    $aggregator = db_fetch_object(db_query_range('SELECT title FROM {aggregator_feed} WHERE fid="%d"', array($feed->fid), 0, 1));
    array_push($rows, array($username, $aggregator->title, $disabled, $frozen, $edit));
  }

  return theme('table', $headers, $rows);
}

/**
 * Add/Delete/Update Feeds
 */
function udplanet_feeds_admin_edit(&$form_state, $edit = array(), $individual = FALSE) {
  if ($individual) {
    $user = db_fetch_object(db_query('SELECT name
                                      FROM {udplanet_feeds}
                                      LEFT JOIN {users} ON udplanet_feeds.uid = users.uid
                                      WHERE lid = "%s"', $edit['lid']));
    $username = $user->name;
  }
  $aggregator = db_fetch_object(db_query('SELECT title, url FROM {aggregator_feed} WHERE fid="%d"', array($edit['fid']), 0, 1));
  $feed_title = $aggregator->title;
  $feed_url = $aggregator->url;

  $form = array();

  $form['feed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Feed'),
    '#collapsible' => TRUE,
    '#collapsed' => !$individual,
    '#tree' => FALSE,
  );
  $form['feed']['lid'] = array(
    '#type' => 'hidden',
    '#value' => $edit['lid'],
  );
  $form['feed']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#default_value' => $username,
    '#description' => t('User the blog will belong to.'),
    '#autocomplete_path' => 'user/autocomplete',
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
    '#maxlength' => 255,
    '#description' => t('The URL of the feed to be aggregated.'),
  );
  $form['feed']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disabled'),
    '#default_value' => $edit['disabled'],
    '#description' => t('Whether or not the blog will be aggregated. (unchecke overrides frozen)'),
  );
  $form['feed']['frozen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Frozen'),
    '#default_value' => $edit['frozen'],
    '#description' => t('Whether or not the blog will be aggregated. (unchecked overrides enabled) -- User can not modify this'),
  );

  $form['feed']['submit'] = array(
    '#type' => 'submit',
    '#value' => $individual ? 'Update' : 'Add'
  );

  if ($edit['lid'] > 0) {
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
function udplanet_feeds_admin_edit_submit($form, &$form_state, $userid = NULL) {
  $form_state['redirect'] = 'admin/settings/udplanet/feeds';
  $op  = $form_state['clicked_button']['#post']['op'];
  $lid = $form_state['clicked_button']['#post']['lid'];
  $feed_url = $form_state['clicked_button']['#post']['feed_url'];
  $feed_title = $form_state['clicked_button']['#post']['feed_title'];
  $username = $form_state['clicked_button']['#post']['username'];
  $disabled = $form_state['clicked_button']['#post']['disabled'];
  $frozen = $form_state['clicked_button']['#post']['frozen'];
  $feed = db_fetch_object(db_query_range('SELECT fid FROM {udplanet_feeds} WHERE lid="%d"', array($lid), 0, 1));
  $fid = $feed->fid;

  // Get uid from  username
  $user = db_fetch_object(db_query('SELECT uid FROM {users} WHERE name="%s"', $username));
  $uid = $user->uid;
  if (!isset($uid)) {
    form_set_error('user_404', t('Unknown user %username.', array('%username' => $username)));
    return NULL;
  }

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
    db_query('UPDATE {udplanet_feeds} SET uid="%s", fid="%s", disabled="%s", frozen="%s" WHERE lid=%d', $uid, $fid, $disabled, $frozen, $lid);
    db_query('UPDATE {aggregator_feed} SET url="%s", title="%s" WHERE fid=%d', $feed_url, $feed_title, $fid);
  }

  // Delete
  if ($op == 'Delete') {
    db_query('DELETE FROM {udplanet_feeds} WHERE lid=%d', $lid);
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
    db_query('INSERT INTO {udplanet_feeds} (uid, fid, disabled, frozen) VALUES ("%s", "%s", "%s", "%s")', $uid, $fid, $disabled, $frozen);
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
