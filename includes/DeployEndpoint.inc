<?php

class DeployEndpoint {

  var $authentication = NULL;

  var $service = NULL;

  function __construct(DeployAuthentication $authentication, DeployService $service) {
    $this->authentication = $authentication;
    $this->service = $service;
  }

  function deploy(DeployEntityIteratorIterator $iterator) {
    $this->authentication->authenticate();
    // @todo Create DeployEntityIterator.
    $this->service->deploy($iterator);
  }

}
