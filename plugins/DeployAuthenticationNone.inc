<?php

class DeployAuthenticationNone implements DeployAuthentication {

  var $service = NULL;

  var $config = array();

  function __construct(DeployService $service, Array $config = array()) {
    $this->service = $service;
    $this->config = $config;
  }

  function deploy(Array $entity_wrappers) {
    $this->service->deploy($entity_wrappers);
  }

  function configForm(&$form_state) {
    return array();
  }

}
