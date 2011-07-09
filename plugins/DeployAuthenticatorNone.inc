<?php

class DeployAuthenticatorNone implements DeployAuthenticator {

  var $service = NULL;

  var $config = array();

  function __construct(DeployService $service, Array $config = array()) {
    $this->service = $service;
    $this->config += array(
      'debug' => FALSE,
    );
    $this->config = array_merge($this->config, $config);
  }

  function deploy(Traversable $iterator) {
    $this->service->deploy($iterator);
  }

  function configForm(&$form_state) {
    return array();
  }

}
