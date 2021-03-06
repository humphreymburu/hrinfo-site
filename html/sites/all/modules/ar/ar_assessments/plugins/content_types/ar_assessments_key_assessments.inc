<?php

/**
 * @file
 * Dynamic assessments plugin.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Dynamic Assessments'),
  'description' => t('Choose the Assessments you want to display.'),
  'category' => t('Assessments'),
  'edit form' => 'ar_assessments_key_assessments_edit_form',
  'render callback' => 'ar_assessments_key_assessments_render',
  'defaults' => array(
    'filters' => array(
      'operation' => '',
      'source' => '',
    ),
    'limit' => 5,
  ),
);

/**
 * Edit form callback for the content type.
 */
function ar_assessments_key_assessments_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $dropdowns = ar_assessments_get_list_of_filters();
  $form['filters'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  foreach ($dropdowns as $name => $dropdown) {
    $form['filters'][$name] = array(
      '#title' => $name,
      '#type' => 'select',
      '#options' => $dropdown,
      '#required' => FALSE,
      '#empty_value' => '',
      '#default_value' => $conf['filters'][$name],
    );
  }

  $form['limit'] = array(
    '#title' => t('Number of assessments to display'),
    '#type' => 'select',
    '#options' => array_combine(range(1, 10), range(1, 10)),
    '#required' => TRUE,
    '#default_value' => $conf['limit'],
  );

  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function ar_assessments_key_assessments_edit_form_submit($form, &$form_state) {
  $form_state['conf']['limit'] = $form_state['values']['limit'];
  foreach ($form_state['values']['filters'] as $key => $value) {
    $form_state['conf']['filters'][$key] = $value;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Run-time rendering of the body of the block (content type)
 *
 * @See ctools_plugin_examples for more advanced info.
 */
function ar_assessments_key_assessments_render($subtype, $conf, $panel_args, $context = NULL) {
  // Initial content is blank.
  $block = new stdClass();
  $block->title = 'Assessments';
  $block->content = '';

  // Override title.
  if ($conf['override_title']) {
    $block->title = $conf['override_title_text'];
  }

  $limit = $conf['limit'];

  $data = ar_assessments_fetch_assessments($conf['filters'], $conf['limit']);
  if (!empty($data['data'])) {
    $links = array();
    foreach ($data['data'] as $key => $value) {
      $links[] = array(
        'title' => $value['label'],
        'href' => $value['url'],
      );
    }
    $nid = 0;
    if ($conf['filters']['operation']) {
      $nid = $conf['filters']['operation'];
    }
    elseif ($conf['filters']['bundles']) {
      $nid = $conf['filters']['bundles'];
    }
    $read_more = '<div class="more-link read-more">';
    $read_more .= l(t('View more'), 'node/' . $nid . '/ar_assessments');
    $read_more .= '</div>';

    $block->content = $read_more . theme('links', array('links' => $links));
  }

  return $block;
}
