<?php

class DeployAuthenticationOAuth implements DeployAuthentication {

  var $config = array();

  function __construct(Array $config) {
    $this->config = $config;
  }

  function authenticate() {
    return TRUE;
  }

}
