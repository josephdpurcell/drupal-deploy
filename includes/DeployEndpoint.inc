<?php

class DeployEndpoint {

  var $authentication = NULL;

  var $service = NULL;

  function __construct(DeployAuthentication $authentication) {
    $this->authentication = $authentication;
  }

  function deploy(Traversable $iterator) {
    $this->authentication->deploy($iterator);
  }

}
