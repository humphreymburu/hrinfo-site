<?php

/**
 * @file
 * Generic OpenID Connect client.
 *
 * Used primarily to login to Drupal sites powered by oauth2_server or PHP
 * sites powered by oauth2-server-php.
 */

class OpenIDConnectClientGeneric extends OpenIDConnectClientBase {

  /**
   * {@inheritdoc}
   */
  public function settingsForm() {
    $form = parent::settingsForm();

    $default_site = 'https://example.com/oauth2';

    $form['autodiscover_endpoints'] = array(
      '#title' => t('Auto discover endpoints'),
      '#type' => 'checkbox',
      '#description' => t(
        'Requires that IdP supports "<a href="@url" target="_blank">Well-Known URI Registry</a>".',
        array('@url' => 'https://openid.net/specs/openid-connect-discovery-1_0.html')
      ),
      '#default_value' => $this->getSetting('autodiscover_endpoints', FALSE),
    );

    // Auto discover fields.
    $form['issuer_url'] = array(
      '#title' => t('Issuer URL'),
      '#type' => 'textfield',
      '#default_value' => $this->getSetting('issuer_url', $default_site),
      '#states' => array(
        'visible' => array(
          ':input[name="clients[' . $this->name . '][autodiscover_endpoints]"]' => array('checked' => TRUE),
        ),
      ),
    );

    // Classic fields.
    $form['authorization_endpoint'] = array(
      '#title' => t('Authorization endpoint'),
      '#type' => 'textfield',
      '#default_value' => $this->getSetting('authorization_endpoint', $default_site . '/authorize'),
      '#states' => array(
        'visible' => array(
          ':input[name="clients[' . $this->name . '][autodiscover_endpoints]"]' => array('checked' => FALSE),
        ),
      ),
    );
    $form['token_endpoint'] = array(
      '#title' => t('Token endpoint'),
      '#type' => 'textfield',
      '#default_value' => $this->getSetting('token_endpoint', $default_site . '/token'),
      '#states' => array(
        'visible' => array(
          ':input[name="clients[' . $this->name . '][autodiscover_endpoints]"]' => array('checked' => FALSE),
        ),
      ),
    );
    $form['userinfo_endpoint'] = array(
      '#title' => t('UserInfo endpoint'),
      '#type' => 'textfield',
      '#default_value' => $this->getSetting('userinfo_endpoint', $default_site . '/UserInfo'),
      '#states' => array(
        'visible' => array(
          ':input[name="clients[' . $this->name . '][autodiscover_endpoints]"]' => array('checked' => FALSE),
        ),
      ),
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function getEndpoints() {
    if ($this->getSetting('autodiscover_endpoints', FALSE)) {
      $discover = $this->discoverRequest($this->getSetting('issuer_url'));
      $endpoints = array();

      foreach ($discover as $attribute => $value) {
        if (preg_match('/_endpoint$/', $attribute)) {
          $attribute = preg_replace('/_endpoint$/', '', $attribute);
          $endpoints[$attribute] = $value;
        }
      }

      return $endpoints;
    }
    else {
      return array(
        'authorization' => $this->getSetting('authorization_endpoint'),
        'token' => $this->getSetting('token_endpoint'),
        'userinfo' => $this->getSetting('userinfo_endpoint'),
      );
    }
  }

  /**
   * Get Issuer info.
   * @see https://openid.net/specs/openid-connect-discovery-1_0.html#ProviderConfig
   *
   * @param string $issuer_url
   * @return bool|mixed
   */
  protected function discoverRequest($issuer_url) {
    if (!$issuer_url) {
      return FALSE;
    }

    $well_known_config_url = rtrim($issuer_url, '/') . '/.well-known/openid-configuration';
    $request_url = url($well_known_config_url, array('external' => TRUE));

    // Get data from idp openid configuration.
    $response = drupal_http_request($request_url);

    if (!isset($response->error) && $response->code == 200) {
      $response_data = drupal_json_decode($response->data);

      return $response_data;
    }
    else {
      openid_connect_log_request_error(__FUNCTION__, $this->name, $response);

      return FALSE;
    }
  }
}
