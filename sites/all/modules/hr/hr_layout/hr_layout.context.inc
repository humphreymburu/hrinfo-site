<?php
/**
 * @file
 * hr_layout.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function hr_layout_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'outside_space';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'context_og_condition_group_type' => array(
      'values' => array(
        '_none_' => '_none_',
      ),
      'options' => array(
        'node_form' => 0,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-hr_search-page' => array(
          'module' => 'views',
          'delta' => '-exp-hr_search-page',
          'region' => 'branding',
          'weight' => '-10',
        ),
        'system-main-menu' => array(
          'module' => 'system',
          'delta' => 'main-menu',
          'region' => 'navigation',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;
  $export['outside_space'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'sitewide';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'sitewide' => array(
      'values' => array(
        1 => 1,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'hr_layout-hr_layout_login' => array(
          'module' => 'hr_layout',
          'delta' => 'hr_layout_login',
          'region' => 'top',
          'weight' => '-8',
        ),
        'system-user-menu' => array(
          'module' => 'system',
          'delta' => 'user-menu',
          'region' => 'top',
          'weight' => '-7',
        ),
        'hr_help-hr_help' => array(
          'module' => 'hr_help',
          'delta' => 'hr_help',
          'region' => 'top',
          'weight' => '-5',
        ),
        'hr_assessments-hr_assessment_create' => array(
          'delta' => 'hr_assessment_create',
          'module' => 'hr_assessments',
          'region' => 'top',
          'weight' => -2,
        ),
        'locale-language' => array(
          'module' => 'locale',
          'delta' => 'language',
          'region' => 'top',
          'weight' => 10,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;
  $export['sitewide'] = $context;

  return $export;
}
