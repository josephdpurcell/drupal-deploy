<?php

class DeployEndpoint {

  var $authentication = NULL;

  var $service = NULL;

  function __construct(DeployAuthentication $authentication, DeployService $service) {
    $this->authentication = $authentication;
    $this->service = $service;
  }

  function deploy(Array $entity_ids) {
    $this->authentication->authenticate();
    // @todo Create DeployEntityIterator.
    $this->service->deploy($entity_ids);
  }

}
