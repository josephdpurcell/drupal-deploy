<?php

/**
 * @file
 * This is not a full plugin, but an abstract class that plugins can extend.
 */

class DeployServiceRest implements DeployService {

  var $config = array();

  var $headers = array();

  protected $plan;

  function __construct(DeployPlan $plan, Array $config = array()) {
    $this->config += array(
      'debug' => FALSE,
      'url' => '',
      'headers' => array(),
    );
    $this->config = array_merge($this->config, $config);
    $this->plan = $plan;
  }

  function deploy(Traversable $iterator) {}

  function httpPut($uuid, $data, $format) {
    $url = $this->config['url'] . '/uuid_entity/' . $uuid . '.' . $format;
    $options = array(
      'method' => 'PUT',
      'headers' => $this->config['headers'],
      'data' => $data
    );

    if ($this->config['debug']) {
      watchdog('deploy', 'Request URL: %url', array('%url' => $url), WATCHDOG_DEBUG);
      watchdog('deploy', 'Request headers: <pre>@headers</pre>', array('@headers' => print_r($this->headers, TRUE)), WATCHDOG_DEBUG);
      watchdog('deploy', 'Request data: <pre>@data</pre>', array('@data' => $data), WATCHDOG_DEBUG);
    }

    $response = drupal_http_request($url, $options);

    if ($this->config['debug']) {
      watchdog('deploy', 'Request response: <pre>@response</pre>', array('@response' => print_r($response, TRUE)), WATCHDOG_DEBUG);
    }
    if (isset($response->error)) {
      watchdog('deploy', 'Request response error: %code %error', array('%code' => $response->code, '%error' => $response->error), WATCHDOG_ERROR);
    }
    return $response;
  }

  function configForm(&$form_state) {
    return array(
      'url' => array(
        '#type' => 'textfield',
        '#title' => t('Endpoint URL'),
        '#description' => t('Enter endpoint URL. Example: %url', array('%url' => 'http://example.com/api')),
        '#default_value' => $this->config['url'],
      ),
    );
  }

}
