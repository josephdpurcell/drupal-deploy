<?php

/**
 * @file
 * This is not a full plugin, but an abstract class that plugins can extend.
 */

class DeployServiceRest implements DeployService {

  var $config = array();

  var $headers = array();

  function __construct(Array $config = array()) {
    $defaults = array(
      'url' => '',
    );
    $this->config = array_merge($defaults, $config);
  }

  function deploy(Traversable $iterator) {}

  function setHeader($key, $value) {
    $this->headers[$key] = $value;
  }

  function httpPut($uuid, $data, $format) {
    $url = $this->config['url'] . '/uuid_entity/' . $uuid . '.' . $format;
    $options = array(
      'method' => 'PUT',
      'headers' => $this->headers,
      'data' => $data
    );

    watchdog('deploy', 'URL: ' . $url, array(), WATCHDOG_DEBUG);
    watchdog('deploy', 'Headers: ' . print_r($this->headers, TRUE), array(), WATCHDOG_DEBUG);
    watchdog('deploy', 'Payload: ' . $data, array(), WATCHDOG_DEBUG);

    drupal_http_request($url, $options);
  }

  function configForm(&$form_state) {
    return array(
      'url' => array(
        '#type' => 'textfield',
        '#title' => t('Endpoint URL'),
        '#description' => t('Enter endpoint URL. Example: %url', array('%url' => 'http://example.com/api/rest')),
        '#default_value' => $this->config['url'],
      ),
    );
  }

}
