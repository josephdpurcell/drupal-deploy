<?php

class DeployEndpoint {

  var $authenticator = NULL;

  var $service = NULL;

  function __construct(DeployAuthenticator $authenticator) {
    $this->authenticator = $authenticator;
  }

  function deploy(Traversable $iterator) {
    $this->authenticator->deploy($iterator);
  }

}
