<?php
// $Id: udplanet_theme.inc,v 1.1.2.2.2.1 2010/04/28 16:03:07 mtecknology Exp $

/**
 * @file
 * This file is used to hold all themed output functions
 */

/**
 * Custom theme function: page output
 */
function theme_page_udplanet($title, $url, $heading, $link, $content, $time, $uid) {
  $timestamp = format_date($time, 'custom', 'F d, Y g:i A');
  $picture = theme('user_picture', user_load($uid));

  $output  = '<div class="udp-entry">';
  $output .= '  <div class="udp-person-info">';
  $output .= '    <a href="' . $url . '">';
  $output .= '      ' . $picture;
  $output .= '      <br />' . $title;
  $output .= '    </a>';
  $output .= '  </div>';
  $output .= '  <div class="udp-post">';
  $output .= '    <div class="udp-post2">';
  $output .= '      <div class="udp-post-header">';
  $output .= '        <h4 class="udp-post-title">';
  $output .= '          <a href="' . $link . '">' . $heading . '</a>';
  $output .= '        </h4>';
  $output .= '      </div>';
  $output .= '      <div class="udp-post-contents">';
  $output .= '        ' . $content;
  $output .= '      </div>';
  $output .= '      <div class="udp-post-footer">';
  $output .= '        <p>';
  $output .= '          <a href="' . $link . '">' . $timestamp . '</a>';
  $output .= '        </p>';
  $output .= '      </div>';
  $output .= '    </div>';
  $output .= '  </div>';
  $output .= '</div>';

  return $output;
}

/**
 * Custom theme function: date heading
 */
function theme_heading_udplanet($date) {
  $output  = '<h2 class="udp-date">';
  $output .=   $date;
  $output .= '</h2>';

  return $output;
}

/**
 * Custom theme function: block feeds
 */
function theme_sidebar_udplanet_feeds_open() {
  $output  = '<div class="udp-inside">';
  $output .= '  <ul>';

  return $output;
}

/**
 * Custom theme function: block feeds
 */
function theme_sidebar_udplanet_feeds_list($title, $feed, $link) {
  $output  = '    <li>';
  $output .= '      <a href="' . $link . '">' . $title . '</a> <a href="' . $feed . '"> (feed)</a>';
  $output .= '    </li>';

  return $output;
}

/**
 * Custom theme function: block feeds
 */
function theme_sidebar_udplanet_feeds_close() {
  $output  = '  </ul>';
  $output .= '</div>';

  return $output;
}

