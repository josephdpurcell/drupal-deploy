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

  function deploy(Array $entity_wrappers) {}

  function httpPost($entity_type, $entity_id, $data, $format) {
    $url = $this->config['url'] . '/' . $entity_type . '/' . $entity_id . '.' . $format;
    $options = array(
      'method' => 'POST',
      'headers' => $this->headers,
      'data' => $data
    );
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
