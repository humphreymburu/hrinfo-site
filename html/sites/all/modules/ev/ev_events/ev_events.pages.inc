<?php

/**
 * @file
 * Menu callbacks for the Humanitarian Events.
 */

/**
 * Callback that renders the calendar.
 */
function ev_events_calendar($node) {
  // Make sure that the og single menu is not displayed.
  context_set('context', 'hr_space_menu', FALSE);

  // Check if it's a cluster.
  $is_cluster = $node->type == 'hr_bundle';

  $operation = $is_cluster ? $op = node_load(_hr_bundles_get_operation($node->nid)) : $node;
  $operation_wrapper = entity_metadata_wrapper('node', $operation);

  // Set active menu item, it will override current_path.
  menu_set_active_item('node/' . $node->nid . '/humanitarian-events');

  $api_base_url = variable_get('ev_events_api_url', 'https://events.staging.humanitarianresponse.info');
  $settings = array(
    'base_url' => $api_base_url,
    'header' => array(
      'left' => 'prev,next today',
      'center' => 'title',
      'right' => 'month,agendaWeek,agendaDay,listYear,upcoming,past',
    ),
    'editable' => FALSE,
    'eventLimit' => TRUE,
    'firstDay' => 1,
    'defaultView' => 'month',
    'weekends' => TRUE,
    'timeFormat' => 'H:mm',
    'timezone' => drupal_get_user_timezone(),
    'buttonText' => array(
      'today' => 'Today',
    ),
    'views' => array(
      'month' => array(
        'buttonText' => 'Month',
      ),
      'agendaWeek' => array(
        'buttonText' => 'Week',
      ),
      'agendaDay' => array(
        'buttonText' => 'Day',
      ),
      'listYear' => array(
        'buttonText' => 'Year',
      ),
      'upcoming' => array(
        'buttonText' => 'Upcoming',
      ),
      'past' => array(
        'buttonText' => 'Past Events',
      ),
    ),
    'events' => array(
      'url' => $api_base_url . '/api/v0/fetch-events',
      'type' => 'GET',
      'data' => array(
        'hub' => '',
        'clu' => '',
        'cat' => '',
        'loc' => '',
      )
    ),
  );

  $settings['availableFilters'] = array(
    'hub' => '',
    'clu' => '',
    'cat' => '',
    'loc' => '',
  );

  if (isset($_GET['view'])) {
    $settings['defaultView'] = $_GET['view'];
  };

  if (isset($_GET['date'])) {
    $settings['defaultDate'] = $_GET['date'];
  };

  if (variable_get_value('ev_events_calendar_country')) {
    $settings['events']['data']['loc'] = variable_get_value('ev_events_calendar_country');
  }
  else {
    if (isset($node->field_country['und'][0]['target_id'])) {
      $term = taxonomy_term_load($node->field_country['und'][0]['target_id']);
      $settings['events']['data']['vloc'] = $term->name;
    }
  }

  if (variable_get_value('ev_events_calendar_cluster')) {
    $settings['events']['data']['clu'] = variable_get_value('ev_events_calendar_cluster');
  }

  drupal_add_library('chosen', 'drupal.chosen');
  drupal_add_js('sites/all/libraries/jspdf/dist/jspdf.min.js');
  drupal_add_js('sites/all/libraries/jspdf/jspdf.plugin.autotable.js');
  drupal_add_js(drupal_get_path('module', 'ev_events') . '/js/ev_events_calendar.js');

  return theme('fullcalendar_calendar', array(
    'calendar_id' => 'fullcalendar',
    'calendar_settings' => $settings,
  ));
}
