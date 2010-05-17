<?php
// $Id: udplanet_content.inc,v 1.1.2.5.2.1 2010/04/28 16:03:07 mtecknology Exp $

/**
 * @file
 * This file is used to control output to the user
 */

/**
 * Page Content
 */
function udplanet_page_content() {
  drupal_set_title(variable_get('udplanet_page_title', NULL));
  drupal_add_css(drupal_get_path('module', 'udplanet') . '/udplanet-style.css');
  drupal_add_feed('planet/rss');

  $feeds = udplanet_get_items();

  foreach ($feeds as $feed) {
    if (isset($feed['content'])) {
      $planet .= udplanet_content_heading($feed['time'], $lastday);
      $planet .= theme('page_udplanet', $feed['title'], $feed['url'], $feed['heading'], $feed['link'], $feed['content'], $feed['time'], $feed['uid']);
    }
  }

  if ($planet == '') {
    $planet = 'No planet content available.';
  }

  return $planet;
}

/**
 * RSS Content
 */
function udplanet_page_rss() {
  $feeds = udplanet_get_items();

  foreach ($feeds as $feed) {
    $items .= udplanet_format_rss_item($feed['heading'], $feed['link'], $feed['content'], $feed['title'], array('pubDate' => date('r', $feed['time'])));
  }

  $url = url('planet/rss', array('absolute' => TRUE));

  $page_title = variable_get('udplanet_page_title', NULL);
  if ($page_title != NULL && $page_title != '') {
    $site_name = $page_title;
  }
  else {
    $site_name = variable_get('site_name', 'Drupal') . 'Planet';
  }

  $page_desc = variable_get('udplanet_page_desc', NULL);
  if ($page_desc != NULL && $page_desc != '') {
    $description = $page_desc;
  }
  else {
    $description = t('Planet Feeds');
  }

  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?" . ">\n";
  $output .= "<rss version=\"2.0\">\n";
  $output .= format_rss_channel(t('@site_name', array('@site_name' => $site_name)), $url, $description, $items);
  $output .= "</rss>\n";

  print $output;
}

function udplanet_format_rss_item($title, $link, $description, $author, $args = array()) {
  $output = "<item>\n";
  $output .= ' <title>'. check_plain($title) ."</title>\n";
  $output .= ' <link>'. check_url($link) ."</link>\n";
  $output .= ' <description>'. check_plain($description) ."</description>\n";
  $output .= ' <author>'. check_plain($author) ."</author>\n";
  $output .= format_xml_elements($args);
  $output .= "</item>\n";

  return $output;
}


/**
 * Function to pull the planet data from the database
 */
function udplanet_get_items() {
  $items_limit = variable_get('udplanet_feed_items', 40);

  $items = db_query('SELECT
                     {users}.uid AS uid,
                     {aggregator_feed}.title AS title,
                     {aggregator_feed}.url AS url,
                     {aggregator_item}.title AS heading,
                     {aggregator_item}.link AS link,
                     {aggregator_item}.description AS content,
                     {aggregator_item}.timestamp AS time
                     FROM {udplanet_feeds}
                     LEFT JOIN {users} ON {udplanet_feeds}.uid = {users}.uid
                     LEFT JOIN {aggregator_feed} ON {udplanet_feeds}.fid = {aggregator_feed}.fid
                     LEFT JOIN {aggregator_item} ON {aggregator_feed}.fid = {aggregator_item}.fid
                     WHERE {udplanet_feeds}.disabled IS NOT TRUE
                     AND {udplanet_feeds}.frozen IS NOT TRUE
                     AND {aggregator_item}.timestamp IS NOT NULL
                     ORDER BY time DESC
                     LIMIT %d', $items_limit);

  $feeds = array();

  while ($item = db_fetch_object($items)) {
    $feeds[] = array(
      'title' => $item->title,
      'url' => $item->url,
      'heading' => $item->heading,
      'link' => $item->link,
      'content' => $item->content,
      'time' => $item->time,
      'uid' => $item->uid,
    );
  }

  return $feeds;
}

/**
 * Function to manage change in heading
 */
function udplanet_content_heading($time, &$day_last) {
  $day_this = format_date($time, 'custom', 'F d, Y');
  if ($day_last != $day_this) {
    $day_last = $day_this;
    $output = theme('heading_udplanet', $day_this);
  }

  return $output;
}

/**
 * Function that returns block content
 *
 * This is somewhat cluttered and ugly but does what it's supposed to.
 */
function udplanet_block_content() {
  $feeds = db_query('SELECT title, url, link FROM {aggregator_feed} ORDER BY title');

  $block_content = '<div id="udp-rightColumn">';

  if (variable_get('udplanet_block_feeds', TRUE)) {
    $block_content .= theme('sidebar_udplanet_feeds_open');
    while ($feed = db_fetch_object($feeds)) {
      $block_content .= theme('sidebar_udplanet_feeds_list', $feed->title, $feed->url, $feed->link);
    }
    $block_content .= theme('sidebar_udplanet_feeds_close');
  }

  $block_content .= '</div>';

  return $block_content;
}

