<?php

/**
 * @file
 * HID profiles pages.
 */

/**
 * Admin form for managing the system variables associated with this module.
 */
function hid_profiles_settings_form($form, &$form_state) {
  $form['hid_profiles_app_url'] = array(
    '#type' => 'textfield',
    '#title' => t('HID App URL'),
    '#description' => t('The base URL of the Humanitarian ID application.'),
    '#default_value' => variable_get('hid_profiles_app_url', NULL),
    '#required' => TRUE,
  );
  $form['hid_profiles_api_endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('API endpoint'),
    '#description' => t('The base URL for endpoints of the Humanitarian ID Profiles API.'),
    '#default_value' => variable_get('hid_profiles_api_endpoint', NULL),
    '#required' => TRUE,
  );
  $form['hid_profiles_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => t('The machine name of this application as defined in the Humanitarian ID Profiles API.'),
    '#default_value' => variable_get('hid_profiles_api_key', NULL),
    '#required' => TRUE,
  );
  $form['hid_profiles_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('API secret'),
    '#description' => t('The secret key provided by the Humanitarian ID Profiles API.'),
    '#default_value' => variable_get('hid_profiles_api_secret', NULL),
    '#required' => TRUE,
  );
  $form['hid_profiles_v2'] = array(
    '#type' => 'checkbox',
    '#title' => t('HID v2'),
    '#description' => t('Check this box if using the v2 API.'),
    '#default_value' => variable_get('hid_profiles_v2', FALSE),
    '#required' => FALSE,
  );
  $form['hid_profiles_v2_api_endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('API endpoint (v2)'),
    '#description' => t('The base URL for endpoints of Humanitarian ID v2 API.'),
    '#default_value' => variable_get('hid_profiles_v2_api_endpoint', NULL),
    '#required' => FALSE,
  );
  $form['hid_profiles_v2_api_key'] = array(
    '#type' => 'textarea',
    '#title' => t('HID v2 API key'),
    '#description' => t('API key for HID v2'),
    '#default_value' => variable_get('hid_profiles_v2_api_key', NULL),
  );
  return system_settings_form($form);
}

/**
 * Helper function to return the results in json with the right CORS config.
 *
 * @param array $matches
 *   Array of results.
 */
function _hid_profiles_json_helper(array $matches) {
  drupal_add_http_header('Access-Control-Allow-Origin', '*');
  drupal_add_http_header('Access-Control-Allow-Methods', 'GET');
  drupal_json_output($matches);
}

/**
 * Autocomplete callback function.
 *
 * Return an autocomplete JSON object of matching Organizations.
 *
 * @param string $org_names
 *   A keyword for searching Organization names.
 * @param bool $extended
 *   Option to return extended information about organizations.
 *
 * @TODO: Convert this to an endpoint for HID.
 */
function hid_organizations_autocomplete($org_names, $extended = FALSE) {
  $matches = $organizations = array();
  if (empty($org_names)) {
    _hid_profiles_json_helper($matches);
  }

  // Determine the max number of Organizations to return per request.
  $limit = variable_get('hid_organizations_autocomplete_limit', 30);
  // Limit the query to "starts with" or "contains".
  $operator = variable_get('hid_organizations_autocomplete_true', TRUE) ? 'CONTAINS' : 'STARTS_WITH';

  // Query the terms by name.
  $query = new EntityFieldQuery();
  $results = $query
    ->entityCondition('entity_type', 'taxonomy_term')
    ->entityCondition('bundle', 'hr_organization')
    ->propertyCondition('name', db_like($org_names), $operator)
    ->range(0, $limit)
    ->execute();
  if ($data = reset($results)) {
    $organizations = $data;
  }

  $num_results = count($organizations);
  // If there are not enough results, look for acronyms as well.
  if ($num_results < $limit) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'taxonomy_term')
      ->entityCondition('bundle', 'hr_organization')
      ->fieldCondition('field_acronym', 'value', db_like($org_names), $operator);
    // Exclude already retrieved organizations from previous query.
    if ($num_results > 0) {
      $query->propertyCondition('tid', array_keys($organizations), 'NOT IN');
    }
    $results = $query->range(0, $limit - $num_results)
      ->execute();
    if ($data = reset($results)) {
      $organizations += $data;
    }
  }

  if (!empty($organizations) && $terms = entity_load('taxonomy_term', array_keys($organizations))) {
    foreach ($terms as $term) {
      if (isset($term->tid) && $term_wrapper = entity_metadata_wrapper('taxonomy_term', $term)) {
        $key = 'hrinfo_org_' . $term->tid;
        $display_name = $term->name;
        $acronym = $term_wrapper->field_acronym->value();
        if (!empty($acronym)) {
          $display_name .= ' (' . $acronym . ')';
        }

        // If the request was simple, we return just the term name and the
        // acronym, whereas for extended requests, the return array includes more
        // data about the organization.
        if (!$extended) {
          $matches[$key] = $display_name;
        }
        else {
          $org = $term_wrapper->field_organization_type->value();
          if ($org) {
            $matches[$key] = array(
              'name' => $display_name,
              'org_type_id' => 'hrinfo_org_type_' . $term_wrapper->field_organization_type->tid->value(),
              'org_type_name' => $term_wrapper->field_organization_type->name->value(),
            );
          }
          $matches[$key] = array(
            'name' => $display_name,
            'org_type_id' => '',
            'org_type_name' => '',
          );
        }
      }
    }
  }

  _hid_profiles_json_helper($matches);
}

/**
 * Autocmplete callback fucntion.
 *
 * Return an autocomplete JSON object of matching Locations.
 *
 * Generates JSON output of all countries for Locations.
 */
function hid_locations_countries() {
  $countries = array();

  // Get the vocabulary for the Locations to limit the term query by VID.
  $vocabulary = taxonomy_vocabulary_machine_name_load('hr_location');

  // Get the top-level terms for the Location vocabulary.
  $terms = taxonomy_get_tree($vocabulary->vid, 0, 1);

  foreach ($terms as $term) {
    $countries['hrinfo_loc_' . $term->tid] = $term->name;
  }

  _hid_profiles_json_helper($countries);
}

/**
 * Callback function.
 *
 * Return an autocomplete JSON object of matching Locations.
 *
 * Return JSON output of all countries for Locations.
 */
function hid_location_country($country) {
  // country.country_id
  // country.name
  // country.regions
  // country.regions.region_id.name
  // country.regions.region_id.cities
  // country.regions.region_id.cities.city_id.
  $countries = array(
    'country_id'  => $country->tid,
    'name'        => $country->name,
    'regions'     => array(),
  );

  // Get the vocabulary for the Locations to limit the term query by VID.
  $vocabulary = taxonomy_vocabulary_machine_name_load('hr_location');

  // Get the next 2 levels of terms for the Country term as the parent.
  $terms = taxonomy_get_tree($vocabulary->vid, $country->tid, 2);

  foreach ($terms as $term) {
    if (in_array($country->tid, $term->parents)) {
      if (!isset($countries['regions']['hrinfo_loc_' . $term->tid])) {
        $countries['regions']['hrinfo_loc_' . $term->tid] = array(
          'name' => $term->name,
          'cities' => array(),
        );
      }
    }
    else {
      if (!isset($countries['regions']['hrinfo_loc_' . $term->parents[0]])) {
        $countries['regions']['hrinfo_loc_' . $term->parents[0]] = array(
          'name' => $term->name,
          'cities' => array(),
        );
      }
      $countries['regions']['hrinfo_loc_' . $term->parents[0]]['cities']['hrinfo_loc_' . $term->tid] = $term->name;
    }
  }

  _hid_profiles_json_helper($countries);
}

/**
 * HID template loader.
 */
function hid_profiles_load_template($file, $id) {
  $template = DRUPAL_ROOT . '/' . drupal_get_path('module', 'hid_profiles') . '/templates/' . $file . '.html';
  $template = file_get_contents($template);
  $template_elt = array(
    '#tag' => 'script',
    '#attributes' => array(
      'type' => 'text/template',
      'id' => $id,
    ),
    '#value' => $template,
  );
  drupal_add_html_head($template_elt, 'hid_profiles_' . $file);
}

/**
 * Access denied helper.
 */
function hid_profiles_local_contacts_no_access($reason) {
  context_set('context', 'hid_contacts', FALSE);

  $message = '<p>' . t('You can\'t access the contact list for this operation') . '</p>';

  if ($reason == 'anonymous') {
    $img_path = drupal_get_path('module', 'hid_profiles') . '/img';
    $destination = drupal_get_destination();
    $message = theme('hid_profiles_local_contacts_hid_login',
      array(
        'logo_image' => '/' . $img_path . '/message.png',
        'login_button' => l('<img src="/' . $img_path . '/log-in-button.png" />', '//www.humanitarianresponse.info/login', array('query' => array('hid_redirect' => $destination['destination']), 'html' => TRUE)),
        'register_button' => l('<img src="/' . $img_path . '/register-button.png" />', '//humanitarian.id/#register', array('html' => TRUE)),
      )
    );
  }

  if ($reason == 'unverified') {
    $message = '<p>' .
      t('This contact list has been set as private by our field-level administrators. Only "verified" Humanitarian ID users can access the contact list. For information about "verified" users, please see our <a href="@faq-link">FAQs</a> or contact us at <a href="@mail-hrinfo">info@humanitarian.id</a>.',
        array(
          '@faq-link' => 'https://about.humanitarian.id/faqs',
          '@mail-hrinfo' => 'mailto:info@humanitarian.id',
        ))
      . '</p>';
  }

  return array(
    '#theme' => 'hid_profiles_local_contacts_no_access',
    '#message' => $message,
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'hid_profiles') . '/css/hid_profiles_contacts_no_access.css',
      ),
    ),
  );
}

/**
 * Callback function to display the list of local contacts for a node.
 *
 * @param object $node
 *   Operation or Cluster node.
 *
 * @return string
 *   A themed markup blob.
 */
function hid_profiles_local_contacts($node) {
  context_set('context', 'hr_space_menu', FALSE);
  $hid_access = 'open';
  $hid = hid_profiles_get_user_data();
  if ($node->type === 'hr_bundle' || $node->type == 'hr_operation') {
    $operation = $node->type == 'hr_bundle' ? $op = node_load(_hr_bundles_get_operation($node->nid)) : $node;
    $operation_wrapper = entity_metadata_wrapper('node', $operation);

    // Open access operations list their contacts in public, whereas closed, only
    // show to those logged in and verified.
    $hid_access = $operation_wrapper->field_hid_access->value();
  }
  if (!user_is_anonymous()) {
    $templates = array(
      'contacts_list_table_row',
      'contacts_view',
    );
    foreach ($templates as $template) {
      hid_profiles_load_template($template, $template);
    }

    $settings = array();
    $settings['operation_id'] = $operation ? $operation->nid : '';
    // Get the operation id from the cluster.
    $settings['bundle'] = $node->type == 'hr_bundle' ? $node->title : '';
    $settings['bundle_id'] = $node->type == 'hr_bundle' ? $node->nid : '';
    $settings['disaster'] = $node->type == 'hr_disaster' ? $node->title : '';
    $settings['disaster_id'] = $node->type == 'hr_disaster' ? $node->nid : '';
    $settings['api_endpoint'] = variable_get('hid_profiles_v2_api_endpoint', NULL);
    $settings['token'] = hid_profiles_get_token();

    if (!$settings['token']) {
      return hid_profiles_local_contacts_no_access('anonymous');
    }

    return array(
      '#theme' => 'hid_profiles_local_contacts',
      '#attached' => array(
        'js' => array(
          '//cdnjs.cloudflare.com/ajax/libs/underscore.js/1.8.2/underscore-min.js' => array('type' => 'external'),
          '//cdnjs.cloudflare.com/ajax/libs/backbone.js/1.1.2/backbone-min.js' => array('type' => 'external'),
          drupal_get_path('module', 'hid_profiles') . '/js/hid_profiles_contacts.js' => array('type' => 'file'),
          array(
            'data' => array('hid_profiles' => $settings),
            'type' => 'setting'
          ),
        ),
        'library' => array(
          array('chosen', 'drupal.chosen'),
          array('system', 'ui.autocomplete'),
        ),
      ),
    );
  }
  else {

    // Display the HID Login / Register screen for anonymous users.
    if (user_is_anonymous()) {
      $reason = 'anonymous';
    }

    // Display a specific message to non verified users.
    if (($hid_access == 'closed' && empty($hid->profile->verified)) || !$hid->profile->verified) {
      $reason = 'unverified';
    }

    return hid_profiles_local_contacts_no_access($reason);
  }
}
