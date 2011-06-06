<?php

class DeployEndpoint {

  var $authentication = NULL;

  var $service = NULL;

  function __construct(DeployAuthentication $authentication, DeployService $service) {
    $this->authentication = $authentication;
    $this->service = $service;
  }

  function deploy(Array $entities) {
    $this->authentication->authenticate();
    $this->service->deploy($entities);
  }

}
